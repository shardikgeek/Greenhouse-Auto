#include <stm32f4xx.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dht.h"
#include "stm32_ub_lcd_2x16.h" // Inclusion de la libreria para manejar el LCD
#define BLINK_TIME 1e6 // Tiempo de paradeo de la pantalla en mS



// Variables globales.
uint32_t TimingDelay = 0;

struct{
	uint32_t contador;
	uint8_t flag;
}led;

// Prototipos de funciones.
void leer_dht(void);
void inicializar_leds(void);



int main(void){

	/*	PP3 Ejercicio 1
	 *	Se debe mostrar un cartel, en un display LCD.
	 *
	 */

	SystemInit(); // Activa el systick.
	SysTick_Config(SystemCoreClock / 1e6); // Configuracion del tiempo de la interrupcion (cada 1us).
	UB_LCD_2x16_Init(); // Inicializacion del display.
	inicializar_leds();

	while(1){

		// Chequeo dht.
		if(dht.flag){
			leer_dht();
			dht.flag = 0;
			if(dht.flag_timeout){
				dht.flag_timeout = 0;
				dht.estado = STATE_DHT_TIMEOUT;
				UB_LCD_2x16_String(0,1,"TIMEOUT    "); // Texto en la linea 1
			}
			else{
				UB_LCD_2x16_String(0,1,"               "); // Texto en la linea 1
			}
		}

		//Chequeo LED.
		switch(led.flag){
		case 0: GPIO_ResetBits(GPIOD,GPIO_Pin_13);break;
		case 1:	GPIO_SetBits(GPIOD,GPIO_Pin_13);break;
		default:GPIO_ResetBits(GPIOD,GPIO_Pin_13);break;
		}

//		if(display.flag){
//			if(dht.estado == STATE_DHT_CHECKSUM_GOOD){
//				UB_LCD_2x16_String(1,0,"TEMP:"); // Texto en la linea 1
//				UB_LCD_2x16_String(7,0,dht.temperatura_string); // Texto en la linea 1
//			}
//			else{
//				UB_LCD_2x16_String(1,0,"OTRO"); // Texto en la linea 1
//			}
//			display.flag = 0;
//		}

	}
}

void delay(uint32_t tiempo){
	/* Funcion delay()
	 * Realiza un retardo en funcion de la frecuencia de reloj del microcontrolador.
	 * Recibe como parametro el retraso, para este caso, en mS debido a la configuracion del Systick.
	 */

	TimingDelay = tiempo;
	while(TimingDelay!=0);

}

void TimingDelay_Decrement(void){
	/* Funcion TimingDelay_Decrement
	 * Es llamada por la rutina de atencion del systick.
	 * En este caso decrementa una variable global.
	 */

	if (TimingDelay != 0)
	{
		TimingDelay--;
	}

	// Rutina DHT
	if(dht.flag){
		if(dht.timeout >= 1){
			dht.timeout--;
		}

		if(dht.timeout <= 0){
			dht.flag_timeout = 1;
		}
	}

	if(dht.contador >= 1){
		dht.contador--;
	}
	else{
		dht.flag = 1;
		dht.contador = 2000e3;
		dht.flag_timeout = 0;
		dht.timeout = 1000e3; // Se le agrega un tiempo de timeout para esperar la funcion DHT11Start.
	}

	// Rutina del LED.
	if(led.contador >= 1){
		led.contador--;
	}
	else{
		led.flag = (led.flag) ? 0 : 1;
		led.contador = 1000e3;
	}
}

void leer_dht(){

	uint8_t Rh_byte1, Rh_byte2, Temp_byte1, Temp_byte2;
	uint16_t sum, RH, TEMP;
	char aux[5] = "";

	DHT11_start ();

	check_response ();
//	GPIO_ToggleBits(GPIOD,GPIO_Pin_12);
	if(dht.flag_timeout) //Si se vencio el timeout vuelve al main.
		return;

	Rh_byte1 = read_data ();
	if(dht.flag_timeout)  //Si se vencio el timeout vuelve al main.
			return;

	Rh_byte2 = read_data ();
	if(dht.flag_timeout)  //Si se vencio el timeout vuelve al main.
			return;

	Temp_byte1 = read_data ();
	if(dht.flag_timeout)  //Si se vencio el timeout vuelve al main.
			return;

	Temp_byte2 = read_data ();
	if(dht.flag_timeout)  //Si se vencio el timeout vuelve al main.
			return;

	sum = read_data();
	if(dht.flag_timeout)  //Si se vencio el timeout vuelve al main.
			return;

	if (sum == (Rh_byte1+Rh_byte2+Temp_byte1+Temp_byte2))    // if the data is correct
	{
		UB_LCD_2x16_String(1,0,"TEMP:"); // Texto en la linea 1
		sprintf(aux,"%d.%d",Temp_byte1,Temp_byte2);
		UB_LCD_2x16_String(6,0,aux); // Texto en la linea 1
		//UB_LCD_2x16_String(1,0," C");
	}
	else{
		UB_LCD_2x16_String(1,0,"ERR-CHECKSUM   "); // Texto en la linea 1
	}
}

void inicializar_leds(){
	GPIO_InitTypeDef GPIO_Init_Pins; // Estrucura de datos para configurar el GPIO

	//
	//Inicializacion de los leds.
	//

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	GPIO_Init_Pins.GPIO_Pin= GPIO_Pin_15 |GPIO_Pin_12 | GPIO_Pin_13| GPIO_Pin_14;
	GPIO_Init_Pins.GPIO_Mode=GPIO_Mode_OUT ;
	GPIO_Init_Pins.GPIO_Speed= GPIO_Speed_100MHz;
	GPIO_Init_Pins.GPIO_OType= GPIO_OType_PP ;
	GPIO_Init_Pins.GPIO_PuPd= GPIO_PuPd_NOPULL;

	GPIO_Init(GPIOD,&GPIO_Init_Pins); // Carga de la estrucura de datos.

}


