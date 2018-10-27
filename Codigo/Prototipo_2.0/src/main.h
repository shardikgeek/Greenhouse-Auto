/*
 * main.h
 *
 *  Created on: 18 sep. 2018
 *      Author: diego
 */

#ifndef MAIN_H_
#define MAIN_H_
#define NUMERO_ETAPAS 5

//


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
	// Esta estructura es la la que contiene los valores de control del sistema.
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

typedef struct{
	uint8_t limite_delta_temp; // Limite maximo entre la temperatura exterior e interior.
	uint8_t max_temp_fan; // Limite maximo para prender el ventilador.
	uint8_t min_temp_fan; // Limite minimo para apagar el ventilador.
	uint8_t min_temp_calentador; // Temperatura minima para prender el ventilador.
	uint8_t max_temp_calentador; // Temeperatura maxima para apagar el ventilador.
}temperaturas;

struct{
	// Esta estructura contiene los datos del cultivo

	TM_RTC_t etapa[NUMERO_ETAPAS]; // Fechas de las distintas etapas.
	uint8_t etapa_actual;
	uint16_t contador; // Contador para disparar eventos.
	uint16_t contador_aux; // Contador para chequear cultivo.
	char nombre[16];
	temperaturas variables[NUMERO_ETAPAS];
	struct{
		uint8_t limite_delta_temp; // Limite maximo entre la temperatura exterior e interior.
		uint8_t max_temp_fan; // Limite maximo para prender el ventilador.
		uint8_t min_temp_fan; // Limite minimo para apagar el ventilador.
		uint8_t min_temp_calentador; // Temperatura minima para prender el ventilador.
		uint8_t max_temp_calentador; // Temeperatura maxima para apagar el ventilador.
	}control;
	struct {
		unsigned int control_activo : 1;
		unsigned int fin_contador : 1;
	}flag;
}cultivo;

struct{
	uint16_t aux;
	uint16_t timeout;
	uint8_t contador;
	char buffer[17];
	char comando[17];
	char cant_bytes[17];
	char datos[17];
	struct {
		unsigned int fin_comando : 1;
		unsigned int fin_cantbytes : 1;
		unsigned int fin_datos : 1;
		unsigned int fin_contador : 1;
		unsigned int comienzo_paquete : 1;
		unsigned int fin_paquete : 1;
	}flag;
}serial;

struct{
	/*	struct sistema
	 *  Estructura que contiene los estados mas generales del sistema.
	 */
	struct {
		unsigned int conexion_serial : 1;
	}flag;
}sistema;

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
void serial_task(void);
void cargar_datos(void);
void procesar_paquete(void);
int fecha_valida(TM_RTC_t fecha_control);
void configurar_cultivo_tomate(void);
void configurar_cultivo_morron(void);
void configurar_cultivo_zanahoria(void);
static void USART3_Config(void);
void enviar_comando(char *cmd);


#endif /* MAIN_H_ */
