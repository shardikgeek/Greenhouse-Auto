/*
 * menu.h
 *
 *  Created on: 31 jul. 2018
 *      Author: diego
 */

#ifndef MENU_H_
#define MENU_H_
// Macros para menuses.
#define MENU_PRINCIPAL_TEMPERATURA 1
#define MENU_PRINCIPAL_HUMEDAD 2
#define MENU_PRINCIPAL_EXPORTAR 3

// Prototipos de funciones
void menu_principal(void);
void menu_temperatura(void);
void menu_humedad(void);
void menu_exportar(void);
void rutina_botones(void);
void mostrar_temperatura_interna(void);
void mostrar_temperatura_externa(void);
void mostrar_temperatura_promedio(void);
void mostrar_msj_error(void);

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
		menu_lcd.estado_menu_principal = MENU_PRINCIPAL_TEMPERATURA;
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
		menu_lcd.estado_menu_principal=MENU_PRINCIPAL_EXPORTAR;
	}break;
	}
}

void menu_temperatura(){
	switch(menu_lcd.estado_menu_temperatura){
	case 0:{
		menu_lcd.estado_menu_temperatura = 1;
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
		menu_lcd.estado_menu_temperatura=3;
	}break;
	}
}

void menu_humedad(){
	switch(menu_lcd.estado_menu_humedad){
	case 0:{
		menu_lcd.estado_menu_humedad = 1;
	}break;
	case 1:{
		UB_LCD_2x16_String(0,0,"    Humedad     "); // Texto en la linea 1
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
		menu_lcd.estado_menu_humedad=3;
	}break;
	}
}

void menu_exportar(){
	switch(menu_lcd.estado_menu_exportar){
	case 0:{
		menu_lcd.estado_menu_exportar = 1;
	}break;
	case 1:{
		UB_LCD_2x16_String(0,0,"    Exportar    "); // Texto en la linea 1
		UB_LCD_2x16_String(0,1,"->   Temperatura"); // Texto en la linea 1
	}break;
	case 2:{
		UB_LCD_2x16_String(0,0,"     Temperatura"); // Texto en la linea 1
		UB_LCD_2x16_String(0,1,"->       Humedad"); // Texto en la linea 1
	}break;
	case 3:{
		UB_LCD_2x16_String(0,0,"         Humedad"); // Texto en la linea 1
		UB_LCD_2x16_String(0,1,"->       WatchOS"); // Texto en la linea 1
	}break;
	default:{
		menu_lcd.estado_menu_exportar=3;
	}break;
	}
}

