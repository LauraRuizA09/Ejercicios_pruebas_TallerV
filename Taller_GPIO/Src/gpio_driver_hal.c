/*
 * gpio_driver_hal.c
 *
 *  Created on: 29/11/2024
 *      Author: laura ruiz arango
 */


#include "gpio_driver_hal.h"
#include "stm32f4xx_hal.h"
#include "stm32_assert.h"  //ayudarnos a verificar errores

/*___Headers for private functions___*/

void gpio_enable_clock_peripheral(GPIO_Handler_t *pGPIOHandler);
void gpio_config_mode(GPIO_Handler_t *pGPIOHandler);
void gpio_config_output_type(GPIO_Handler_t *pGPIOHandler);
void gpio_config_output_speed(GPIO_Handler_t *pGPIOHandler);
void gpio_config_pullup_pulldown(GPIO_Handler_t *pGPIOHandler);
void gpio_config_alternate_function(GPIO_Handler_t *pGPIOHandler);


/*
 * Para cualquier periferico hay varios pasos que siempre se deben seguir en un
 * orden escrito para poder que el sistema permita configurar el periferico X.
 * Lo primero y mas importante es activar la señal del reloj principal hacia ese
 * elemento especifico (relacionado con el periferico RCC), a esto llamaremos
 * simplemente "activar el periferico o activar la señal de reloj del perfierico)
 */

void gpio_Config (GPIO_Handler_t *pGPIOHandler){

	/*Verificamos que el pin seleccionado es correcto */
	assert_param(IS_GPIO_PIN(pGPIOHandler->pinConfig.GPIO_PinNumber));

	//1) ACtivar el periferico
	gpio_enable_clock_peripheral(pGPIOHandler);

	//Despues de activado, podemos comenzar a configurar

	//2) Configuracion del registro GPIOx_MODER
	gpio_config_mode(pGPIOHandler);

	//3) Configuracion del registro GPIOx_OTYPER
	gpio_config_output_type(pGPIOHandler);

	//4) Configuracion de la velocidad
	gpio_config_output_speed(pGPIOHandler);

	//5) Configurando si se desea pull-up, pull-down o flotante
	gpio_config_pullup_pulldown(pGPIOHandler);

	//6) Configuracion de las fucniones alternativas
	gpio_config_alternate_function(pGPIOHandler);

}  //FIN del GPIO_config


/*Enable clock for specific GPIOx port */

void gpio_enable_clock_peripheral(GPIO_Handler_t *pGPIOHandler){

	//Verificamos que el puerto configurado si es permitido
	assert_param(IS_GPIO_ALL_INSTANCE(pGPIOHandler->pGPIOx));


	//Verificmaos para GPIOA
		if(pGPIOHandler->pGPIOx == GPIOA){

			//Escribimos 1 (SET) en la posicion correspondiente al GPIOA
			RCC->AHB1ENR |= (1 << RCC_AHB1ENR_GPIOAEN);
		}

		//Verificamos para GPIOB
		else if(pGPIOHandler->pGPIOx == GPIOB){

			//EScribimos 1 (SET) en la posicion correspondinete al GPIOB
			RCC->AHB1ENR |= (1 << RCC_AHB1ENR_GPIOBEN);
		}

		//Verificamos para GPIOC
		else if(pGPIOHandler->pGPIOx == GPIOC){

					//EScribimos 1 (SET) en la posicion correspondinete al GPIOC
			RCC->AHB1ENR |= (1 << RCC_AHB1ENR_GPIOCEN);
		}


		//Verificamos para GPIOD
		else if(pGPIOHandler->pGPIOx == GPIOD){

					//EScribimos 1 (SET) en la posicion correspondinete al GPIOD
			RCC->AHB1ENR |= (1 << RCC_AHB1ENR_GPIODEN);
		}


		//Verificamos para GPIOE
		else if(pGPIOHandler->pGPIOx == GPIOE){

					//EScribimos 1 (SET) en la posicion correspondinete al GPIOE
			RCC->AHB1ENR |= (1 << RCC_AHB1ENR_GPIOEEN);
		}


		//Verificamos para GPIOH
		else if(pGPIOHandler->pGPIOx == GPIOH){

					//EScribimos 1 (SET) en la posicion correspondinete al GPIOH
			RCC->AHB1ENR |= (1 << RCC_AHB1ENR_GPIOHEN);
		}

}

