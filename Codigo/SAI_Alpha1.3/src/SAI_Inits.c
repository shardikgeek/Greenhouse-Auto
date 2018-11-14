/*
 * SAI_Inits.c
 *
 *  Created on: 13 nov. 2018
 *      Author: diego
 */
#include "SAI_Inits.h"


void inicializar_leds(void){
	/*	Funcion inicializar_leds()
	 *	No recive ni devuelve un valor.
	 *	Se inicializan los 4 leds de la placa Discovery STM32F4.
	 */

	GPIO_InitTypeDef GPIO_Init_Pins; // Estrucura de datos para configurar el GPIO

	//
	//Inicializacion de los leds.
	//

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	GPIO_Init_Pins.GPIO_Pin= GPIO_Pin_15 |GPIO_Pin_12 | GPIO_Pin_13;
	GPIO_Init_Pins.GPIO_Mode=GPIO_Mode_OUT ;
	GPIO_Init_Pins.GPIO_Speed= GPIO_Speed_100MHz;
	GPIO_Init_Pins.GPIO_OType= GPIO_OType_PP ;
	GPIO_Init_Pins.GPIO_PuPd= GPIO_PuPd_NOPULL;

	GPIO_Init(GPIOD,&GPIO_Init_Pins); // Carga de la estrucura de datos.

}

void inicializar_fan(void){
	/*	Funcion inicializar_leds()
	 *	No recive ni devuelve un valor.
	 *	Se inicializan los 4 leds de la placa Discovery STM32F4.
	 */

	GPIO_InitTypeDef GPIO_Init_Pins; // Estrucura de datos para configurar el GPIO

	//
	//Inicializacion de los leds.
	//

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	GPIO_Init_Pins.GPIO_Pin= GPIO_Pin_14;
	GPIO_Init_Pins.GPIO_Mode=GPIO_Mode_OUT ;
	GPIO_Init_Pins.GPIO_Speed= GPIO_Speed_100MHz;
	GPIO_Init_Pins.GPIO_OType= GPIO_OType_PP ;
	GPIO_Init_Pins.GPIO_PuPd= GPIO_PuPd_NOPULL;

	GPIO_Init(GPIOD,&GPIO_Init_Pins); // Carga de la estrucura de datos.

}

void inicializar_ventana(void)
{
	GPIO_InitTypeDef GPIOA_Stepper;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIOA_Stepper.GPIO_Mode = GPIO_Mode_OUT;
	GPIOA_Stepper.GPIO_OType = GPIO_OType_PP;
	GPIOA_Stepper.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
	GPIOA_Stepper.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIOA_Stepper.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIOA_Stepper);
}

void inicializar_calentador(void){
	/*	Funcion inicializar_leds()
	 *	No recive ni devuelve un valor.
	 *	Se inicializan los 4 leds de la placa Discovery STM32F4.
	 */

	GPIO_InitTypeDef GPIO_Init_Pins; // Estrucura de datos para configurar el GPIO

	//
	//Inicializacion de los leds.
	//

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	GPIO_Init_Pins.GPIO_Pin= GPIO_Pin_4;
	GPIO_Init_Pins.GPIO_Mode=GPIO_Mode_OUT ;
	GPIO_Init_Pins.GPIO_Speed= GPIO_Speed_100MHz;
	GPIO_Init_Pins.GPIO_OType= GPIO_OType_PP ;
	GPIO_Init_Pins.GPIO_PuPd= GPIO_PuPd_NOPULL;

	GPIO_Init(GPIOC,&GPIO_Init_Pins); // Carga de la estrucura de datos.

}

void inicializar_bomba(void){
	/*	Funcion inicializar_leds()
	 *	No recive ni devuelve un valor.
	 *	Se inicializan los 4 leds de la placa Discovery STM32F4.
	 */

	GPIO_InitTypeDef GPIO_Init_Pins; // Estrucura de datos para configurar el GPIO

	//
	//Inicializacion de los leds.
	//

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	GPIO_Init_Pins.GPIO_Pin= GPIO_Pin_5;
	GPIO_Init_Pins.GPIO_Mode=GPIO_Mode_OUT ;
	GPIO_Init_Pins.GPIO_Speed= GPIO_Speed_100MHz;
	GPIO_Init_Pins.GPIO_OType= GPIO_OType_PP ;
	GPIO_Init_Pins.GPIO_PuPd= GPIO_PuPd_NOPULL;

	GPIO_Init(GPIOC,&GPIO_Init_Pins); // Carga de la estrucura de datos.

}
