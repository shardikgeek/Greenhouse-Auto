#include "stm32_ub_lcd_2x16.h" // Inclusion de la libreria para manejar el LCD
#include "alvarez_timer_config.h"
#include "discovery_io.h"

// Prototipos de funciones.
void mostrarTexto();
void inicializar_pines();
void menu();
void menu_hola();
void rutina_botones();

uint8_t bandera_menu = 0;
uint8_t estado_menu = 0;
uint8_t estado_menu_hola = 0;


int main(void){

	/*	PP3 Ejercicio 1
	 *	Se debe mostrar un cartel, en un display LCD.
	 *
	 */

	SystemInit(); // Activa el systick.
	SysTick_Config(SystemCoreClock / 1000); // Configuracion del tiempo de la interrupcion (cada 1ms).
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
	static uint8_t estado = 0;

	switch(estado){
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

	estado++;
	estado = (estado>1) ? 0 : estado;
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

void menu(){

	switch(estado_menu){
	case 0:{
		estado_menu = 1;
	}break;
	case 1:{
		UB_LCD_2x16_String(0,0,"      MENU      "); // Texto en la linea 1
		UB_LCD_2x16_String(0,1,"->   Temperatura"); // Texto en la linea 1
	}break;
	case 2:{
		UB_LCD_2x16_String(0,0,"     Temperatura"); // Texto en la linea 1
		UB_LCD_2x16_String(0,1,"->       Humedad"); // Texto en la linea 1
	}break;
	case 3:{
		UB_LCD_2x16_String(0,0,"         Humedad"); // Texto en la linea 1
		UB_LCD_2x16_String(0,1,"->      Exportar"); // Texto en la linea 1
	}break;
	default:{
		estado_menu=3;
	}break;
	}
}

void menu_hola(){
	switch(estado_menu_hola){
	case 0:{
		estado_menu_hola = 1;
	}break;
	case 1:{
		UB_LCD_2x16_String(0,0,"  Temperatura   "); // Texto en la linea 1
		UB_LCD_2x16_String(0,1,"->       Interna"); // Texto en la linea 1
	}break;
	case 2:{
		UB_LCD_2x16_String(0,0,"         Interna"); // Texto en la linea 1
		UB_LCD_2x16_String(0,1,"->       Externa"); // Texto en la linea 1
	}break;
	case 3:{
		UB_LCD_2x16_String(0,0,"         Externa"); // Texto en la linea 1
		UB_LCD_2x16_String(0,1,"-> Prom. del dia"); // Texto en la linea 1
	}break;
	default:{
		estado_menu_hola=3;
	}break;
	}
}

void rutina_botones(){
	if(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_2))
	{
		delay(10);
		if(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_2)){
			GPIO_ToggleBits(GPIOD, GPIO_Pin_14);	//PD14 encendido
			TIM_Cmd(TIM4, DISABLE); // Habilita el contador para el timer 2.
			bandera_menu = (!bandera_menu) ? 1 : bandera_menu; // Se sube la bandera.

			if(bandera_menu && !estado_menu && !estado_menu_hola){
				estado_menu = 1;
				menu();
			}
			else if(estado_menu>=1 && !estado_menu_hola){
				estado_menu_hola = 1;
				menu_hola();
			}
		}
		while(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_2));
	}

	if(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_1))
	{
		delay(10);
		if(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_1)){
			GPIO_ToggleBits(GPIOD, GPIO_Pin_13);	//PD14 encendido

			if(bandera_menu && !estado_menu_hola)
				mostrarTexto();

			if(!estado_menu_hola){
				TIM_Cmd(TIM4, ENABLE); // Habilita el contador para el timer 2.
				bandera_menu = (bandera_menu) ? 0 : bandera_menu; // se baja la bandera.
				estado_menu = 0;
			}
			else{
				estado_menu_hola = 0;
				menu();
			}
		}
		while(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_1));
	}

	if(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_0))
	{
		delay(10);
		if(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_0)){
			GPIO_ToggleBits(GPIOD, GPIO_Pin_12);	//PD14 encendido
			if(bandera_menu && estado_menu !=0 && !estado_menu_hola){
				estado_menu++;
				menu();
			}
			if(bandera_menu && estado_menu !=0 && estado_menu_hola){
				estado_menu_hola++;
				menu_hola();
			}
			while(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_0));
		}
	}

	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))
	{
		delay(10);
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)){
			GPIO_ToggleBits(GPIOD, GPIO_Pin_15);	//PD14 encendido
			if(bandera_menu && estado_menu !=0 && !estado_menu_hola){
				estado_menu--;
				menu();
			}
			if(bandera_menu && estado_menu !=0 && estado_menu_hola){
				estado_menu_hola--;
				menu_hola();
			}
		}
		while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0));
	}
}



