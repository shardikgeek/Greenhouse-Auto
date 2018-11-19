/*
 * SAI_Protocolo_Serial.c
 *
 *  Created on: 13 nov. 2018
 *      Author: diego
 */
#include "SAI_Protocolo_Serial.h"
// Codigo para USART.
void enviar_comando(char *cmd){
	/*	Funcion enviar_comando
	 * Envia una cadena a traves del USART3
	 */
	uint16_t n = strlen(cmd);
	uint16_t i;
	for(i=0;i<n;i++){
		USART_SendData(USART3, cmd[i]); // Se envia el dato que ingreso.
		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET); // Se espera a que termine de enviar (NOTAR QUE DICE FLAG).
	}
}

void USART3_IRQHandler(void) {
	/*	Handler USART3_IRQHandler
	 * 	Se encarga de recibir los paquetes a traves del puerto serie.
	 * 	Se asegura que cada paquete tenga su formato correcto. Se esperan caracteres hasta 100ms,
	 * 	despues de este tiempo se toma como un fallo de recepcion.
	 */

	if (USART_GetITStatus(USART3, USART_IT_RXNE)) { // Se limpia la bandera de interrupcion (NOTAR QUE DICE IT)

		serial.aux = USART_ReceiveData(USART3); // Copia el utlimo dato que ingreso.

		if(serial.aux == ':' && !serial.flag.comienzo_paquete){ // Se guarda un paquete si comienza un mensaje con :
			serial.timeout = 100; // Se setea el tiempo maximo de espera.
			serial.contador = 0; // Se reinicia el contador.
			serial.flag.comienzo_paquete = 1; // Se informa el comienzo del paquete.
			serial.flag.fin_paquete = 0; // Se limpia la bandera de fin de paquete.
			strcpy(serial.buffer,""); // Limpia la string.
			strcpy(serial.cant_bytes,""); // Limpia la string.
			strcpy(serial.comando,""); // Limpia la string.
			strcpy(serial.datos,""); // Limpia la string.
		}
		else if((serial.aux == '!' && serial.flag.comienzo_paquete) || serial.contador == 256){ // Se termina paquete con !

			if(!serial.flag.fin_comando || !serial.flag.fin_cantbytes ){
				enviar_comando(":ERR,10,BAD_FORMAT!");
				serial.contador = 0; // Se reinica el contador.
				serial.flag.comienzo_paquete = 0; // Se reinicia la bandera.
				serial.flag.fin_comando = 0; // Se reinicia la bandera.
				serial.flag.fin_cantbytes = 0; // Se reinicia la bandera.
				serial.flag.fin_datos = 0; // Se reinicia la bandera.
				serial.flag.fin_paquete = 0; // Se informa el paquete disponible.
				serial.timeout = 0; // Se detiene el contador de tiempo de espera.
			}
			else{
				strncpy(serial.datos,serial.buffer,256); // Se guardan los datos recibidos.
				strncpy(serial.buffer,"",17); // Se limpia el buffer.
				serial.flag.fin_datos = 1; // Se informa que los datos estan disponibles.
				serial.contador = 0; // Se reinica el contador.
				serial.flag.comienzo_paquete = 0; // Se reinicia la bandera.
				serial.flag.fin_comando = 0; // Se reinicia la bandera.
				serial.flag.fin_cantbytes = 0; // Se reinicia la bandera.
				serial.flag.fin_datos = 0; // Se reinicia la bandera.
				serial.flag.fin_paquete = 1; // Se informa el paquete disponible.
				serial.timeout = 0; // Se detiene el contador de tiempo de espera.
			}
		}
		else if((serial.aux == ',' ) && serial.flag.comienzo_paquete){ // Se detecta el fin de campo.
			serial.timeout = 100; // Se setea el tiempo maximo de espera.
			if(!serial.flag.fin_comando && !serial.flag.fin_cantbytes && !serial.flag.fin_datos){ // Se detecta si es la parte de comando.
				strncpy(serial.comando,serial.buffer,17); // Se guarda el comando.
				strncpy(serial.buffer,"",17); // Se limpia el buffer.
				serial.flag.fin_comando = 1; // Se informa la llegada del comando.
			}
			else if(serial.flag.fin_comando && !serial.flag.fin_cantbytes && !serial.flag.fin_datos){ // Se detecta si es la parte de cantidad de bytes.
				strncpy(serial.cant_bytes,serial.buffer,17); // Se guarda la cantidad de bytes.
				strncpy(serial.buffer,"",17); // Se limpia el buffer.
				serial.flag.fin_cantbytes = 1;  // Se informa la llegada de la cantidad de bytes.
			}
			serial.contador = 0; // Se reinica el contador para guardar el proximo campo.
		}
		else if((serial.aux != ':' && serial.aux != '!' ) && serial.flag.comienzo_paquete){ // Se agrega un caracter al buffer.
			serial.timeout = 100; // Se setea el tiempo maximo de espera.
			serial.buffer[serial.contador] = serial.aux; // Se guarda el caracter.
			serial.contador++;
		}

		//Clear interrupt flag
		USART_ClearITPendingBit(USART3, USART_IT_RXNE); // Se limpia la bandera de interrupcion (NOTAR QUE DICE IT)
	}
}

void USART3_Config(void)
{
	/*	Funcion USART3_Config
	 * 	Configura los pines necesarios para la conexion a traves de RS232.
	 * 	Ademas inicializa las interrupciones para el USART3 en modo recepcion.
	 */
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
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // estaba en 50MHz
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
	USART_InitStructure.USART_BaudRate = 9600;
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
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 10;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStruct);

	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); // Se habilitan las interrupciones cuando se recibe un dato.
}
