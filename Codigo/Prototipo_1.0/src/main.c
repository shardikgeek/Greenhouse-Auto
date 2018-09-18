//#include <stm32f4xx.h>
//#include "dht.h"
//#include "adc.h"
//#include "stm32_ub_lcd_2x16.h" // Inclusion de la libreria para manejar el LCD
//
//// Variables globales.
//struct{
//	uint32_t contador;
//	uint8_t flag;
//}led;
//
//struct{
//	uint32_t contador;
//	uint8_t flag;
//	uint8_t estado;
//}display;
//
//struct{
//	uint16_t contador;
//	uint16_t adc_cuentas;
//	uint8_t flag;
//}ldr;
//
//struct{
//	uint16_t contador;
//	uint16_t adc_cuentas;
//	uint8_t flag;
//}yl;
//
//// Prototipos de funciones.
//void inicializar_leds();
//void task_manager(void);
#include "main.h"

int main(void){

	/*	PP3 Ejercicio 1
	 *	Se debe mostrar un cartel, en un display LCD.
	 *
	 */
	UB_LCD_2x16_Init(); // Inicializacion del display.
	inicializar_leds();
	adc_inicializar();
	TIM5_Start(); // Inicializa el timer del DHT.
	SystemInit(); // Activa el systick.
	SysTick_Config(SystemCoreClock / 1e3); // Configuracion del tiempo de la interrupcion (cada 1us).

	while(1){
		task_manager();
	}
}

void task_scheduler(void){
	/* Funcion TimingDelay_Decrement
	 * Es llamada por la rutina de atencion del systick.
	 * En este caso decrementa una variable global.
	 */

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
		dht.contador = 2000;
		dht.flag_timeout = 0;
		dht.timeout = 1000; // Se le agrega un tiempo de timeout para esperar la funcion DHT11Start.
	}

	// Rutina LDR
	if(ldr.contador >= 1){
		ldr.contador--;
	}
	else{
		ldr.flag = 1;
		ldr.contador = 500;
	}

	// Ruitna YL-69
	if(yl.contador >= 1){
		yl.contador--;
	}
	else{
		yl.flag = 1;
		yl.contador = 500;
	}

	// Rutina del LED.
	if(led.contador >= 1){
		led.contador--;
	}
	else{
		led.flag = (led.flag) ? 0 : 1;
		led.contador = 1000;
	}

	// Rutina display.
	if(display.contador >= 1){
		display.contador--;
	}
	else{
		display.flag = 1;
		display.contador = 3000;
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

void task_manager(void){
	// Chequeo dht.
	if(dht.flag){
		leer_dht();
		dht.flag = 0;
	}

	//Chequeo LED.
	switch(led.flag){
	case 0: GPIO_ResetBits(GPIOD,GPIO_Pin_13);break;
	case 1:	GPIO_SetBits(GPIOD,GPIO_Pin_13);break;
	default:GPIO_ResetBits(GPIOD,GPIO_Pin_13);break;
	}

	if(ldr.flag){
		ldr.adc_cuentas = adc_leer_cuentas_ldr();
		ldr.flag = 0;
	}

	if(yl.flag){
		yl.adc_cuentas = adc_leer_cuentas_yl69();
		yl.flag = 0;
	}

	// Refresco de display.
	if(display.flag){
		char buffer[10];

		if(display.estado <2){
			display.estado++;
		}
		else{
			display.estado = 0;
		}

		switch(display.estado){
		case 0:{
			sprintf(buffer,"%d   ",ldr.adc_cuentas);
			UB_LCD_2x16_String(0,0,"LDR:       ");
			UB_LCD_2x16_String(12,0,buffer);
			UB_LCD_2x16_String(0,1,"                ");
		};break;

		case 1:{
			sprintf(buffer,"%d   ",yl.adc_cuentas);
			UB_LCD_2x16_String(0,0,"YL-69:     ");
			UB_LCD_2x16_String(12,0,buffer);
			UB_LCD_2x16_String(0,1,"                ");
		};break;

		case 2:{
			switch(dht.estado){
			case STATE_DHT_CHECKSUM_GOOD:{
				UB_LCD_2x16_String(0,0,"Temp:       "); // Texto en la linea 1
				UB_LCD_2x16_String(12,0,dht.temperatura_string); // Texto en la linea 1
				UB_LCD_2x16_String(0,1,"                "); // Texto en la linea 1
				display.flag = 0;
			};break;
			case STATE_DHT_CHECKSUM_BAD:{
				UB_LCD_2x16_String(0,0,"Error de CHEcK  "); // Texto en la linea 1
				UB_LCD_2x16_String(0,1,"                "); // Texto en la linea 1
				display.flag = 0;
			};break;
			case STATE_DHT_DESCONECTADO:{
				UB_LCD_2x16_String(0,0,"DHT desconectado"); // Texto en la linea 1
				UB_LCD_2x16_String(0,1,"                "); // Texto en la linea 1
				display.flag = 0;
			};break;
			case STATE_DHT_TIMEOUT:{
				UB_LCD_2x16_String(0,0,"DHT Timeout     "); // Texto en la linea 1
				UB_LCD_2x16_String(0,1,"                "); // Texto en la linea 1
				display.flag = 0;
			};break;
			default:{
				UB_LCD_2x16_String(0,0,"Err  desconocido"); // Texto en la linea 1
				UB_LCD_2x16_String(0,1,"                "); // Texto en la linea
				display.flag = 0;
			};break;}
		};break;

		default:{
			UB_LCD_2x16_String(0,0,"Display-fail");
			UB_LCD_2x16_String(0,1,"                ");
		};break;
	}
		display.flag = 0;

	}
}







