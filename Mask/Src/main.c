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


uint8_t contador 	= 0;
uint8_t bandera 	= 0;

/*
 * The main function	, where everything happens.
 */
//uint8_t isPar(uint8_t numero);
//void isPar(int numero);
//void isImpar(int numero);
//uint8_t isPar(void);
void isPar(void);
int main(void)
{

	contador = 60;
//	bandera = isPar(contador);

//	isPar(contador);

	isPar();

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

//void isPar(int numero){
//
//	numero = numero%2;
//	//si numero es impar entra a la condicion de if
//	if (numero){
//		// si el numero es impar la bandera tomara el valor de 0
//		bandera = 0;
//	}
//	else{
//		// si el numero es par entrar a esta conficion y bandera es igual a 1
//		bandera = 1;
//	}
//
//}



//void isImpar(int numero){
//
//	numero = numero%2;
//	//si numero es impar entra a la condicion de if
//	if (!numero){
//		// si el numero es impar la bandera tomara el valor de 0
//		bandera = 0;
//	}
//	else{
//		// si el numero es par entrar a esta conficion y bandera es igual a 1
//		bandera = 1;
//	}
//
//}
//
//uint8_t isPar(void){
//
//	contador = contador%2;
//	uint8_t condicion = 0;
//
//	if(contador){
//		condicion = 0;
//	}
//	else{
//		condicion = 1;
//	}
//
//	return condicion;
//}
//
void isPar(void){

	contador = contador%2;

	if(contador){
		bandera = 0;
	}
	else{
		bandera = 1;
	}


}

