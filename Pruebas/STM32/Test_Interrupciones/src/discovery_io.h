/*
 * discovery_io.h
 *
 *  Created on: 27 jul. 2018
 *      Author: diego
 */

#ifndef DISCOVERY_IO_H_
#define DISCOVERY_IO_H_
#define BLINK_TIME 1000 // Tiempo de paradeo de la pantalla en mS

// Variables globales.
uint32_t TimingDelay = 0;

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

void inicializar_boton_user(){

	//
	//Inicializacion de los botones.
	//

	GPIO_InitTypeDef GPIO_Init_Pins; // Estrucura de datos para configurar el GPIO

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_Init_Pins.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init_Pins.GPIO_Mode = GPIO_Mode_IN; //Entrada
	GPIO_Init_Pins.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init_Pins.GPIO_PuPd = GPIO_PuPd_NOPULL;

	GPIO_Init(GPIOA, &GPIO_Init_Pins);// Carga de la estrucura de datos.

}

void inicializar_leds(){
	GPIO_InitTypeDef GPIO_Init_Pins; // Estrucura de datos para configurar el GPIO

	//
	//Inicializacion de los leds.
	//

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	GPIO_Init_Pins.GPIO_Pin= GPIO_Pin_11 |GPIO_Pin_12 | GPIO_Pin_13| GPIO_Pin_14;
	GPIO_Init_Pins.GPIO_Mode=GPIO_Mode_OUT ;
	GPIO_Init_Pins.GPIO_Speed= GPIO_Speed_100MHz;
	GPIO_Init_Pins.GPIO_OType= GPIO_OType_PP ;
	GPIO_Init_Pins.GPIO_PuPd= GPIO_PuPd_NOPULL;

	GPIO_Init(GPIOD,&GPIO_Init_Pins); // Carga de la estrucura de datos.

}

void boton_press(void (*ptr_func)()){
	if(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))
	{
		delay(10);
		if(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)){

			if(ptr_func != 0){
				ptr_func();
			}
		}
		while(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0));
	}
}


#endif /* DISCOVERY_IO_H_ */
