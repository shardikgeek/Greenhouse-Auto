/*
 * sai_estructura.c
 *
 *  Created on: 5 sep. 2018
 *      Author: diego
 */
#include "sai_estructura.h"

struct{
	uint16_t contador;
	uint8_t flag;
}led;

struct{
	uint16_t contador;
	uint16_t adc_cuentas;
	uint8_t flag;
}ldr;

struct{
	uint16_t contador;
	uint16_t adc_cuentas;
	uint8_t flag;
}yl;

struct{
	uint16_t contador;
	uint8_t flag;
}display;

void scheduler(void){
	if(led.contador >= 1){
		led.contador--;
	}
	else{
		led.flag = (led.flag) ? 0 : 1;
		led.contador = 1000;
	}

	if(ldr.contador >= 1){
		ldr.contador--;
	}
	else{
		ldr.flag = 1;
		ldr.contador = 500;
	}

	if(yl.contador >= 1){
		yl.contador--;
	}
	else{
		yl.flag = 1;
		yl.contador = 500;
	}

	if(display.contador >= 1){
		display.contador--;
	}
	else{
		display.flag = 1;
		display.contador = 1000;
	}


}

void flag_checker(void){

	switch(led.flag){
	case 0: GPIO_ResetBits(GPIOD,GPIO_Pin_13);break;
	case 1:	GPIO_SetBits(GPIOD,GPIO_Pin_13);break;
	default:GPIO_ResetBits(GPIOD,GPIO_Pin_13);break;
	}

	if(display.flag){
		refresh_display();
		display.flag = 0;
	}

	if(ldr.flag){
		ldr.adc_cuentas = adc_leer_cuentas_ldr();
		ldr.flag = 0;
	}

	if(yl.flag){
		yl.adc_cuentas = adc_leer_cuentas_yl69();
		yl.flag = 0;
	}

}

void refresh_display(void){
	char buffer[10];
	sprintf(buffer,"%d   ",ldr.adc_cuentas);
	UB_LCD_2x16_String(10,0,buffer);
	sprintf(buffer,"%d   ",yl.adc_cuentas);
	UB_LCD_2x16_String(10,1,buffer);
}
