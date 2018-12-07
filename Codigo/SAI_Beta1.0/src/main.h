/*
 * main.h
 *
 *  Created on: 18 sep. 2018
 *      Author: diego
 */

/*	PINES UTILIZADOS
 * 	PC1 -> YL-69 (ADC - Channel) (andando)
 * 	PC2 -> LDR (ADC - Channel) (andando)
 * 	PC4 -> Calentador (DigitalOutput)
 * 	PC5 -> Bomba (DigitalOutput)
 * 	PE7 -> Temp Interior (DigitalInput) (andando)
 * 	PE9 -> Temp Exterior (DigitalInput) (andando)
 * 	PA5 -> Buzzer (DAC)
 * 	PD8 -> TX (USART3) (andando)
 * 	PD9 -> RX (USART3) (andando)
 * 	PD11 -> CTS (USART3) (andando)
 * 	PD12 -> RTS (USART3) (andando)
 * 	PD14 -> Motor (DAC-PWM - Channel)
 */

/* TIMERS UTILZIADOS
 * Systick -> Sistema.
 * Timer 5 -> DHT.
 * Timer 3 -> Ventana (Paso a paso).
 */

#ifndef MAIN_H_
#define MAIN_H_
#define NUMERO_ETAPAS 3
#define MIN_T_ETAPA_0 1
#define MIN_T_ETAPA_1 1
#define MIN_T_ETAPA_2 1
#define MIN_Z_ETAPA_0 2
#define MIN_Z_ETAPA_1 2
#define MIN_Z_ETAPA_2 3
#define VOLT_REF	2950 //Voltaje maximo de tension (3V)

#include <stm32f4xx.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dht.h"
#include "adc.h"
#include "stm32_ub_lcd_2x16.h" // Inclusion de la libreria para manejar el LCD
#include <rtc.h>
#include "stm32_ub_fatfs.h" // Libreria de fatfs.
#include "ventana.h"
#include "fan.h"
#include "fsm.h"

struct{
	uint8_t contador_menu;
	uint8_t contador_down;
	uint8_t contador_back;
	uint8_t contador_up;
	uint8_t contador_enter;
	struct {
		unsigned int fin_menu : 1;
		unsigned int fin_back : 1;
		unsigned int fin_up : 1;
		unsigned int fin_down : 1;
		unsigned int fin_enter : 1;
	}flag;
}botones;


// Variables globales.
struct{
	uint32_t contador;
	struct {
		unsigned int fin_contador : 1;
		unsigned int otra : 1;
	}flag;
}led;

struct{
	uint16_t contador;
	uint16_t contador_promedio;
	uint16_t adc_cuentas;
	uint16_t adc_cuentas_temp;
	struct {
		unsigned int fin_contador : 1;
		unsigned int fin_promedio : 1;
	}flag;
}ldr;

struct{
	uint16_t contador;
	uint16_t contador_promedio;
	uint16_t adc_cuentas;
	uint8_t flag;
	uint8_t flag_promedio;
	uint8_t humedad;
}yl;

struct{
	// Esta estructura es la la que contiene los valores de control del sistema.
	uint8_t limite_delta_temp;
	uint8_t max_temp_fan;
	uint8_t min_temp_fan;
	uint8_t min_temp_calentador;
	uint8_t max_temp_calentador;
	uint8_t min_hum;
	uint8_t max_hum;
	struct {
			unsigned int bomba_encendida : 1;
			unsigned int calentador_encendido : 1;
			unsigned int fan_encendido : 1;
			unsigned int ventana_abierta : 1;
			unsigned int agua_tanque : 1;
		}flag;
}control;

typedef struct{
	uint8_t limite_delta_temp; // Limite maximo entre la temperatura exterior e interior.
	uint8_t max_temp_fan; // Limite maximo para prender el ventilador.
	uint8_t min_temp_fan; // Limite minimo para apagar el ventilador.
	uint8_t min_temp_calentador; // Temperatura minima para prender el ventilador.
	uint8_t max_temp_calentador; // Temeperatura maxima para apagar el ventilador.
}temperaturas;

typedef struct{
	uint8_t min_hum; // Limite maximo entre la temperatura exterior e interior.
	uint8_t max_hum; // Limite maximo para prender el ventilador.
}humedad;

struct{
	// Esta estructura contiene los datos del cultivo

