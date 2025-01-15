/*
 * timer_driver_hal.c
 *
 *  Created on: 6/12/2024
 *      Author: laura
 */

#include "stm32f4xx.h"
#include "stm32_assert.h"

#include "timer_driver_hal.h"

/*
 * Variable que guarda la referecnia del periferico que se esta utilizando
 */

TIM_TypeDef *ptrTimerUsed;

/*
 * __-Headers for privvate functions___-
 */
static void timer_enable_clock_peripheral(Timer_Handler_t *pTimerHandler);
static void timer_set_prescaler(Timer_Handler_t *pTimerHandler);
static void timer_set_period(Timer_Handler_t *pTimerHandler);
static void timer_set_mode(Timer_Handler_t *pTimerHandler);
static void timer_config_interrupt(Timer_Handler_t *pTimerHandler);

/*
 * Funcion en la que cargamos la configuracion del Timer
 * Recordar que siempre se debe comenzar con activar la señal de reloj
 * del periferico que se esta utilizando.
 *
 * Ademas, en este caso, debemos ser cuidadosos al momento de utilizar las interrupciones.
 * Los timer estan conectados directamente al elemento NVIC del Cortex-Mx
 * debemos configurar y/o utilizar:
 *
 *  - TIMx_CR1  (Control register1)
 *  - TIMx_SMCR (slave mode control register) -> mantener en 0 para modo timer basico
 *  - TIMx_DIER (DMA and Interrupt enable register)
 *  - TIMx_SR (Status register9
 *  - TIMx_CNT (counter)
 *  - TIMx_PSC (Pre-scaler)
 *  - TIMx_ARR (auto-reload register)
 *
 *  Como vamos a trabajar con interrupciones antes de configurar una nueva, debemos desactivar
 *  el sistema global de interrupciones, activar la IQR especifica y luego volver a encender
 *  el sistema.
 */


void timer_Config(Timer_Handler_t *pTimerHandler){

	//Guardamos una referecnia al periferico que estamos utilizando...
	ptrTimerUsed = pTimerHandler->pTIMx;

	/* 0. Desactivamos las interrupciones globales mienstras configuramos el sistema*/
	__disable_irq();

	/* 1. Activar la señal de reloj del periferico requerido*/
	timer_enable_clock_peripheral(pTimerHandler);

	/* 2. Configuramos el Pre- Scaler*/
	timer_set_prescaler(pTimerHandler);

	/* 3. Configuramos si UP_COUNT o DOWN_COUNT (mode)*/
	timer_set_mode(pTimerHandler);

	/*4. Configuramos el auto- reload*/
	timer_set_period(pTimerHandler);

	/* 5. Configuramos la interrupcion*/
	timer_config_interrupt(pTimerHandler);

	/* x. Volvemos a activar las interrupciones del sistema*/
	__enable_irq();

	/*El timer inicia apagado*/
	timer_SetState(pTimerHandler, TIMER_OFF);

}

/*
 *
 */
void timer_enable_clock_peripheral(Timer_Handler_t *pTimerHandler){

	//Verificamos que es un timer permitido
	assert_param(IS_TIM_INSTANCE(pTimerHandler->pTIMx));

	if(pTimerHandler->pTIMx == TIM2){
		RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	}
	else if(pTimerHandler->pTIMx == TIM3){
		RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	}
	else if(pTimerHandler->pTIMx == TIM4){
		RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
	}
	else if(pTimerHandler->pTIMx == TIM5){
		RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;
	}
	else if(pTimerHandler->pTIMx == TIM9){
		RCC->APB2ENR |= RCC_APB2ENR_TIM9EN;
	}
	else if(pTimerHandler->pTIMx == TIM10){
		RCC->APB2ENR |= RCC_APB2ENR_TIM10EN;
	}
	else if(pTimerHandler->pTIMx == TIM11){
		RCC->APB2ENR |= RCC_APB2ENR_TIM11EN;
	}
	else{
		__NOP();
	}
}

/*
 * Elprescaler nos configura la velocidad a la que se incrementa el registro
 */

void timer_set_prescaler(Timer_Handler_t *pTimerHandler){

	//Verificamos que el valor del prescaler es valido
	assert_param(IS_TIMER_PRESC(pTimerHandler->TIMx_Config.TIMx_Prescaler));

	//Configuramos el valor del prescaler
	pTimerHandler->pTIMx->PSC = pTimerHandler->TIMx_Config.TIMx_Prescaler - 1; //16000000
}

/*
 * ESta funcion configura el limite hasta donde cuenta el Timer para generar un
 * eveto "update" (cuando esta contando de forma ascendente), o configura
 * el valor desde donde se comienza a contar, cuando el sistema fucniona de forma
 * descendente.
 */

void timer_set_period(Timer_Handler_t *pTimerHandler){

	//Verificamos que el valor que gemera el periodo es valido
	assert_param(IS_TIMER_PERIOD(pTimerHandler->TIMx_Config.TIMx_Prescaler));

	//Aca hace falta algo.....

	//CONFIGURAMOS EL VALOR DEL AUTORELOAD
	pTimerHandler->pTIMx->ARR = pTimerHandler->TIMx_Config.TIMx_Period - 1;

}


/*
 * Upcounter or Downcounter
 */

void timer_set_mode(Timer_Handler_t *pTimerHandler){

	//Verificamos que el modo de funcionamiento es correcto
	assert_param(IS_TIMER_MODE(pTimerHandler->TIMx_Config.TIMx_mode));

	//Verificamos cual es el modo que se desea configurar
	if(pTimerHandler->TIMx_Config.TIMx_mode == TIMER_UP_COUNTER){

		//Configuramos en mdoo UPcouner DIR = 0
		pTimerHandler->pTIMx->CR1 &= ~TIM_CR1_DIR;
	}
	else{

		//Configuramos en modo DOWNcounter DIR = 1
		pTimerHandler->pTIMx->CR1 |= TIM_CR1_DIR;
	}
}


