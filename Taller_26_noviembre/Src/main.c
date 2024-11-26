/**
 ******************************************************************************
 * @file           : main.c
 * @author         : namontoy@unal.edu.co
 * @brief          : Here starts the magic!!
 ******************************************************************************
 */

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

uint8_t contador 	= 0;
int segundos	= 0;
uint8_t bandera 	= 0;
uint32_t ejecuciones= 0;

/*
 * The main function, where everything happens.
 */

void isPar(int numero);

int main(void)
{

	contador = 59;
//	bandera = isPar(contador);

	isPar(contador);


	while (1) {
	}
	return 0;
}

////Esta funcion retorna 1 si es impar
//uint8_t isPar(uint8_t numero){
//
//	uint8_t condicion = 0;
//
//	condicion = numero%2;
//
//	return condicion;
//}

void isPar(int numero){

	numero = numero%2;
	//si numero es impar entra a la condicion de if
	if (numero){
		// si el numero es impar la bandera tomara el valor de 0
		bandera = 0;
	}
	else{
		// si el numero es par entrar a esta conficion y bandera es igual a 1
		bandera = 1;
	}

}
//
//uint8_t isPar(void){
//	return condicion;
//}
//
//void isPar(void){
//}

