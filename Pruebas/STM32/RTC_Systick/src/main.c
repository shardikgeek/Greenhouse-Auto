#define BLINK_TIME 500 // Tiempo de paradeo de la pantalla en mS

/* Include core modules */
#include "stm32f4xx.h"
#include "stm32_ub_lcd_2x16.h" // Inclusion de la libreria para manejar el LCD
/* Include my libraries here */
#include "defines.h"
#include "tm_stm32f4_ds1307.h"
#include <stdio.h>


// Variables globales.
uint32_t TimingDelay = 0;

// Prototipos de funciones.
void delay(uint32_t tiempo);
void mostrarTexto();

int main(void){
	char str[16];
	TM_DS1307_Time_t time;
	uint8_t last;

	/*	PP3 Ejercicio 1
	 *	Se debe mostrar un cartel, en un display LCD.
	 *
	 */

	 /* Initialize DS1307 */
	if (TM_DS1307_Init() != TM_DS1307_Result_Ok) {
		/* Red LED on */

		/* Show on LCD */
		//TM_ILI9341_Puts(10, 10, "ERROR", &TM_Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_ORANGE);

		UB_LCD_2x16_String(1,0,"ERROR"); // Texto en la linea 1

		while (1);
	}

	 /* Set output pin to 4096 Hz */
	TM_DS1307_EnableOutputPin(TM_DS1307_OutputFrequency_4096Hz);


	SystemInit(); // Activa el systick.
	SysTick_Config(SystemCoreClock / 1000); // Configuracion del tiempo de la interrupcion (cada 1ms).
	UB_LCD_2x16_Init(); // Inicializacion del display.

	while(1){
		 TM_DS1307_GetDateTime(&time);
		 /* Display on LCD */
		 sprintf(str, "%d:%d:%d",time.hours,time.minutes,time.seconds);
		 UB_LCD_2x16_String(1,0,str); // Texto en la linea 1
		 delay(BLINK_TIME);


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
}

void mostrarTexto(){
	/*	Funcion mostrarTexto()
	 *	Escribe un cartel en el display LCD conectado a la placa discovery.
	 */

	UB_LCD_2x16_String(1,0,"\"TDII FRBB\""); // Texto en la linea 1
	UB_LCD_2x16_String(1,1,"Lo mejor!"); // Texto en la linea 2
	delay(BLINK_TIME);
	UB_LCD_2x16_Clear(); // Limpia la pantalla LCD.
	delay(BLINK_TIME);
}

