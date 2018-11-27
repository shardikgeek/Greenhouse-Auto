/*
 * ventana.h
 *
 *  Libreria para controlar un motor paso a paso. Se utiliza para abrir y cerrar una ventana.
 *  Se necesita de un delay entre los pasos del motor, por lo tanto se utiliza un timer (Timer 3).
 *
 */

#ifndef VENTANA_H_
#define VENTANA_H_
#define N_VUELTAS 6700
#define IN1 GPIO_Pin_10
#define IN2 GPIO_Pin_13
#define IN3 GPIO_Pin_14
#define IN4 GPIO_Pin_15

#include "stm32f4xx_it.h"
#include <stm32f4xx_tim.h>
#include <stm32f4xx_gpio.h>

uint16_t sentido = 2; // 1: Abir
// 0: Cerrar
// 2: Apagado
uint16_t step = 0;
uint16_t vuelta = 0;


void TIM3_Init(void);
void TIM3_Config(void);
void TIM3_Start(void);
void TIM3_IRQHandler(void);
void abrir_ventana();
void cerrar_ventana();
void parar_ventana();


/* Timer 3 Canal 1 */

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
	TIM_TimeBaseStructure.TIM_Period = 0.004e3; // 1seg (40e3 = 1 seg --> 0.04e3 = 1 mseg) de periodo.  ----> Frecuencia = 1 kHz
	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	TIM_ITConfig(TIM3, TIM_IT_CC1, ENABLE);	// habilitacion de las interrupciones por el timer 4.
	TIM_Cmd(TIM3, ENABLE); // Habilita el contador para el timer 4.


}

void TIM3_Config(void){
	/*
	 *	TIMX_Config habilita el relog y las interrupciones globales para el timer X
	 */

	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); // Se habilita el reloj.

	/* Se habilitan las interrupciones globales para el timer X*/
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
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

void TIM3_IRQHandler (void)  {
	/* TIM3_IRQHandler rutina de interrupcion del timer 3.
	 *  Realiza un toogle en el led rojo de la placa discovery.
	 */

	if (TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET)
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC1); // Se limpia la bandera de interrupcion.

		if(sentido == 1) //Abrir Ventana (Antihorario)
		{
			if(step == 0){
				GPIO_Write(GPIOA,IN1);
			}
			else if(step == 1){
				GPIO_Write(GPIOA,IN1| IN2);
			}
			else if(step == 2){
				GPIO_Write(GPIOA,IN2);
			}
			else if(step == 3){
				GPIO_Write(GPIOA,IN3|IN2);
			}
			else if(step == 4){
				GPIO_Write(GPIOA,IN3);
			}
			else if(step == 5){
				GPIO_Write(GPIOA,IN3|IN4);
			}
			else if(step == 6){
				GPIO_Write(GPIOA,IN4);
			}
			else if(step == 7){
				GPIO_Write(GPIOA,IN4|IN1);
			}
			else if(step > 7){
				step = 0;
			}
			vuelta++;
		}

		else if(sentido == 0) //Cerrar Ventana (Antihorario)
		{
			if(step == 7){
				GPIO_Write(GPIOA,IN1);
			}
			else if(step == 6){
				GPIO_Write(GPIOA,IN1| IN2);
			}
			else if(step == 5){
				GPIO_Write(GPIOA,IN2);
			}
			else if(step == 4){
				GPIO_Write(GPIOA,IN3|IN2);
			}
			else if(step == 3){
				GPIO_Write(GPIOA,IN3);
			}
			else if(step == 2){
				GPIO_Write(GPIOA,IN3|IN4);
			}
			else if(step == 1){
				GPIO_Write(GPIOA,IN4);
			}
			else if(step == 0){
				GPIO_Write(GPIOA,IN4|IN1);
			}
			else if(step > 7){
				step = 0;
			}
			vuelta++;
		}

		else if(sentido == 2) //Para ventana
		{
			GPIO_ResetBits(GPIOA,IN1|IN2|IN3|IN4);
			vuelta = 0;
			step = 0;
		}

		step ++;
	}
}


void abrir_ventana()
{
	sentido = 1; //Abrir ventana
}

void cerrar_ventana()
{
	sentido = 0; //Cerrar ventana
}

void parar_ventana()
{
	sentido = 2;
	vuelta = 0;
}


#endif /* VENTANA_H_ */
