/*
 * menu.h
 *
 *  Created on: 31 jul. 2018
 *      Author: diego
 */

#ifndef MENU_H_
#define MENU_H_
// Macros para menuses.
#define MENU_TEMPERATURA 1
#define MENU_HUMEDAD 2
#define MENU_EXPORTAR 3

uint8_t estado_menu_hola = 0;

struct{
    struct{
    unsigned char principal : 1;
    unsigned char lectura_dht : 1;
    unsigned char lectura_humedad : 1;
    unsigned char lectura_temp : 1;
    unsigned char lectura_temp2 : 1;
    unsigned char motor : 1;
    unsigned char tanque : 1;
    unsigned char tanque2 : 1;
  }menu_flags; // 1 byte.

  uint8_t estado_menu_principal;
  uint8_t estado_menu_temperatura;
  uint8_t estado_menu_humedad;
  uint8_t estado_menu_exportar;
}menu_lcd;

void menu_principal(){

	switch(menu_lcd.estado_menu_principal){
	case 0:{
		menu_lcd.estado_menu_principal = MENU_TEMPERATURA;
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
		menu_lcd.estado_menu_principal=MENU_EXPORTAR;
	}break;
	}
}

void menu_temperatura(){
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
			menu_lcd.menu_flags.principal = (!menu_lcd.menu_flags.principal) ? 1 : menu_lcd.menu_flags.principal; // Se sube la bandera.

			if(menu_lcd.menu_flags.principal && !menu_lcd.estado_menu_principal && !estado_menu_hola){
				menu_lcd.estado_menu_principal = MENU_TEMPERATURA;
				menu_principal();
			}
			else if(menu_lcd.estado_menu_principal>=1 && !estado_menu_hola){
				estado_menu_hola = 1;
				menu_temperatura();
			}
		}
		while(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_2));
	}

	if(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_1))
	{
		delay(10);
		if(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_1)){
			GPIO_ToggleBits(GPIOD, GPIO_Pin_13);	//PD14 encendido

			if(menu_lcd.menu_flags.principal && !estado_menu_hola)
				mostrarTexto();

			if(!estado_menu_hola){
				TIM_Cmd(TIM4, ENABLE); // Habilita el contador para el timer 2.
				menu_lcd.menu_flags.principal = (menu_lcd.menu_flags.principal) ? 0 : menu_lcd.menu_flags.principal; // se baja la bandera.
				menu_lcd.estado_menu_principal = 0;
			}
			else{
				estado_menu_hola = 0;
				menu_principal();
			}
		}
		while(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_1));
	}

	if(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_0))
	{
		delay(10);
		if(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_0)){
			GPIO_ToggleBits(GPIOD, GPIO_Pin_12);	//PD14 encendido
			if(menu_lcd.menu_flags.principal && menu_lcd.estado_menu_principal !=0 && !estado_menu_hola){
				menu_lcd.estado_menu_principal++;
				menu_principal();
			}
			if(menu_lcd.menu_flags.principal && menu_lcd.estado_menu_principal !=0 && estado_menu_hola){
				estado_menu_hola++;
				menu_temperatura();
			}
			while(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_0));
		}
	}

	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))
	{
		delay(10);
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)){
			GPIO_ToggleBits(GPIOD, GPIO_Pin_15);	//PD14 encendido
			if(menu_lcd.menu_flags.principal && menu_lcd.estado_menu_principal !=0 && !estado_menu_hola){
				menu_lcd.estado_menu_principal--;
				menu_principal();
			}
			if(menu_lcd.menu_flags.principal && menu_lcd.estado_menu_principal !=0 && estado_menu_hola){
				estado_menu_hola--;
				menu_temperatura();
			}
		}
		while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0));
	}
}

#endif /* MENU_H_ */
