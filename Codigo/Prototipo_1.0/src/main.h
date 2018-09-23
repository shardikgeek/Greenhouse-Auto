/*
 * main.h
 *
 *  Created on: 18 sep. 2018
 *      Author: diego
 */

#ifndef MAIN_H_
#define MAIN_H_
#include <stm32f4xx.h>
#include "dht.h"
#include "adc.h"
#include "stm32_ub_lcd_2x16.h" // Inclusion de la libreria para manejar el LCD

// Variables globales.
struct{
	uint32_t contador;
	uint8_t flag;
}led;

struct{
	uint32_t contador;
	uint8_t flag;
	uint8_t estado;
}display;

struct{
	uint16_t contador;
	uint16_t adc_cuentas;
	uint8_t flag;
}ldr;

struct{
	uint16_t contador;
	uint16_t adc_cuentas;
	uint8_t flag;
}yl;

// Prototipos de funciones.
void inicializar_leds(void);
void task_manager(void);


#endif /* MAIN_H_ */