	TM_RTC_t etapa[NUMERO_ETAPAS];
	uint8_t etapa_actual;
	uint8_t tipo;
	uint16_t contador; // Contador para disparar eventos.
	uint16_t contador_aux; // Contador para chequear cultivo.
	char nombre[16]; // (Se puede eliminar despues y usar tipo).
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
		unsigned int cambio_etapa : 1;
		unsigned int pause : 1;
	}flag;
}cultivo;

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

struct{
	uint8_t aux;
	char lectura[17];
	struct {
		unsigned int conectada : 1;
		unsigned int montada : 1;
		unsigned int fallo_escritura : 1;
		unsigned int fallo_lectura : 1;
		unsigned int fallo_abrir_archivo : 1;
		unsigned int archivo_existente : 1;
		unsigned int archivo_inexistente : 1;
		unsigned int mostrar_valores : 1;
	}flag;
}tarjeta_sd;

struct{
	uint32_t contador;
	uint8_t flag;
	uint8_t estado;
	uint32_t refresh_time;
}display;


struct{
	/*	struct sistema
	 *  Estructura que contiene los estados mas generales del sistema.
	 */
	uint8_t contador_alarma;
	struct {
		unsigned int conexion_serial : 1;
		unsigned int modo_monitor_serial : 1;
		unsigned int conf_correcta : 1;
		unsigned int contador_alarma : 1;
		unsigned int first_dat_temp_int : 1;
		unsigned int menu : 1;
	}flag;
	struct {
			unsigned int leer_sd : 1;
			unsigned int abrir_archivo : 1;
			unsigned int escribir_sd: 1;
			unsigned int sd_desconectada : 1;
			unsigned int dht_desconectado : 1;
			unsigned int menu : 1;
	}error;
}sistema;

TM_RTC_AlarmTime_t AlarmTime;
TM_RTC_t datatime;
char mensaje_global[100];
extern uint16_t vuelta;

temperaturas tomate[3] = {
	//{limite_delta_temp,max_temp_fan,min_temp_fan,min_temp_calentador,max_temp_calentador}
	{5,31,30,28,29},
	{5,30,28,20,18},
	{5,31,30,28,29}
};

humedad tomate_h[3] = {
	//{minima humedad,maxima humedad}
		{30,70},
		{40,50},
		{80,90}
};

uint8_t tomate_minutos[3] = {1,2,3};

temperaturas zanahoria[3] = {
	//{limite_delta_temp,max_temp_fan,min_temp_fan,min_temp_calentador,max_temp_calentador}
	{1,2,3,4,5},
	{6,7,8,9,10},
	{11,12,13,14,15}
};

humedad zanahoria_h[3] = {
		//{minima humedad,maxima humedad}
		{60,70},
		{30,70},
		{60,70}
};

temperaturas custom[3] = {
	//{limite_delta_temp,max_temp_fan,min_temp_fan,min_temp_calentador,max_temp_calentador}
	{1,2,3,4,5},
	{6,7,8,9,10},
	{11,12,13,14,15}
};

uint8_t zanahoria_minutos[3] = {2,4,6};

// Prototipos de funciones.
void inicializar_leds(void);
void inicializar_fan(void);
void inicializar_calentador(void);
void inicializar_bomba(void);
void inicializar_ventana(void);
void task_manager(void);
void dht_interior_task(void);
void dht_exterior_task(void);
void ventana_task(void);
void led_task(void);
void ldr_task(void);
void yl69_task(void);
void display_task(void);
void serial_task(void);
void control_temp_task(void);
void check_cultivo_task(void);
void cargar_datos(void);
void procesar_paquete(void);
int fecha_valida(TM_RTC_t fecha_control);
void configurar_cultivo_tomate(void);
void configurar_cultivo_morron(void);
void configurar_cultivo_zanahoria(void);
static void USART3_Config(void);
void enviar_comando(char *cmd);
void activar_alarma(void);
void backup_etapas(void);
void log_etapas(void);
void datalogger_mejorado(void);
void serial_modificar_temp(void);
void configurar_horario_etapa(uint8_t n_etapa,uint8_t minutos);
void inicializar_botones(void);
void serial_modificar_fecha(void);
void serial_modificar_minutos_t(void);
void serial_modificar_minutos_z(void);
void control_hum_task(void);
void leer_log(void);
void leer_etapas(void);
void menu_task(void);
void inicializar_nivel_agua(void);

#endif /* MAIN_H_ */
