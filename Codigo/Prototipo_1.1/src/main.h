/*
 * main.h
 *
 *  Created on: 18 sep. 2018
 *      Author: diego
 */

#ifndef MAIN_H_
#define MAIN_H_
#include <stm32f4xx.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dht.h"
#include "adc.h"
#include "stm32_ub_lcd_2x16.h" // Inclusion de la libreria para manejar el LCD
#include <rtc.h>


// Variables globales.
struct{
	uint32_t contador;
	struct {
		unsigned int fin_contador : 1;
		unsigned int otra : 1;
		unsigned int wacho : 1;
	}flag;
}led;

struct{
	uint32_t contador;
	uint8_t flag;
	uint8_t estado;
}display;

struct{
	uint16_t contador;
	uint16_t contador_promedio;
	uint16_t adc_cuentas;
	uint16_t adc_cuentas_temp;
	struct {
		unsigned int fin_contador : 1;
		unsigned int fin_promedio : 1;
		unsigned int wacho : 1;
	}flag;
}ldr;

struct{
	uint16_t contador;
	uint16_t contador_promedio;
	uint16_t adc_cuentas;
	uint8_t flag;
	uint8_t flag_promedio;
}yl;

struct{
	uint8_t hora;
	uint8_t minutos;
	uint8_t segundos;
	uint16_t contador;
	struct {
		unsigned int fin_contador : 1;
		unsigned int fin_promedio : 1;
		unsigned int wacho : 1;
	}flag;
}tiempo;

struct{
	uint8_t limite_delta_temp;
	uint8_t max_temp_fan;
	uint8_t min_temp_fan;
	uint8_t min_temp_calentador;
	uint8_t max_temp_calentador;
	struct {
			unsigned int bomba_encendida : 1;
			unsigned int calentador_encendido : 1;
			unsigned int fan_encendido : 1;
		}flag;
}control;

TM_RTC_AlarmTime_t AlarmTime;
TM_RTC_t datatime;

// Prototipos de funciones.
void inicializar_leds(void);
void task_manager(void);
void dht_interior_task(void);
void dht_exterior_task(void);
void led_task(void);
void ldr_task(void);
void yl69_task(void);
void display_task(void);
void tiempo_task(void);
void control_temp_task(void);
void cargar_datos(void);


#endif /* MAIN_H_ */