/*Configures the mode in which the pin will work:
 *   - Input
 *   - Output
 *   - Analog
 *   - Alternate Function
 */

void gpio_config_mode(GPIO_Handler_t *pGPIOHandler){

	uint32_t auxConfig = 0;

	/* Verificamos si el modo que se ha seleccionado es permitido */
	assert_param(IS_GPIO_MODE(pGPIOHandler->pinConfig.GPIO_PinMode));

	//Aca estamos leyendo la config. moviendo "PinNUmber" veces hacia la izquierda ese valor  (shift left)
	// y todo eso lo cargamos en la variable auxConfig
	auxConfig = (pGPIOHandler->pinConfig.GPIO_PinMode << 2 *pGPIOHandler->pinConfig.GPIO_PinNumber);

	//Antes de cargar el nuevo valor, limpiamos los bits especificos de ese registro (debemos escribir 0b00)
	// para lo cual aplicamos una mascara y una operacion bitwise AND
	pGPIOHandler->pGPIOx->MODER &= ~(0b11 << 2 *pGPIOHandler->pinConfig.GPIO_PinNumber);

	//Cargamos a auxconfig en el registro MODER
	pGPIOHandler->pGPIOx->MODER |= auxConfig;

}

/*
 * Configures which type of output the PinX will use:
 * 	- Push-Pull
 * 	- openDrain
 */

void gpio_config_output_type(GPIO_Handler_t *pGPIOHandler){

	uint32_t auxConfig = 0;

	/* Verificamos que el tipo de salida corresponda a los que se pueden utilizar*/
	assert_param(IS_GPIO_OUTPUT_TYPE(pGPIOHandler->pinConfig.GPIO_PinOutputType));

	//De nuevo, leemos y movemos el valor un numero "PinNUmber" de veces
	auxConfig = (pGPIOHandler->pinConfig.GPIO_PinOutputType << pGPIOHandler->pinConfig.GPIO_PinNumber);

	//Limpiamos antes de cargar
	pGPIOHandler->pGPIOx->OTYPER &= ~(SET << pGPIOHandler->pinConfig.GPIO_PinNumber);

	//Cargamos el resultado sobre el registro adecuado
	pGPIOHandler->pGPIOx->OTYPER |= auxConfig;

}

/*
 * Selects between four different possible speeds for output PinX
 * 	- Low
 * 	- Medium
 * 	- Fast
 * 	- HighSpeed
 */

void gpio_config_output_speed(GPIO_Handler_t *pGPIOHandler){

	uint32_t auxConfig = 0;

	/**/
	assert_param(IS_GPIO_OSPEED(pGPIOHandler->pinConfig.GPIO_PinOutSpeed));

	auxConfig = (pGPIOHandler->pinConfig.GPIO_PinOutputSpeed << 2*pGPIOHandler->pinConfig.GPIO_PinNumber);

	//LImpiando la posicion antes de cargar la nueva configuracion
	pGPIOHandler->pGPIOx->OSPEEDR &= ~(0b11 << 2* pGPIOHandler->pinConfig.GPIO_PinNumber);

	//Cragamos los resultados sobre el registro adecuado
	pGPIOHandler->pGPIOx->OSPEEDR |=auxConfig;
}

/*
	 * Turns ON/OFF the pull-up and pull-down resistor for each PinX in selected GPIO port
	 */


