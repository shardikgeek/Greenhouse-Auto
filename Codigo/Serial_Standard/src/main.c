/**
 ******************************************************************************
 * @file    main.c
 * @author  Diego Alvarez - TDII
 * @version
 * @date    9-Julio-2018
 * @brief	Ejercicio 1 - PP7
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

void delay(uint32_t tiempo);
void RCC_Configuration(void);
void GPIO_Configuration(void);
void USART2_Configuration(void);

int main(void)
{
	unsigned int contador = 0;
	const unsigned char texto[31] = " TDII & Electrónica lo mejor!!!";

	RCC_Configuration(); 	// Habilitación de los timers.
	GPIO_Configuration();	// Configuración de los pines utilizados para USART.
	USART2_Configuration();	// Configuración del USART.

	while(1)
	{
			while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET); // Verifica si se encuentra un dato disponible.
			{
				while(contador < 31) // Ciclo necesario para imprimir toda la cadena (31 caracteres).
				{
					USART_SendData(USART2,texto[contador]); // Emision del texto a traves de USART.
					while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);  // Espera que el registro de transmision este libre.
					contador++;  	// Permite enviar todos los caracteres del texto.
				}
				delay(10000000);
				contador = 0;		// Vuelve a comenzar el texto.
			}
	}
	while(1);
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
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	/* GPIOB habilitación del timer */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
}

void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/*-------------------------- Configuración GPIO ----------------------------*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Conección de los pines USART al AF */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);
}

void USART2_Configuration(void)
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

	USART_Init(USART2, &USART_InitStructure);

	USART_Cmd(USART2, ENABLE);
}

