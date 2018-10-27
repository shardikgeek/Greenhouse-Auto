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
static void USART3_Config(void);

int main(void)
{
	unsigned int contador = 0;
	const unsigned char texto[31] = " TDII & Electrónica lo mejor!!!";
//	SystemInit();
	USART3_Config();

	while(1)
	{
		/* Send dummy data */
		USART_SendData(USART3, 'c');
		/* Loop until the end of transmission */
		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
		{}

		delay(10e6);
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
}