void gpio_config_pullup_pulldown(GPIO_Handler_t *pGPIOHandler){

	uint32_t auxConfig = 0;

	/* Verificamos si la configuracion cargada para las resistencias es correcta*/
	assert_param(IS_GPIO_PUPDR(pGPIOHandler->pinConfig.GPIO_PinPuPdControl));

	auxConfig = (pGPIOHandler->pinConfig.GPIO_PinPuPdControl << 2*pGPIOHandler->pinConfig.GPIO_PinNumber);

	//Limpiando la posicion antes de cargar la nueva configuracion
	pGPIOHandler->pGPIOx->PUPDR &= ~(0b11 << 2 * pGPIOHandler->pinConfig.GPIO_PinNumber);

	//Cargamos  el resuelto sobre el registro adecuado
	pGPIOHandler->pGPIOx->PUPDR |= auxConfig;

}

/*
* Allows to configurate other functions (more specialized) on the selected PinX
*/



void gpio_config_alternate_function(GPIO_Handler_t *pGPIOHandler){

	uint32_t auxPosition = 0;

	if(pGPIOHandler->pinConfig.GPIO_PinMode == GPIO_MODE_ALTFN){

		//Seleccionamos primero si se debe utilizar el registro bajo (AFRL) o el alto (AFRH)

		if(pGPIOHandler->pinConfig.GPIO_PinNumber < 8){

			//estamos en el registro AFRL, que controla los pines del PIN 0 al PIN 7
			auxPosition = 4 *pGPIOHandler->pinConfig.GPIO_PinNumber;

			//limpiamos primero la posicion del registro que deseamos escribir a continuacion
			pGPIOHandler->pGPIOx->AFR[0] &= ~(0b1111 << auxPosition);

			//y escribimos el valor configurado en la posicion seleccionada
			pGPIOHandler->pGPIOx->AFR[0] |= (pGPIOHandler->pinConfig.GPIO_PinAltFunMode << auxPosition);

		}

		else{
			//estamos en el registro AFRH, que controla los pines del PIN 8 al PIN 15
			auxPosition = 4 *(pGPIOHandler->pinConfig.GPIO_PinNumber -8);

			//Limpiamos primero la posicion del registro que deseamos escribir a continuacion
			pGPIOHandler->pGPIOx->AFR[1] &= ~(0b1111 << auxPosition);

			// y escribimos el valor configurado en la posicion seleccionada
			pGPIOHandler->pGPIOx->AFR[1] |= (pGPIOHandler->pinConfig.GPIO_PinAltFunMode << auxPosition);
		}
	}
}

void gpio_WritePin(GPIO_Handler_t *pPinHandler, uint8_t newState){


	/*Verficamos si la accion que deseamos realizar es permitida*/
	assert_param(IS_GPIO_PIN_ACTION(newState));

	//LImpiamos la posicion que deseamos//
	//pPinHandler->pGPIOx->ODR &= ~(SET << pPinHandler->pinConfig.GPIO_PinNumber);

	if (newState == SET){

		//Trabajando con la parte baja del registro
		pPinHandler->pGPIOx->BSRR |= (SET <<pPinHandler->pinConfig.GPIO_PinNumber);
	}
	else{

		//TRabajamos con la parte alta del registro
		pPinHandler->pGPIOx->BSRR |= (SET << (pPinHandler->pinConfig.GPIO_PinNumber + 16));
	}



}


/*
 * Funcion para leer el estado de un pin especifico
 */

uint32_t gpio_ReadPin(GPIO_Handler_t *pPinHandler){

	//creamos una variable auxiliar la cual luego retornaremos

	uint32_t pinValue = 0;

	//Cargamos el valor del registro IDR, desplazado a la derecha tantas veces como la ubicacion
	// del pin espeifico

	pinValue = (pPinHandler->pGPIOx->IDR << pPinHandler->pinConfig.GPIO_PinNumber);

	return pinValue;
}






















