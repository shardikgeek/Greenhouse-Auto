/*
 * fan.h
 *
 *  Libreria para controlar un ventilador utilizando modulacion PWM.
 *  A traves de un pulso rectangular generado por el Timer 4.
 *
 */

#ifndef FAN_H_
#define FAN_H_

#include "stm32f4xx_it.h"
#include <stm32f4xx_tim.h>
#include <stm32f4xx_gpio.h>

uint16_t CCR3_Val = 10;

void TIM4_Init(void);
void TIM4_Config();
void TIM4_Start(void);
void encender_ventilador();
void apagar_ventilador();

/* Timer 4 (PWM) Canal 3 */

void TIM4_Init(void){

	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	uint16_t PrescalerValue = 0;

/*	 -----------------------------------------------------------------------
	    TIM4 Configuration: generate 4 PWM signals with  different duty cycles.

	    In this example TIM3 input clock (TIM3CLK) is set to 2 * APB1 clock (PCLK1),
	    since APB1 prescaler is different from 1.
	      TIM3CLK = 2 * PCLK1
	      PCLK1 = HCLK / 4
	      => TIM3CLK = HCLK / 2 = SystemCoreClock /2  --> 84MHz

	    Para obtener el clock del contador TIM3 de 2MHz, ajusto el prescaler con la siguiente ecuación:
	       Prescaler = (TIM3CLK / TIM3 counter clock) - 1
	       Prescaler = ((SystemCoreClock /2) /2 MHz) - 1

	    Para obtener el clock de salida del TIM3 de 1 KHz, el preriodo (ARR) se calcula de la siguiente manera:
	        ARR = (TIM3 counter clock / TIM3 output clock) - 1
	           = 1999

	    TIM4 Channel3 duty cycle = (TIM4_CCR1/ TIM3_ARR)* 100 = 50%

	    Note:
	     SystemCoreClock variable holds HCLK frequency and is defined in system_stm32f4xx.c file.
	     Each time the core clock (HCLK) changes, user had to call SystemCoreClockUpdate()
	     function to update SystemCoreClock variable value. Otherwise, any configuration
	     based on this variable will be incorrect.
	  -----------------------------------------------------------------------
	  */

	/*Compute the prescaler value*/
	PrescalerValue = (uint16_t) ((SystemCoreClock /2) / 2000000) - 1; // 2MHz

	// Time base configuration
	TIM_TimeBaseStructure.TIM_Period = 1999; //1999
	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

	// PWM1 Mode configuration: Channel3
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = CCR3_Val;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	TIM_OC3Init(TIM4, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM4, ENABLE);

	// TIM4 enable counter
	TIM_Cmd(TIM4, ENABLE);
}

void TIM4_Config(){

	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);  // Se habilita el reloj del timer 4.
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); // Se habilita el reloj de GPIOD.

	// GPIOC Configuration: TIM4 CH3 (PD14)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_TIM4);	// Asocia los pines del GPIOD a las salidas del timer 4
}

void TIM4_Start(void){
	 //	TIMX_Start deja el timer X ready to go.

	TIM4_Config(); // Configuracion del timer.
	TIM4_Init(); // Inicializacion del timer.
}

void encender_ventilador()
{
	CCR3_Val = 1999;  // Estado activo del pulso mas largo
	TIM4_Start();     // Inicializacion y configuracion del timer 4 (Ventilador)
}

void apagar_ventilador()
{
	CCR3_Val = 1;     // Estado activo del pulso muy corto
	TIM4_Start();     // Inicializacion y configuracion del timer 4 (Ventilador)
}

#endif /* FAN_H_ */
