/*
 * main.h
 *
 *  Created on: 18 sep. 2018
 *      Author: diego
 */

#ifndef MAIN_H_
#define MAIN_H_
#define NUMERO_ETAPAS 5
#include <stm32f4xx.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dht.h"
#include "adc.h"
#include "stm32_ub_lcd_2x16.h" // Inclusion de la libreria para manejar el LCD
#include <rtc.h>
#include "stm32f4xx_usart.h"


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

struct{
	TM_RTC_t etapa[NUMERO_ETAPAS];
	uint8_t etapa_actual;
	uint16_t contador;
	char nombre[16];
	struct{
		uint8_t limite_delta_temp;
		uint8_t max_temp_fan;
		uint8_t min_temp_fan;
		uint8_t min_temp_calentador;
		uint8_t max_temp_calentador;
	}control;
	struct {
		unsigned int control_activo : 1;
	}flag;
}cultivo;

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
void control_temp_task(void);
void check_cultivo_task(void);
void cargar_datos(void);
void RCC_Configuration(void);
void GPIO_Configuration(void);
void USART3_Configuration(void);
static void NVIC_Config(void);


#endif /* MAIN_H_ */
