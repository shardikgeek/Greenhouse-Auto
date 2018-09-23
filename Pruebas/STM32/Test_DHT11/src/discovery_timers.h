/*	
 *	Libreria para inicializar los timers.
 *	Los interrput handlers que se encuentran en el archivo stm32f4xx_it.c
 *	deben borrarse.
 *
 */

/* Includes ------------------------------------------------------------------*/
//#include "stm32f4xx_it.h"
#include "dht.h"
//#include <stm32f4xx_tim.h>
//#include <stm32f4xx_gpio.h>
#define L_VERDE	GPIO_Pin_12
#define L_NARAN	GPIO_Pin_13
#define L_ROJO	GPIO_Pin_14
#define L_AZUL	GPIO_Pin_15

/////////////////////////// TIMER 3/////////////////////////////
void TIM3_Init(void){

	/* 
	 *	TIMX_Init Configura los parametros del timer X.
	 *	Formulas:
	 *	TIM3CLK = 84MHz. Reloj del timer, para modiicarlo hay que cambiar el archivo system_stm32f4xx.c .
	 *	frecuencia. Frecuencia a la que cuenta el timer, este valor modifica el prescaler.
	 *	TIM_Period = (Tei*TIM3CLK)/(Prescalervalue+1). Tei es el valor de periodo de interrupcion deseado en segundos.
	 *  
	 *	ATENCION: TIM_Period y PrescalerValue no deben superar el valor de 0xFFFF (65536) ya que son de 16bits.
	 */

	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	uint16_t PrescalerValue = 0; // Variable para el prescaler.
	uint16_t frecuencia = 4e3; // Frecuencia del contador a 4kHz. Tener cuidado de no cometer overflow en la variable PrescalerValue.
	PrescalerValue = (uint16_t) ((SystemCoreClock /2) / frecuencia) - 1; //Conversion de frecuencia al valor de Prescaler.
	TIM_TimeBaseStructure.TIM_Period = 8e3; // 10seg (4e3 = 1 seg --> 40e3 = 10 seg) de periodo.
	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	TIM_ITConfig(TIM3, TIM_IT_CC1, ENABLE); // habilitacion de las interrupciones por el timer 3.
	TIM_Cmd(TIM3, ENABLE); // Habilita el contador para el timer 3.

}

void TIM3_Config(void){
	/* 
	 *	TIMX_Config habilita el relog y las interrupciones globales para el timer X
	 */

	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); // Se habilita el reloj.

	/* Se habilitan las interrupciones globales para el timer X*/
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}

void TIM3_Start(void){
	/* 
	 *	TIMX_Start deja el timer X ready to go.
	 */

	TIM3_Config(); // Configuracion del timer.
	TIM3_Init(); // Inicializacion del timer.
}
////////////////////////////////////////////////////////////////

/////////////////////////// TIMER 4/////////////////////////////
void TIM4_Init(void){
	/* 
	 *	TIMX_Init Configura los parametros del timer X.
	 *	Formulas:
	 *	TIM3CLK = 84MHz. Reloj del timer, para modiicarlo hay que cambiar el archivo system_stm32f4xx.c .
	 *	frecuencia. Frecuencia a la que cuenta el timer, este valor modifica el prescaler.
	 *	TIM_Period = (Tei*TIM3CLK)/(Prescalervalue+1). Tei es el valor de periodo de interrupcion deseado en segundos.
	 *  
	 *	ATENCION: TIM_Period y PrescalerValue no deben superar el valor de 0xFFFF (65536) ya que son de 16bits.
	 */

	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	uint16_t PrescalerValue = 0; // Variable para el prescaler.
	uint16_t frecuencia = 4e3; // Frecuencia del contador a 4kHz. Tener cuidado de no cometer overflow en la variable PrescalerValue.
	PrescalerValue = (uint16_t) ((SystemCoreClock /2) / frecuencia) - 1; //Conversion de frecuencia al valor de Prescaler.
	TIM_TimeBaseStructure.TIM_Period = 20e3; // 1seg (4e3 = 1 seg --> 20e3 = 5 seg) de periodo.
	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	TIM_ITConfig(TIM4, TIM_IT_CC1, ENABLE);	// habilitacion de las interrupciones por el timer 4.
	TIM_Cmd(TIM4, ENABLE); // Habilita el contador para el timer 4.

}

void TIM4_Config(void){
	/* 
	 *	TIMX_Config habilita el relog y las interrupciones globales para el timer X
	 */

	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); // Se habilita el reloj.

	/* Se habilitan las interrupciones globales para el timer X*/
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}

void TIM4_Start(void){
	/* 
	 *	TIMX_Start deja el timer X ready to go.
	 */

	TIM4_Config(); // Configuracion del timer.
	TIM4_Init(); // Inicializacion del timer.
}
//////////////////////////////////////////////////////////////

