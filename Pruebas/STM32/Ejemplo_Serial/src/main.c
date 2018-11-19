/**
 ******************************************************************************
 * @file    main.c
 *
 *		Transmisor serie bajo el protocolo RS232.
 *		El programa emite un mensaje desde el kit hacia la PC, utilizando un
 *		adaptador RS232-USB. El mensaje es "TDII & Electronica lo mejor!!"
 *		Este mensaje se puede visualizar utilizando un software en la PC.
 *
 *		Conexión: Tx PA.2
 *				  Rx PA.3
 *
 ******************************************************************************
 **/

#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_usart.h"

uint8_t contador = 0;
char buffer[10];
uint8_t paquete_listo = 0;

void delay(uint32_t tiempo);
void RCC_Configuration(void);
void GPIO_Configuration(void);
void USART3_Configuration(void);
static void NVIC_Config(void);

int main(void)
{
	RCC_Configuration(); 	// Habilitación de los timers.
	GPIO_Configuration();	// Configuración de los pines utilizados para USART.
	USART3_Configuration();	// Configuración del USART.
	NVIC_Config();          // Configuracion del NVIC.
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	uint8_t i = 0;


	while(1)
	{
/*		if(paquete_listo == 1)
		{
			for(i=0;i<10;i++)
			{
				USART_SendData(USART3,buffer[i]);
				buffer[i] = ' ';
			}
			paquete_listo=0;
		}*/

	}
}


void delay(uint32_t tiempo)
{
	/*
	 * Funcion encargada de generar un retardo.
	 */

	while(--tiempo);
}

void RCC_Configuration(void)
{
	/* --------------------------- System Clocks Configuration -----------------*/
	/* USART3 habilitación del timer */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	/* GPIOB habilitación del timer */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
}

void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/*-------------------------- Configuración GPIO ----------------------------*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	/* Conección de los pines USART al AF */
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_USART3);
}

static void NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Enable the USARTx Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 10;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void USART3_Configuration(void)
{
	USART_InitTypeDef USART_InitStructure;

	/* USARTx configuration ------------------------------------------------------*/
	/* USARTx configurada como sigue:
        - BaudRate = 9600 baud
        - Largo de palabra = 8 Bits
        - Un Bit de stop
        - Sin paridad
        - COntrol de flujo por hardware deshabilitado (RTS and CTS signals)
        - Recepcion y transmision habilitadas
	 */
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;

	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART3, &USART_InitStructure);

	USART_Cmd(USART3, ENABLE);
}

void USART3_IRQHandler(void) {
	//Check if interrupt was because data is received
	if (USART_GetITStatus(USART3, USART_IT_RXNE)) {
		//Do your stuff here

	/*	if(contador<10)
		{
			buffer[contador] = USART_ReceiveData(USART3);
			contador++;
		}

		else
		{
			contador = 0;
			paquete_listo = 1;
		}
*/
		//Clear interrupt flag

		if( USART_ReceiveData(USART3) == 'a')
		{
			USART_SendData(USART3,'-');
		}

		USART_ClearITPendingBit(USART3, USART_IT_RXNE);

	}
}
