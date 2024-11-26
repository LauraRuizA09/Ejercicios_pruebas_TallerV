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



// Definicion variables globales

//Variables mas estandar (mejor utilizar estas)

uint8_t dias_semana;
int32_t milisegundos;

//Variables primitivas

//El tamaño de estas cambian segun el compilador (procesador arm)

int huevos;

// Definicion de las cabeceras de las funciones de la clase

uint16_t operacionclase(uint16_t budget, uint16_t cost);


// Definicion de las cabeceras de las funciones del main
extern void configMagic(void);


/*
 * The main function, where everything happens.
 */
int main(void)
{
	/* Configuramos la magia! */
	configMagic();

	//Variables locales, solo existen dentro de la funcion (utilizar mas)

	//definidas su tipo e inicializadas
	uint16_t presupuesto = 10000;
	uint16_t compras = 5000;
	uint16_t operacion = 0;

	//operacion=presupuesto - compras; //Esto es una sentencia

	operacion =  operacionclase(presupuesto, compras);

	printf("Me fui de compras\n");
	printf("El presupuesto fue de: %\d\n",presupuesto);
	printf("Las compras fueron de: %\d\n",compras);



	// Ciclo principal


//		printf("Hola Mundo!\n");
//		printf("Characters: %c %c\n", 'a', 65);
//		printf("Decimals: %d %ld\n", 1977, 650000L);
//		printf("Preceding with blanks: %10d\n", 1977);
//		printf("Preceding with zeros: %010d\n", 1977);
//		printf("Some different radices: %d %x %o %#x %#o\n", 100, 100, 100, 100, 100);
//		printf("floats: %4.2f %+.0e %E\n", 3.1416, 3.1416, 3.1416);
//		printf("Width trick: %*d\n", 5, 10);
//		printf("%s\n", "A string");

	while (1) {
	}

	return 0;
}

//Ejemplo clase

uint16_t operacionclase(uint16_t budget, uint16_t cost){

	uint16_t auxoperation;

	auxoperation = budget - cost;

	return auxoperation;


}

