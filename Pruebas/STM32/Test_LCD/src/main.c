#include "stm32_ub_lcd_2x16.h" // Inclusion de la libreria para manejar el LCD
#define BLINK_TIME 1000 // Tiempo de paradeo de la pantalla en mS

// Variables globales.
uint32_t TimingDelay = 0;

// Prototipos de funciones.
void delay(uint32_t tiempo);
void mostrarTexto();

int main(void){

	/*	PP3 Ejercicio 1
	 *	Se debe mostrar un cartel, en un display LCD.
	 *
	 */

	SystemInit(); // Activa el systick.
	SysTick_Config(SystemCoreClock / 1000); // Configuracion del tiempo de la interrupcion (cada 1ms).
	UB_LCD_2x16_Init(); // Inicializacion del display.

	while(1){
		mostrarTexto(); // Escribe el display LCD.
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

