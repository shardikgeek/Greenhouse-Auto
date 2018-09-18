/*
 * dht.h
 *	Esta libreria utiliza el TIMER 5 para realizar un peque�o delay.
 *
 *  Created on: 11 ago. 2018
 *      Author: diego
 */
// Pin 1 GPIOA DHT11
#ifndef DHT_H_
#define DHT_H_
#include <stm32f4xx.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TIME_TIMEOUT 10

#define STATE_DHT_TIMEOUT 4
#define STATE_DHT_DESCONECTADO 1
#define STATE_DHT_CHECKSUM_GOOD 2
#define STATE_DHT_CHECKSUM_BAD 3

uint32_t TimingDelay = 0;

struct{
	uint32_t contador;
	uint32_t contador_desconectado;
	uint8_t flag;
	uint8_t check;
	uint32_t timeout;
	uint8_t flag_timeout;
	uint8_t estado;
	char temperatura_string[15];
	double temperatura;
	double humedad;
}dht;

void DHT11_start(void);
void check_response(void);
uint8_t read_data (void);
void delay(uint32_t tiempo);
void leer_dht();
void TIM5_Start(void);

void delay(uint32_t tiempo){
	/* Funcion delay()
	 * Realiza un retardo en funcion de la frecuencia de reloj del microcontrolador.
	 * Recibe como parametro el retraso, para este caso, en mS debido a la configuracion del Systick.
	 */

	TimingDelay = tiempo;
	while(TimingDelay!=0);

}


void set_gpio_output (void)
{
	GPIO_InitTypeDef GPIO_Init_Pins; // Estrucura de datos para configurar el GPIO

	//
	//Inicializacion del pin.
	//

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_Init_Pins.GPIO_Pin= GPIO_Pin_1;
	GPIO_Init_Pins.GPIO_Mode=GPIO_Mode_OUT ;
	GPIO_Init_Pins.GPIO_Speed= GPIO_Speed_100MHz;
	GPIO_Init_Pins.GPIO_OType= GPIO_OType_PP ;
	GPIO_Init_Pins.GPIO_PuPd= GPIO_PuPd_NOPULL;

	GPIO_Init(GPIOA,&GPIO_Init_Pins); // Carga de la estrucura de datos.
}

void set_gpio_input (void)
{
	GPIO_InitTypeDef GPIO_Init_Pins; // Estrucura de datos para configurar el GPIO

	//
	//Inicializacion del pin.
	//

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_Init_Pins.GPIO_Pin= GPIO_Pin_1;
	GPIO_Init_Pins.GPIO_Mode=GPIO_Mode_IN;
	GPIO_Init_Pins.GPIO_Speed= GPIO_Speed_100MHz;
	GPIO_Init_Pins.GPIO_OType= GPIO_OType_PP;
	GPIO_Init_Pins.GPIO_PuPd= GPIO_PuPd_NOPULL;

	GPIO_Init(GPIOA,&GPIO_Init_Pins); // Carga de la estrucura de datos.
}

void DHT11_start (void)
{
	set_gpio_output ();  // set the pin as output
	GPIO_WriteBit(GPIOA,GPIO_Pin_1,0); // pull the pin low
	delay(18000);   // wait for 18ms
	set_gpio_input ();   // set as input
}

void check_response (void)
{
	delay(40);
	if (!(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)))
	{
		delay(80);
		if ((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1))) dht.check = 1;
	}

	dht.timeout = TIME_TIMEOUT;
	while (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1) && !dht.flag_timeout);   // wait for the pin to go low
}

uint8_t read_data (void)
{
	uint8_t i,j;
	for (j=0;j<8;j++)
	{
		dht.timeout = TIME_TIMEOUT;
		while (!(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)) && !dht.flag_timeout);   // wait for the pin to go high
		if(dht.flag_timeout)  //Si se vencio el timeout vuelve al main.
					return 0;

		delay(40);   // wait for 40 us

		if ((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)) == 0)   // if the pin is low
		{
			i&= ~(1<<(7-j));   // write 0
		}
		else i|= (1<<(7-j));  // if the pin is high, write 1

		dht.timeout = TIME_TIMEOUT; // set the timeout
		while ((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)) && !dht.flag_timeout);  // wait for the pin to go low or the timeout dead.
		if(dht.flag_timeout)  //Si se vencio el timeout vuelve al main.
					return 0;
	}
	return i;
}

void leer_dht(){

	uint8_t Rh_byte1, Rh_byte2, Temp_byte1, Temp_byte2;
	uint16_t sum;
	char aux[5] = "";

	TIM_Cmd(TIM5, ENABLE); // Habilita el timer 5 para la lectura.
	DHT11_start ();


	check_response ();
	//	GPIO_ToggleBits(GPIOD,GPIO_Pin_12);
	if(dht.flag_timeout){ //Si se vencio el timeout vuelve al main.
		dht.estado = STATE_DHT_TIMEOUT;
		TIM_Cmd(TIM5, DISABLE);
		return;
	}

	Rh_byte1 = read_data ();
	if(dht.flag_timeout){ //Si se vencio el timeout vuelve al main.
		dht.estado = STATE_DHT_TIMEOUT;
		TIM_Cmd(TIM5, DISABLE);
		return;
	}

	Rh_byte2 = read_data ();
	if(dht.flag_timeout){ //Si se vencio el timeout vuelve al main.
		dht.estado = STATE_DHT_TIMEOUT;
		TIM_Cmd(TIM5, DISABLE);
		return;
	}

	Temp_byte1 = read_data ();
	if(dht.flag_timeout){ //Si se vencio el timeout vuelve al main.
		dht.estado = STATE_DHT_TIMEOUT;
		TIM_Cmd(TIM5, DISABLE);
		return;
	}

	Temp_byte2 = read_data ();
	if(dht.flag_timeout){ //Si se vencio el timeout vuelve al main.
		dht.estado = STATE_DHT_TIMEOUT;
		TIM_Cmd(TIM5, DISABLE);
		return;
	}

	sum = read_data();
	if(dht.flag_timeout){ //Si se vencio el timeout vuelve al main.
		dht.estado = STATE_DHT_TIMEOUT;
		TIM_Cmd(TIM5, DISABLE);
		return;
	}

	if (sum == (Rh_byte1+Rh_byte2+Temp_byte1+Temp_byte2))    // if the data is correct
	{
		sprintf(aux,"%d.%d",Temp_byte1,Temp_byte2); // Se crea una cadena con la temperatura.
		strcpy(dht.temperatura_string,aux); // Se guarda la temperatura en la estructura del dht.
		dht.estado = STATE_DHT_CHECKSUM_GOOD; // Se cambia el estado del sensor.
	}
	else
		dht.estado = STATE_DHT_CHECKSUM_BAD; // Se cambia el estado del sensor.

	TIM_Cmd(TIM5, DISABLE);

}

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
	TIM_Cmd(TIM5, DISABLE);
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

#endif /* DHT_H_ */
