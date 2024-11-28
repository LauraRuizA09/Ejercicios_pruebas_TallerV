/**
 ******************************************************************************
 * @file           : main.c
 * @author         : namontoy@unal.edu.co
 * @brief          : Here starts the magic!!
 ******************************************************************************
 */


#include <stdint.h>
#include <stdio.h>
#include <stm32f4xx.h>


// Definicion de las cabeceras de las funciones del main
extern void configMagic(void);


/*
 * The main function, where everything happens.
 */
int main(void)
{
	/* Configuramos la magia! */
	configMagic();






	while (1) {
	}

	return 0;
}