void rutina_botones(){
	if(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_2))
	{
		// BOTON MENU/ENTER
		delay(10);
		if(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_2)){
			GPIO_ToggleBits(GPIOD, GPIO_Pin_14);	//PD14 encendido
			TIM_Cmd(TIM4, DISABLE); // Habilita el contador para el timer 2.
			menu_lcd.menu_flags.principal = (!menu_lcd.menu_flags.principal) ? 1 : menu_lcd.menu_flags.principal; // Se sube la bandera.

			if(menu_lcd.menu_flags.principal && !menu_lcd.estado_menu_principal && !(menu_lcd.estado_menu_exportar | menu_lcd.estado_menu_temperatura | menu_lcd.estado_menu_humedad)){
				menu_lcd.estado_menu_principal = MENU_PRINCIPAL_TEMPERATURA;
				menu_principal();
			}
			else if(menu_lcd.estado_menu_principal>=1 && !(menu_lcd.estado_menu_exportar | menu_lcd.estado_menu_temperatura | menu_lcd.estado_menu_humedad)){

				if(menu_lcd.estado_menu_principal == MENU_PRINCIPAL_TEMPERATURA){
					menu_lcd.estado_menu_temperatura = 1;
					menu_temperatura();
				}
				else if(menu_lcd.estado_menu_principal == MENU_PRINCIPAL_HUMEDAD){
					menu_lcd.estado_menu_humedad = 1;
					menu_humedad();
				}
				else{
					menu_lcd.estado_menu_exportar = 1;
					menu_exportar();
				}
			}
			else if(menu_lcd.estado_menu_principal >=1 && menu_lcd.estado_menu_temperatura >= 1 ){
				switch(menu_lcd.estado_menu_temperatura){
				case 1:
					mostrar_temperatura_interna();break;
				case 2:
					mostrar_temperatura_externa();break;
				case 3:
					mostrar_temperatura_promedio();break;
				default:
					mostrar_msj_error();break;
				}

			}
			while(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_2));
		}
	}

		if(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_1))
		{
			// BOTON BACK.
			delay(10);
			if(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_1)){
				GPIO_ToggleBits(GPIOD, GPIO_Pin_13);	//PD14 encendido

				if(menu_lcd.menu_flags.principal && !(menu_lcd.estado_menu_exportar | menu_lcd.estado_menu_temperatura | menu_lcd.estado_menu_humedad))
					mostrarTexto();

				if(!(menu_lcd.estado_menu_exportar | menu_lcd.estado_menu_temperatura | menu_lcd.estado_menu_humedad)){
					TIM_Cmd(TIM4, ENABLE); // Habilita el contador para el timer 2.
					menu_lcd.menu_flags.principal = (menu_lcd.menu_flags.principal) ? 0 : menu_lcd.menu_flags.principal; // se baja la bandera.
					menu_lcd.estado_menu_principal = 0;
				}
				else{
					if(menu_lcd.estado_menu_temperatura){
						menu_lcd.estado_menu_temperatura = 0;
						menu_principal();}
					else if(menu_lcd.estado_menu_humedad){
						menu_lcd.estado_menu_humedad = 0;
						menu_principal();
					}
					else if(menu_lcd.estado_menu_exportar){
						menu_lcd.estado_menu_exportar = 0;
						menu_principal();
					}
				}
			}
			while(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_1));
		}

		if(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_0))
		{
			// BOTON UP.
			delay(10);
			if(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_0)){
				GPIO_ToggleBits(GPIOD, GPIO_Pin_12);	//PD14 encendido
				if(menu_lcd.menu_flags.principal && menu_lcd.estado_menu_principal !=0 && !(menu_lcd.estado_menu_exportar | menu_lcd.estado_menu_temperatura | menu_lcd.estado_menu_humedad)){
					menu_lcd.estado_menu_principal++;
					menu_principal();
				}
				if(menu_lcd.menu_flags.principal && menu_lcd.estado_menu_principal !=0 && menu_lcd.estado_menu_temperatura){
					menu_lcd.estado_menu_temperatura++;
					menu_temperatura();
				}
				if(menu_lcd.menu_flags.principal && menu_lcd.estado_menu_principal !=0 && menu_lcd.estado_menu_humedad){
					menu_lcd.estado_menu_humedad++;
					menu_humedad();
				}
				if(menu_lcd.menu_flags.principal && menu_lcd.estado_menu_principal !=0 && menu_lcd.estado_menu_exportar){
					menu_lcd.estado_menu_exportar++;
					menu_exportar();
				}
				while(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_0));
			}
		}

		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))
		{
			// BOTON DOWN.
			delay(10);
			if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)){
				GPIO_ToggleBits(GPIOD, GPIO_Pin_15);	//PD14 encendido
				if(menu_lcd.menu_flags.principal && menu_lcd.estado_menu_principal !=0 && !(menu_lcd.estado_menu_exportar | menu_lcd.estado_menu_temperatura | menu_lcd.estado_menu_humedad)){
					menu_lcd.estado_menu_principal--;
					menu_principal();
				}
				if(menu_lcd.menu_flags.principal && menu_lcd.estado_menu_principal !=0 && menu_lcd.estado_menu_temperatura){
					menu_lcd.estado_menu_temperatura--;
					menu_temperatura();
				}
				if(menu_lcd.menu_flags.principal && menu_lcd.estado_menu_principal !=0 && menu_lcd.estado_menu_humedad){
					menu_lcd.estado_menu_humedad--;
					menu_humedad();
				}
				if(menu_lcd.menu_flags.principal && menu_lcd.estado_menu_principal !=0 && menu_lcd.estado_menu_exportar){
					menu_lcd.estado_menu_exportar--;
					menu_exportar();
				}
			}
			while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0));
		}
}

void mostrar_temperatura_interna(){
	UB_LCD_2x16_String(0,0,"Esta quenchi    "); // Texto en la linea 1
	UB_LCD_2x16_String(0,1,"                "); // Texto en la linea 1
	delay(2000);
	menu_temperatura();
}

void mostrar_temperatura_externa(){
	UB_LCD_2x16_String(0,0,"Esta friio      "); // Texto en la linea 1
	UB_LCD_2x16_String(0,1,"                "); // Texto en la linea 1
	delay(2000);
	menu_temperatura();
}

void mostrar_temperatura_promedio(){
	UB_LCD_2x16_String(0,0,"La promedioes..."); // Texto en la linea 1
	UB_LCD_2x16_String(0,1,"                "); // Texto en la linea 1
	delay(2000);
	menu_temperatura();
}

void mostrar_msj_error(){
	UB_LCD_2x16_String(0,0,"    ERRROR      "); // Texto en la linea 1
	UB_LCD_2x16_String(0,1,"                "); // Texto en la linea 1
	delay(2000);
	menu_temperatura();
}

#endif /* MENU_H_ */
