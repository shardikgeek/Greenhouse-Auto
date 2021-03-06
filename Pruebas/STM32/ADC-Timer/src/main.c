/**
 ******************************************************************************
 * @file    main.c
 *
 * Lectura de Humedad utilizando el ADC (Pin PC1)
 *
 * 5% MAX_ADC < SE�AL < 1/4  MAX_ADC   --> LED VERDE ON
 * 1/4 MAX_ADC < SE�AL < 1/2 MAX_AD   --> LED VERDE y NARANJA ON
 * 1/2 MAX_ADC < SE�AL < 3/4 MAX_ADC   --> LED VERDE,NARANJA Y ROJO ON
 * 3/4 MAX_ADC < SE�AL <     MAX_ADC   --> TODOS LOS LEDS ON
 *
 *
 * Por detalles de funcionamiento ver:
 * - STM32f407 - DM00037051.pdf
 * - Manual de referencia STM32F4XXXX - ARM 32 bits - DM00031020.pdf

 ******************************************************************************
 */

#include "stm32f4xx_gpio.h"
#include <stm32f4xx_adc.h>
#include "adc.h"
#include <stm32f4xx_rcc.h>
#include <stdlib.h>
#include <stdio.h>
#include <stm32_ub_lcd_2x16.h>
#include "alvarez_timer_config.h"

#define MAX_ADC	4095.0

void retardo(volatile uint32_t cuenta);
void PORT_Init(void);
void leer();
int adc_cuentas;

int main(void)
{

	SystemInit();
	adc_inicializar();
	PORT_Init();  //Inicializamos los pines donde estan conectados los LEDs
	UB_LCD_2x16_Init();

	UB_LCD_2x16_Clear();
	UB_LCD_2x16_String(0,0,"Tension:");
	TIM4_Start();

	while (1)
	{

	}
}




void retardo(volatile uint32_t cuenta)
{
	while(cuenta--) {};
}



//Funcion que inicializa los pines 12, 13, 15 y 15 del puerto D, los cuales son los
//pines en donde estan conectados los LEDs

void PORT_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure; //Estructura de configuracion

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);	//Habilitacion de la senal de
	//reloj para el periferico GPIOD

	//Enmascaramos los pines que usaremos

	GPIO_InitStructure.GPIO_Pin =	GPIO_Pin_12 |
			GPIO_Pin_13 |
			GPIO_Pin_14 |
			GPIO_Pin_15;

	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_OUT;		//Los pines seleccionados como salida
	GPIO_InitStructure.GPIO_OType	= GPIO_OType_PP;		//Tipo de salida como push pull
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_100MHz;	//Velocidad del clock para el GPIO
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;		//Sin pull ups

	GPIO_Init(GPIOD, &GPIO_InitStructure); //Se aplica la configuracion definidas anteriormente
}

void leer(){
	char buffer[10];

	adc_cuentas=adc_leer_cuentas();
	GPIO_ResetBits(GPIOD,GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);

	if(adc_cuentas<MAX_ADC/4 && adc_cuentas>0.05 * MAX_ADC)
	{
		GPIO_SetBits(GPIOD,GPIO_Pin_12);
		GPIO_ResetBits(GPIOD,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
	}

	if(adc_cuentas>MAX_ADC/4 && adc_cuentas<MAX_ADC/2)
	{
		GPIO_SetBits(GPIOD,GPIO_Pin_12|GPIO_Pin_13);
		GPIO_ResetBits(GPIOD,GPIO_Pin_14|GPIO_Pin_15);
	}

	if(adc_cuentas>MAX_ADC/2 && adc_cuentas<MAX_ADC*3/4)
	{
		GPIO_SetBits(GPIOD,GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14);
		GPIO_ResetBits(GPIOD,GPIO_Pin_15);
	}
	if(adc_cuentas>MAX_ADC*3/4)
	{
		GPIO_SetBits(GPIOD,GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
	}

	sprintf(buffer,"%d   ",adc_cuentas);
	UB_LCD_2x16_String(9,0,buffer);
}





