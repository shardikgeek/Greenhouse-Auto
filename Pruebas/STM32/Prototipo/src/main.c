#include "stm32_ub_lcd_2x16.h" // Inclusion de la libreria para manejar el LCD
#include "alvarez_timer_config.h"
#include "discovery_io.h"
#include "menu.h"

// Prototipos de funciones.
void mostrarTexto();
void inicializar_pines();


int main(void){

	/*	PP3 Ejercicio 1
	 *	Se debe mostrar un cartel, en un display LCD.
	 *
	 */

	enableSystick();
	UB_LCD_2x16_Init(); // Inicializacion del display.
	inicializar_pines();
	inicializar_boton_user();
	TIM4_Start();

	while(1){
		rutina_botones();
	}
}

void mostrarTexto(){
	/*	Funcion mostrarTexto()
	 *	Escribe un cartel en el display LCD conectado a la placa discovery.
	 */
	static uint8_t estado_mostrar_texto = 0;

	switch(estado_mostrar_texto){
	case 0:{
		UB_LCD_2x16_String(0,0,"Tomate     21:30"); // Texto en la linea 1
		UB_LCD_2x16_String(0,1,"Temperatura  24C"); // Texto en la linea 1
	}break;
	case 1:{
		UB_LCD_2x16_String(0,0,"Etapa Germiacion"); // Texto en la linea 1
		UB_LCD_2x16_String(0,1,"Humedad      68%"); // Texto en la linea 1
	}break;
	default:
		UB_LCD_2x16_String(1,0,"     ERROR    "); // Texto en la linea 1
	}

	estado_mostrar_texto++;
	estado_mostrar_texto = (estado_mostrar_texto>1) ? 0 : estado_mostrar_texto;
}

void inicializar_pines()
{
	/* Funcion inicializar_pines()
	 * Inicializa 3 leds de la placa Discovery como salida(D12, D13 y D14)y 3 botones externos
	 * como entrada(C0, C1 y C2).
	 */

	GPIO_InitTypeDef GPIO_Init_Pins; // Estrucura de datos para configurar el GPIO

	//
	//Inicializacion de los leds.
	//

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	GPIO_Init_Pins.GPIO_Pin= GPIO_Pin_15 | GPIO_Pin_12 | GPIO_Pin_13| GPIO_Pin_14;
	GPIO_Init_Pins.GPIO_Mode=GPIO_Mode_OUT ;
	GPIO_Init_Pins.GPIO_Speed= GPIO_Speed_100MHz;
	GPIO_Init_Pins.GPIO_OType= GPIO_OType_PP ;
	GPIO_Init_Pins.GPIO_PuPd= GPIO_PuPd_NOPULL;

	GPIO_Init(GPIOD,&GPIO_Init_Pins); // Carga de la estrucura de datos.

	//
	//Inicializacion de los botones.
	//

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	GPIO_Init_Pins.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2;
	GPIO_Init_Pins.GPIO_Mode = GPIO_Mode_IN; //Entrada
	GPIO_Init_Pins.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init_Pins.GPIO_PuPd = GPIO_PuPd_NOPULL;

	GPIO_Init(GPIOC, &GPIO_Init_Pins);// Carga de la estrucura de datos.

}