/*
 *
 */

void timer_config_interrupt(Timer_Handler_t *pTimerHandler){

	//Verificamos el posible valor configurado
	assert_param(IS_TIMER_INTERRUP(pTimerHandler->TIMx_Config.TIMx_InterruptEnable));

	if(pTimerHandler->TIMx_Config.TIMx_InterruptEnable == TIMER_INT_ENABLE){

		/*Activamos la interrupcion debida al TImerx utilizado*/
		pTimerHandler->pTIMx->DIER |= TIM_DIER_UIE;


		/*Activamos el canal del sistema NVIC para que lea la interrupcion*/
		if(pTimerHandler->pTIMx == TIM2){
			NVIC_EnableIRQ(TIM2_IRQn);
		}
		else if(pTimerHandler->pTIMx == TIM3){
			NVIC_EnableIRQ(TIM3_IRQn);
		}
		else if(pTimerHandler->pTIMx == TIM4){
			NVIC_EnableIRQ(TIM4_IRQn);
		}
		else if(pTimerHandler->pTIMx == TIM5){
			NVIC_EnableIRQ(TIM5_IRQn);
		}
		else if(pTimerHandler->pTIMx == TIM9){
			NVIC_EnableIRQ(TIM1_BRK_TIM9_IRQn);
		}
		else if(pTimerHandler->pTIMx == TIM10){
			NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);
		}
		else if(pTimerHandler->pTIMx == TIM11){
			NVIC_EnableIRQ(TIM1_TRG_COM_TIM11_IRQn);
		}
		else{
			__NOP();
		}
	}
}

/*
 *
 */

void timer_SetState(Timer_Handler_t *pTimerHandler, uint8_t newState){

	//Verificamos que el estado ingresado es adecuado*/
	assert_param(IS_TIMER_STATE(newSTATE));

	/* 4. Reiniciamos el registro counter*/
	pTimerHandler->pTIMx->CNT = 0;

	if(newState == TIMER_ON){

		/*5a. ACtivamos el timer (el CNT debe empezar a contar)*/
		pTimerHandler->pTIMx->CR1 |= TIM_CR1_CEN;
	}
	else{
		/*5b. Desactivamos el Timer (el CNT debe detenerse)*/
		pTimerHandler->pTIMx->CR1 &= ~TIM_CR1_CEN;
	}

}

/**/

__attribute__((weak)) void Timer2_Callback(void){
	__NOP();
}


__attribute__((weak)) void Timer3_Callback(void){
	__NOP();
}


__attribute__((weak)) void Timer4_Callback(void){
	__NOP();
}

__attribute__((weak)) void Timer5_Callback(void){
	__NOP();
}

/*
 * TIMER TIM9 - TIM11
 *
 */

__attribute__((weak)) void Timer9_Callback(void){
	__NOP();
}

__attribute__((weak)) void Timer10_Callback(void){
	__NOP();
}

__attribute__((weak)) void Timer11_Callback(void){
	__NOP();
}

/*
 *  Esta es la funcion a la que apunta el sistema en el vector de interrupciones.
 *  Se debe utilizar usando exactamente el mismo nombre definido en el vector de interrupciones
 *  AL hacerlo correctamente, el sistema apunta a esta funcion y cuando la interrupcion se la
 *  el sistema inmediatamente salta a este lugar en la memoria
 */

void TIM2_IRQHandler(void){

	/*Limpiamos la bandera que indica que la interrupcion se ha generado*/
	TIM2->SR &= ~TIM_SR_UIF;

	/*Llamamos a la funcion que se dbe encargar de hacer algo con esta interrupcion*/
	Timer2_Callback();
}


void TIM3_IRQHandler(void){

	/*Limpiamos la bandera que indica que la interrupcion se ha generado*/
	TIM3->SR &= ~TIM_SR_UIF;

	/*Llamamos a la funcion que se dbe encargar de hacer algo con esta interrupcion*/
	Timer3_Callback();
}



void TIM4_IRQHandler(void){

	/*Limpiamos la bandera que indica que la interrupcion se ha generado*/
	TIM4->SR &= ~TIM_SR_UIF;

	/*Llamamos a la funcion que se dbe encargar de hacer algo con esta interrupcion*/
	Timer4_Callback();
}



void TIM5_IRQHandler(void){

	/*Limpiamos la bandera que indica que la interrupcion se ha generado*/
	TIM5->SR &= ~TIM_SR_UIF;

	/*Llamamos a la funcion que se dbe encargar de hacer algo con esta interrupcion*/
	Timer5_Callback();
}



void TIM9_IRQHandler(void){

	/*Limpiamos la bandera que indica que la interrupcion se ha generado*/
	TIM9->SR &= ~TIM_SR_UIF;

	/*Llamamos a la funcion que se dbe encargar de hacer algo con esta interrupcion*/
	Timer9_Callback();
}


void TIM10_IRQHandler(void){

	/*Limpiamos la bandera que indica que la interrupcion se ha generado*/
	TIM10->SR &= ~TIM_SR_UIF;

	/*Llamamos a la funcion que se dbe encargar de hacer algo con esta interrupcion*/
	Timer10_Callback();
}


void TIM11_IRQHandler(void){

	/*Limpiamos la bandera que indica que la interrupcion se ha generado*/
	TIM11->SR &= ~TIM_SR_UIF;

	/*Llamamos a la funcion que se dbe encargar de hacer algo con esta interrupcion*/
	Timer11_Callback();
}



