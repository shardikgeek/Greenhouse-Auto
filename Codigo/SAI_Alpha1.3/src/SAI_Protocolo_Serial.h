/*
 * SAI_Protocolo_Serial.h
 *
 *  Created on: 13 nov. 2018
 *      Author: diego
 */

#ifndef SAI_PROTOCOLO_SERIAL_H_
#define SAI_PROTOCOLO_SERIAL_H_
#include <stm32f4xx.h>
#include <string.h>
// Prototipos
void USART3_Config(void);
void enviar_comando(char *cmd);

//Variables externas
struct{
	uint16_t aux;
	uint16_t timeout;
	uint8_t contador;
	uint16_t contador_task;
	char buffer[256];
	char comando[17];
	char cant_bytes[17];
	char datos[256];
	struct {
		unsigned int fin_comando : 1;
		unsigned int fin_cantbytes : 1;
		unsigned int fin_datos : 1;
		unsigned int fin_contador : 1;
		unsigned int comienzo_paquete : 1;
		unsigned int fin_paquete : 1;
		unsigned int mostrar_valores : 1;
	}flag;
}serial;

#endif /* SAI_PROTOCOLO_SERIAL_H_ */
