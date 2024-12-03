/**
 ******************************************************************************
 * @file           : main.c
 * @author         : namontoy@unal.edu.co
 * @brief          : Here starts the magic!!
 ******************************************************************************
 */


// Directivas de preprocesamiento

#include <stdint.h>
#include <stm32f4xx_hal.h>
#include "stm32_assert.h"
#include "gpio_driver_hal.h"



// Definimos un pin de prubea
GPIO_Handler_t userLed = {0}; //PINA5

/*
 * The main function, where everything happens.
 */
int main(void)
{
    /* Configuramos el pin */
    userLed.pGPIOx                        = GPIOA;
    userLed.pinConfig.GPIO_PinNumber      = PIN_5;
    userLed.pinConfig.GPIO_PinMode        = GPIO_MODE_OUT;
    userLed.pinConfig.GPIO_PinOutputType  = GPIO_OTYPE_PUSHPULL;
    userLed.pinConfig.GPIO_PinOutputSpeed = GPIO_OSPEED_MEDIUM;
    userLed.pinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;

    /* Cargamos la configuración en los registros que gobiernan el puerto */
    gpio_Config(&userLed);

    gpio_WritePin(&userLed, SET);


    while (1) {
    }

    return 0;
}

/*
 * esta funcion sirve para detectar problemas de parametros
 * incorrectos al momento de ejecturar un programa
 */
void assert_failed(uint8_t* file , uint32_t line){
    while(1){
        //problems...
    }
}

