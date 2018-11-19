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

uint16_t aux = 0;

void delay(uint32_t tiempo);
static void USART3_Config(void);

int main(void)
{
//	unsigned int contador = 0;
//	uint16_t aux = 0;
	SystemInit();
	USART3_Config();

	while(1)
	{
//		if(USART_GetFlagStatus(USART3, USART_FLAG_RXNE)){ // Consulta el registro de recepcion.
//			USART_ClearFlag(USART3,USART_FLAG_RXNE); // Limpia la bandera de recepcion.
//
//			aux = USART_ReceiveData(USART3); // Copia el utlimo dato que ingreso.
//
//			USART_SendData(USART3, aux); // Se envia el dato que ingreso.
//
//			while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET); // Se espera a que termine de transmitirse el dato.
//
//		}

		//delay(10e6);
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

/**
 * @brief Configures the USART3 Peripheral.
 * @param None
 * @retval None
 */
static void USART3_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	/* USART IOs configuration ***********************************/
	/* Enable GPIOC clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	/* Connect PC10 to USART3_Tx */
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_USART3);
	/* Connect PC11 to USART3_Rx*/
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_USART3);
	/* Configure USART3_Tx and USART3_Rx as alternate function */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	/* USART configuration ***************************************/
	/* USART3 configured as follow:
- BaudRate = 115200 baud
- Word Length = 8 Bits
- One Stop Bit
- No parity
- Hardware flow control disabled (RTS and CTS signals)
- Receive and transmit enabled
	 */
	/* Enable USART3 clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3, &USART_InitStructure);
	/* Enable USART3 */
	USART_Cmd(USART3, ENABLE);

	//USART1 is more important than USART2, so it has lower sub priority number
	NVIC_InitTypeDef NVIC_InitStruct;

	NVIC_InitStruct.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 10;
	NVIC_Init(&NVIC_InitStruct);

	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); // Se habilitan las interrupciones cuando se recibe un dato.
}

void USART3_IRQHandler(void) {
    //Check if interrupt was because data is received
    if (USART_GetITStatus(USART3, USART_IT_RXNE)) { // Se limpia la bandera de interrupcion (NOTAR QUE DICE IT)
        //Do your stuff here
    	aux = USART_ReceiveData(USART3); // Copia el utlimo dato que ingreso.

    	USART_SendData(USART3, aux); // Se envia el dato que ingreso.

    	while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET); // Se espera a que termine de enviar (NOTAR QUE DICE FLAG).
        //Clear interrupt flag
        USART_ClearITPendingBit(USART3, USART_IT_RXNE); // Se limpia la bandera de interrupcion (NOTAR QUE DICE IT)
    }
}
