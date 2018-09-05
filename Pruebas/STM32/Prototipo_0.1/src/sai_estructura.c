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
	uint8_t flag;
}ldr;

int adc_cuentas;

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


}

void flag_checker(void){

	switch(led.flag){
	case 0: GPIO_ResetBits(GPIOD,GPIO_Pin_13);break;
	case 1:	GPIO_SetBits(GPIOD,GPIO_Pin_13);break;
	default:GPIO_ResetBits(GPIOD,GPIO_Pin_13);break;
	}

	if(ldr.flag){
		leer();
		ldr.flag = 0;
	}
}

void leer(){

	char buffer[10];
	adc_cuentas = adc_leer_cuentas_ldr();
	sprintf(buffer,"%d   ",adc_cuentas);
	UB_LCD_2x16_String(10,0,buffer);


}