/////////////////////////// TIMER 5/////////////////////////////
void TIM5_Init(void){
	/* 
	 *	TIMX_Init Configura los parametros del timer X.
	 *	Formulas:
	 *	TIM3CLK = 84MHz. Reloj del timer, para modiicarlo hay que cambiar el archivo system_stm32f4xx.c .
	 *	frecuencia. Frecuencia a la que cuenta el timer, este valor modifica el prescaler.
	 *	TIM_Period = (Tei*TIM3CLK)/(Prescalervalue+1). Tei es el valor de periodo de interrupcion deseado en segundos.
	 *  
	 *	ATENCION: TIM_Period y PrescalerValue no deben superar el valor de 0xFFFF (65536) ya que son de 16bits.
	 */

	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	uint16_t PrescalerValue = 0; // Variable para el prescaler.
	uint32_t frecuencia = 10e6; // Frecuencia del contador a 10kHz. Tener cuidado de no cometer overflow en la variable PrescalerValue.
	PrescalerValue = (uint16_t) ((SystemCoreClock /2) / frecuencia) - 1; //Conversion de frecuencia al valor de Prescaler.
	TIM_TimeBaseStructure.TIM_Period = 10; // 321.5uS (10e3 = 1 seg --> 3.215 = 321.5uS) de periodo.
	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
	TIM_ITConfig(TIM5, TIM_IT_CC1, ENABLE); // habilitacion de las interrupciones por el timer 5.
	TIM_Cmd(TIM5, ENABLE); // Habilita el contador para el timer 5.

}

void TIM5_Config(void){
	/* 
	 *	TIMX_Config habilita el relog y las interrupciones globales para el timer X
	 */

	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE); // Se habilita el reloj.

	/* Se habilitan las interrupciones globales para el timer X*/
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 10;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}

void TIM5_Start(void){
	/* 
	 *	TIMX_Start deja el timer X ready to go.
	 */

	TIM5_Config(); // Configuracion del timer.
	TIM5_Init(); // Inicializacion del timer.
}
////////////////////////////////////////////////////////////////

/////////////////////////// TIMER 2/////////////////////////////
void TIM2_Init(void){
	/*
	 *	TIMX_Init Configura los parametros del timer X.
	 *	Formulas:
	 *	TIM3CLK = 84MHz. Reloj del timer, para modiicarlo hay que cambiar el archivo system_stm32f4xx.c .
	 *	frecuencia. Frecuencia a la que cuenta el timer, este valor modifica el prescaler.
	 *	TIM_Period = (Tei*TIM3CLK)/(Prescalervalue+1). Tei es el valor de periodo de interrupcion deseado en segundos.
	 *  
	 *	ATENCION: TIM_Period y PrescalerValue no deben superar el valor de 0xFFFF (65536) ya que son de 16bits.
	 */

	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	uint16_t PrescalerValue = 0; // Variable para el prescaler.
	uint16_t frecuencia = 10e3; // Frecuencia del contador a 10kHz. Tener cuidado de no cometer overflow en la variable PrescalerValue.
	PrescalerValue = (uint16_t) ((SystemCoreClock /2) / frecuencia) - 1; //Conversion de frecuencia al valor de Prescaler.
	TIM_TimeBaseStructure.TIM_Period = 9e2; // // 90mS (10e3 = 1 seg --> 9e2 = 90 mseg) de periodo.
	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE); // habilitacion de las interrupciones por el timer 2.
	TIM_Cmd(TIM2, ENABLE); // Habilita el contador para el timer 2.

}

void TIM2_Config(void){
	/* 
	 *	TIMX_Config habilita el relog y las interrupciones globales para el timer X
	 */

	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); // Se habilita el reloj.

	/* Se habilitan las interrupciones globales para el timer X*/
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}

void TIM2_Start(void){
	/* 
	 *	TIMX_Start deja el timer X ready to go.
	 */

	TIM2_Config(); // Configuracion del timer.
	TIM2_Init(); // Inicializacion del timer.
}
////////////////////////////////////////////////////////////////

/////////// Handlers de interrupciones/////////////////////////
void TIM3_IRQHandler (void)  {
	/* TIM3_IRQHandler rutina de interrupcion del timer 3.
	 *  Realiza un toogle en el led verde de la placa discovery.
	 */

	if (TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET)
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC1); // Se limpia la bandera de interrupcion



	}


void TIM4_IRQHandler (void)  {
	/* TIM4_IRQHandler rutina de interrupcion del timer 4.
	 *  Realiza un toogle en el led rojo de la placa discovery.
	 */

	if (TIM_GetITStatus(TIM4, TIM_IT_CC1) != RESET)
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_CC1); // Se limpia la bandera de interrupci


	}


}

void TIM5_IRQHandler (void)  {
	/* TIM5_IRQHandler rutina de interrupcion del timer 5.
	 *  Realiza un toogle en el led azul de la placa discovery.
	 */

	if (TIM_GetITStatus(TIM5, TIM_IT_CC1) != RESET)
	{
		TIM_ClearITPendingBit(TIM5, TIM_IT_CC1); // Se limpia la bandera de interrupcion.

		if (TimingDelay != 0)
		{
			TimingDelay--;
		}


	}


}

void TIM2_IRQHandler (void)  {
	/* TIM2_IRQHandler rutina de interrupcion del timer 2.
	 *  Realiza un toogle en el led naranja de la placa discovery.
	 */

	if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC1); // Se limpia la bandera de interrupcion.

		GPIO_ToggleBits(GPIOD, L_NARAN); // Se cambia el estado del led naranja.


	}


}
