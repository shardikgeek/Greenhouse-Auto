#include "main.h"

int contador_led = 1000;


int main(void){

	/*	Prototipo 1.3
	 *	Este programa es un prototipo del control de invernadero.
	 *	Tiene RTC, una prueba de seguimiento de cultivos, lee dos sensores externos de temperatura y humedad
	 *	y dos analogicos. 
	 *
	 */

	UB_LCD_2x16_Init(); // Inicializacion del display.
	inicializar_leds();
	inicializar_fan();
	inicializar_calentador();
	inicializar_bomba();
	adc_inicializar();
	TIM5_Start(); // Inicializa el timer del DHT.
	USART3_Config();

	TM_RTC_Init(TM_RTC_ClockSource_External); // Se inicializa el RTC.
	TM_RTC_Interrupts(TM_RTC_Int_1s); //Se activan las interrupciones del RTC.

	SystemInit(); // Activa el systick.
	SysTick_Config(SystemCoreClock / 1e3); // Configuracion del tiempo de la interrupcion (cada 1us).
	UB_Fatfs_Init(); // Init de la SD.

	serial.contador = 0;
	serial.flag.comienzo_paquete = 0;
	serial.flag.fin_paquete = 0;
	serial.timeout = 0;
	sistema.flag.conexion_serial = 0;
	sistema.flag.modo_monitor_serial = 0;


//	// Check ob Medium eingelegt ist
//	  if(UB_Fatfs_CheckMedia(MMC_0)==FATFS_OK) {
//			//GPIO_SetBits(GPIOD,GPIO_Pin_13);
//		// Media mounten
//	    if(UB_Fatfs_Mount(MMC_0)==FATFS_OK) {
//	    	//GPIO_SetBits(GPIOD,GPIO_Pin_12);
//	      // File zum schreiben im root neu anlegen
//	      if(UB_Fatfs_OpenFile(&myFile, "0:/UB_File.txt", F_WR_NEW)==FATFS_OK) {
//	    	// ein paar Textzeilen in das File schreiben
//	        UB_Fatfs_WriteString(&myFile,"Test der WriteString-Funktion");
//	        UB_Fatfs_WriteString(&myFile,"hier Zeile wacho");
//	        UB_Fatfs_WriteString(&myFile,"ENDE");
//	        // File schliessen
//	        UB_Fatfs_CloseFile(&myFile);
//	        GPIO_SetBits(GPIOD,GPIO_Pin_14);
//	      }
//	      // Media unmounten
//	   	  UB_Fatfs_UnMount(MMC_0);
//	    }
//	  }else{
//			GPIO_SetBits(GPIOD,GPIO_Pin_15);
//	  }

	  tarjeta_sd.flag.fallo_abrir_archivo = 0;
	  cargar_datos(); // Se cargan los datos por default.

	while(1){
		task_manager();
	}
}

void task_scheduler(void){
	/*	Funcion task_scheduler()
	 *	Maneja los contadores de las distintas tareas. Esta funcion es llamada por el systick cada
	 *	vez que se vence su contador (1ms en este caso).
	 *	Cuando el ontador de una tarea llega a su, fin se dispara una bandera de evento.
	 *	Este evento es chequeado luego de la interrupcion generada por el systick en el main().
	 *	Puede haber dos tipos de tareas, ciclicas o no ciclicas. En las ciclicas el
	 *	contador se resetea cada vez q una tarea finaliza, en las no ciclicas el contador
	 *	se setea solo cuando es necesario.
	 */

	//	// Rutina DHT
		if(dht_interior.flag){
			if(dht_interior.timeout >= 1){
				dht_interior.timeout--;
			}

			if(dht_interior.timeout <= 0){
				dht_interior.flag_timeout = 1;
			}
		}

		if(dht_interior.contador >= 1){
			dht_interior.contador--;
		}
		else{
			dht_interior.flag = 1;
			dht_interior.contador = 2000;
			dht_interior.flag_timeout = 0;
			dht_interior.timeout = 1000; // Se le agrega un tiempo de timeout para esperar la funcion DHT11Start.
		}

		if(dht_exterior.flag){
			if(dht_exterior.timeout >= 1){
				dht_exterior.timeout--;
			}

			if(dht_exterior.timeout <= 0){
				dht_exterior.flag_timeout = 1;
			}
		}

		if(dht_exterior.contador >= 1){
			dht_exterior.contador--;
		}
		else{
			dht_exterior.flag = 1;
			dht_exterior.contador = 2000;
			dht_exterior.flag_timeout = 0;
			dht_exterior.timeout = 1000; // Se le agrega un tiempo de timeout para esperar la funcion DHT11Start.
		}
	//
		// Rutina LDR
		if(ldr.contador >= 1){
			ldr.contador--;
		}
		else{
			ldr.flag.fin_contador = 1;
			ldr.contador = 100;
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
		led.flag.fin_contador = (led.flag.fin_contador) ? 0 : 1;
		led.contador = contador_led;
	}

	// Rutina display.
	if(display.contador >= 1){
		display.contador--;
	}
	else{
		display.flag = 1;
		display.contador = 2000;
	}

	// Rutina Serial.
	if(serial.timeout >= 1){
		serial.timeout--;
	}
	else if(serial.flag.comienzo_paquete){
		serial.flag.comienzo_paquete = 0;
		enviar_comando(":TMO,-,-!");
	}

	// Contador para el modo monitor
	if(serial.contador_task >= 1){
		serial.contador_task--;
	}
	else if(sistema.flag.modo_monitor_serial){
		serial.contador_task = 1000;
		serial.flag.mostrar_valores = 1;
	}

//	if(cultivo.contador_aux >= 1){
//		cultivo.contador_aux--;
//	}
//	else{
//		cultivo.flag.fin_contador = 1;
//		cultivo.contador_aux = 10e3;
//	}
}

void inicializar_leds(){
	/*	Funcion inicializar_leds()
	 *	No recive ni devuelve un valor.
	 *	Se inicializan los 4 leds de la placa Discovery STM32F4.
	 */

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

void inicializar_fan(){
	/*	Funcion inicializar_leds()
	 *	No recive ni devuelve un valor.
	 *	Se inicializan los 4 leds de la placa Discovery STM32F4.
	 */

	GPIO_InitTypeDef GPIO_Init_Pins; // Estrucura de datos para configurar el GPIO

	//
	//Inicializacion de los leds.
	//

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	GPIO_Init_Pins.GPIO_Pin= GPIO_Pin_14;
	GPIO_Init_Pins.GPIO_Mode=GPIO_Mode_OUT ;
	GPIO_Init_Pins.GPIO_Speed= GPIO_Speed_100MHz;
	GPIO_Init_Pins.GPIO_OType= GPIO_OType_PP ;
	GPIO_Init_Pins.GPIO_PuPd= GPIO_PuPd_NOPULL;

	GPIO_Init(GPIOD,&GPIO_Init_Pins); // Carga de la estrucura de datos.

}

void inicializar_calentador(){
	/*	Funcion inicializar_leds()
		 *	No recive ni devuelve un valor.
		 *	Se inicializan los 4 leds de la placa Discovery STM32F4.
		 */

		GPIO_InitTypeDef GPIO_Init_Pins; // Estrucura de datos para configurar el GPIO

		//
		//Inicializacion de los leds.
		//

		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

		GPIO_Init_Pins.GPIO_Pin= GPIO_Pin_4;
		GPIO_Init_Pins.GPIO_Mode=GPIO_Mode_OUT ;
		GPIO_Init_Pins.GPIO_Speed= GPIO_Speed_100MHz;
		GPIO_Init_Pins.GPIO_OType= GPIO_OType_PP ;
		GPIO_Init_Pins.GPIO_PuPd= GPIO_PuPd_NOPULL;

		GPIO_Init(GPIOC,&GPIO_Init_Pins); // Carga de la estrucura de datos.

}

void inicializar_bomba(){
	/*	Funcion inicializar_leds()
		 *	No recive ni devuelve un valor.
		 *	Se inicializan los 4 leds de la placa Discovery STM32F4.
		 */

		GPIO_InitTypeDef GPIO_Init_Pins; // Estrucura de datos para configurar el GPIO

		//
		//Inicializacion de los leds.
		//

		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

		GPIO_Init_Pins.GPIO_Pin= GPIO_Pin_5;
		GPIO_Init_Pins.GPIO_Mode=GPIO_Mode_OUT ;
		GPIO_Init_Pins.GPIO_Speed= GPIO_Speed_100MHz;
		GPIO_Init_Pins.GPIO_OType= GPIO_OType_PP ;
		GPIO_Init_Pins.GPIO_PuPd= GPIO_PuPd_NOPULL;

		GPIO_Init(GPIOC,&GPIO_Init_Pins); // Carga de la estrucura de datos.

}

void task_manager(void){
	/*	Funcion task_manager
	 *	No recive ni devuelve un valor.
	 * 	Se encarga de llamar a las distintas tareas del programa.
	 *	Estas tareas se realizan o no dependiendo de su flag de evento.
	 */

	// Lectura de datos
	dht_interior_task();
	dht_exterior_task();
	ldr_task();
	yl69_task();

	serial_task();
	// Manejo de la interfaz de usuario.
	display_task();

	// Otras tareas.
	if(!sistema.flag.conexion_serial){ // Se realizan las tareas si no se esta conectado al serial
		led_task();

		// Si se cambia de etapa se cambia el archivo de log.
		if(cultivo.flag.cambio_etapa){
			log_etapas();
			cultivo.flag.cambio_etapa = 0;
		}
	}



//	// Si hay una plantacion activa, se controla la temperatura.
//	if(cultivo.flag.fin_contador){
//		check_cultivo_task();
//		cultivo.flag.fin_contador = 0;
//	}
//
//	if(cultivo.flag.control_activo){
//		control_temp_task();
//	}
}

void dht_interior_task(){
	/*	Funcion dht_interior_task()
	 *	No recive ni devuelve un valor.
	 *	Tarea que se encarga de leer los datos del sensor
	 *	DHT11 de temperatura interior.
	 */
	if(dht_interior.flag){
		leer_dht_interior();
		dht_interior.flag = 0;
	}
}

void dht_exterior_task(){
	/*	Funcion dht_exterior_task()
	 *	No recive ni devuelve un valor.
	 *	Tarea que se encarga de leer los datos del sensor
	 *	DHT11 de temperatura eterior.
	 */
	if(dht_exterior.flag){
		leer_dht_exterior();
		dht_exterior.flag = 0;
	}
}

void led_task(){
	/*	Funcion led_task()
	 *	No recive ni devuelve un valor.
	 *	Tarea que se concentra en el parpadeo
	 *	de un led de control de programa.
	 */
	switch(led.flag.fin_contador){
	case 0: GPIO_ResetBits(GPIOD,GPIO_Pin_13);break;
	case 1:	GPIO_SetBits(GPIOD,GPIO_Pin_13);break;
	default:GPIO_ResetBits(GPIOD,GPIO_Pin_13);break;
	}
}

void ldr_task(){
	/*	Funcion ldr_task()
	 *	No recive ni devuelve un valor.
	 *	Tarea que lee un valor de ADC establecido en adc.h.
	 */

	if(ldr.flag.fin_contador){
		ldr.contador_promedio++; 

		if(ldr.contador_promedio <= 10){
			ldr.adc_cuentas_temp += adc_leer_cuentas_ldr(); // Se agrega el valor leido a la suma del promedio.
		}
		else{
			ldr.adc_cuentas = ldr.adc_cuentas_temp/10; // Se obtiene el promedio.
			ldr.contador_promedio = 0; // Reset del contador.
			ldr.adc_cuentas_temp = 0; // Reset de la suma del promedio.
		}
		ldr.flag.fin_contador = 0;
	}
}

void yl69_task(){
	/*	Funcion yl_69_task()
	 *	No recive ni devuelve un valor.
	 *	Tarea que lee un valor de ADC establecido en adc.h
	 */
	if(yl.flag){
		yl.adc_cuentas = adc_leer_cuentas_yl69();
		yl.flag = 0;
	}
}

void display_task(){
	/*	Funcion display_task()
	 *	No recive ni devuelve un valor.
	 *	Tarea que lee un valor de ADC establecido en adc.h
	 */
	char buffer[16];
	TM_RTC_GetDateTime(&datatime, TM_RTC_Format_BIN); // Se obtiene la hora actual.
	if(display.flag){

		if(!sistema.flag.conexion_serial){
//			char buffer[16];

			if(display.estado <1){
				display.estado++;
			}
			else{
				display.estado = 0;
			}

			switch(display.estado){ // Se muestran distintos carteles cada vez que se vence el contador del display.
			case 0:{
				if(cultivo.flag.control_activo){
					sprintf(buffer,"Etapa: %d       ", cultivo.etapa_actual);
					UB_LCD_2x16_String(0,0,buffer);
					UB_LCD_2x16_String(0,1,cultivo.nombre);
					display.flag = 0;
				}
				else{
					UB_LCD_2x16_String(0,0,"Sin cultivo     ");
					UB_LCD_2x16_String(0,1,"                ");
					display.flag = 0;
				}
			};break;
			case 1:{

				sprintf(buffer,"    %d:%d       ",datatime.hours,datatime.minutes);
				UB_LCD_2x16_String(0,0,buffer);
				UB_LCD_2x16_String(0,1,"                ");
				display.flag = 0;

			};break;
			default:{
				UB_LCD_2x16_String(0,0,"Display-fail");
				UB_LCD_2x16_String(0,1,"            ");
				display.flag = 0;
			};break;
			}
			display.flag = 0;
		}
		else{
			UB_LCD_2x16_String(0,0,"Conexion        ");
			UB_LCD_2x16_String(0,1,"PC              ");
			display.flag = 0;
		}

	}
}

void control_temp_task(){
	/*	Control de temperatura
	 * 	Esta funcion controla la temperatura interna del invernadero.
	 */

	if(abs(dht_exterior.temperatura - dht_interior.temperatura) < control.limite_delta_temp){
		// Si se tiene una diferencia de temperatura razonable entre el interior y el exterior se puede abrir la ventana.

		if(dht_interior.temperatura >= control.max_temp_fan){
			GPIO_SetBits(GPIOD,GPIO_Pin_14); // Se enciende el fan y la ventana.
		}
		else if(dht_exterior.temperatura <= control.min_temp_fan){
			GPIO_ResetBits(GPIOD,GPIO_Pin_14);
		}

		if(dht_interior.temperatura <= control.max_temp_calentador){
			GPIO_SetBits(GPIOD,GPIO_Pin_12); // Se enciende el calentador.
		}
		else if(dht_exterior.temperatura >= control.min_temp_calentador){
			GPIO_ResetBits(GPIOD,GPIO_Pin_12);
		}

	}
	else{
		// Si no se tiene una diferencia de temperatura razonable, es mejor no abrir la ventana.
		GPIO_ResetBits(GPIOD,GPIO_Pin_14 | GPIO_Pin_12);
	}
}

// Tarea de serial.
void serial_task(void){
	/*	Funcion serial_task()
	 * 	Se encarga de manejar los paquetes que van ingresando al MCU.
	 * 	Llama a las distintas funciones que se encargan de ejecutar cada comando.
	 */
	char msj[50] = "";

	if(serial.flag.fin_paquete){
		if(!strcmp(serial.comando,"STR") && sistema.flag.conexion_serial){
			GPIO_ToggleBits(GPIOD,GPIO_Pin_14);
			GPIO_ToggleBits(GPIOC,GPIO_Pin_4 |GPIO_Pin_5);
			enviar_comando(":OKK,-,-!\r\n");
		}
		if(!strcmp(serial.comando,"ATR") && sistema.flag.conexion_serial){
			enviar_comando(":OKK,-,-!");
			enviar_comando(" DHT: ");
			enviar_comando(dht_interior.temperatura_string);
		}
		if(!strcmp(serial.comando,"SKR") && sistema.flag.conexion_serial){
			// Muestra si se pudo leer la tarjeta SD.
			enviar_comando(":OKK,-,-! ");
			if(tarjeta_sd.flag.fallo_abrir_archivo){
				enviar_comando(":SD,4,FAIL!\r\n");
			}
			else{
				enviar_comando(":SD,2,OK!");
				sprintf(msj,"NOMBRE: %s.ESTADO_CULTIVO: %d.ETAPA_CULTIVO: %d.",cultivo.nombre,cultivo.flag.control_activo,cultivo.etapa_actual);
				enviar_comando(msj);
				enviar_comando("\r\n");
			}
		}
		if(!strcmp(serial.comando,"INI") && !sistema.flag.conexion_serial){
			sistema.flag.conexion_serial = 1;
			enviar_comando(":OKK,-,-!\r\n");
		}
		if(!strcmp(serial.comando,"STP") && sistema.flag.conexion_serial){
			sistema.flag.conexion_serial = 0;
			sistema.flag.modo_monitor_serial = 0;
			serial.flag.mostrar_valores = 0;
			enviar_comando(":OKK,-,-!\r\n");
		}
		if(!strcmp(serial.comando,"LED") && sistema.flag.conexion_serial){
			contador_led = atoi(serial.datos);
			enviar_comando(":OKK,-,-!\r\n");
		}
		if(!strcmp(serial.comando,"DAT") && sistema.flag.conexion_serial){
			cargar_datos();
			enviar_comando(":OKK,-,-!\r\n");
		}
		if(!strcmp(serial.comando,"TOM") && sistema.flag.conexion_serial){
			enviar_comando(":OKK,-,-!\r\n");
			configurar_cultivo_tomate();
		}
		if(!strcmp(serial.comando,"LOG") && sistema.flag.conexion_serial){
			enviar_comando(":OKK,-,-!\r\n");
			log_etapas();
		}
		// Comado SDC SD card Coneccted, se chequea si se puede montar la tarjeta.
		if(!strcmp(serial.comando,"SDC") && sistema.flag.conexion_serial){
			enviar_comando(":OKK,-,-! ");
			if(UB_Fatfs_Mount(MMC_0)==FATFS_OK){
				enviar_comando(":SDC,8,Presente!\r\n");
				UB_Fatfs_UnMount(MMC_0);
			}
			else{
				enviar_comando(":SDC,8,Ausente!\r\n");
			}

		}
		if(!strcmp(serial.comando,"MON") && !strcmp(serial.datos,"ON") && sistema.flag.conexion_serial){
			sistema.flag.modo_monitor_serial = 1;
			enviar_comando(":OKK,-,-!\r\n");
		}
		if(!strcmp(serial.comando,"MON") && !strcmp(serial.datos,"OFF") && sistema.flag.conexion_serial){
			sistema.flag.modo_monitor_serial = 0;
			enviar_comando(":OKK,-,-!\r\n");
		}
	}
	serial.flag.fin_paquete = 0;

	if(sistema.flag.modo_monitor_serial && serial.flag.mostrar_valores){ // Si se esta en modo monitor se muestra el valor cada vez q se
		TM_RTC_GetDateTime(&datatime, TM_RTC_Format_BIN); // Se obtiene la hora actual.
		sprintf(msj,"%d:%d:%d",datatime.hours,datatime.minutes,datatime.seconds);
		enviar_comando(msj);
		enviar_comando(" DHTI: ");										//	vence el contador.
		enviar_comando(dht_interior.temperatura_string);
		enviar_comando(" DHTE: ");										//	vence el contador.
		enviar_comando(dht_exterior.temperatura_string);
		sprintf(msj," LDR: %d", ldr.adc_cuentas);
		enviar_comando(msj);
		sprintf(msj," YL-69: %d\r\n", yl.adc_cuentas);
		enviar_comando(msj);
		serial.flag.mostrar_valores = 0;
	}
}

void cargar_datos(){
	/*	Funcion cargar_datos()
	 * 	Esta funcion carga los datos por defecto al sistema de control.
	 * 	De momento los valores se toman de unos arreglos ya predefinidos, la idea es que sean leidos por memoria externa.
	 */
	FIL myFile;   // Filehandler
	char temp_string[53];
	char *temp;
	uint8_t estado_cultivo = -1;
	uint8_t etapa_actual = -1;
	uint8_t tipo_cultivo = -1;


	//Temperaturas por defecto.
	control.limite_delta_temp = 5;
	control.max_temp_fan = 26;
	control.min_temp_fan = 25;
	control.max_temp_calentador = 18;
	control.min_temp_calentador = 17;
	control.flag.bomba_encendida = 0;
	control.flag.fan_encendido = 0;
	control.flag.calentador_encendido = 0;

	// leer sd.
	if(UB_Fatfs_CheckMedia(MMC_0)==FATFS_OK) {
		//GPIO_SetBits(GPIOD,GPIO_Pin_13);
		// Media mounten
		if(UB_Fatfs_Mount(MMC_0)==FATFS_OK) {
			//GPIO_SetBits(GPIOD,GPIO_Pin_12);
			// File zum schreiben im root neu anlegen
			if(UB_Fatfs_OpenFile(&myFile, "0:/UB_File.txt", F_RD)==FATFS_OK) {
				// ein paar Textzeilen in das File schreiben
				if(UB_Fatfs_ReadString(&myFile,temp_string,52) == FATFS_OK){
					tarjeta_sd.flag.fallo_abrir_archivo = 0;
					//strcpy(tarjeta_sd.lectura,temp_string);

					///////////////Codigo que interpreta el texto////////////
					temp = strtok(temp_string," ,.-");

					while(temp != NULL){
						if(!strcmp("CULTIVO_ACTIVO",temp)){
							temp = strtok(NULL," ,.-");
							estado_cultivo = atoi(temp);
						}
						else if(!strcmp("ETAPA_CULTIVO",temp)){
							temp = strtok(NULL," ,.-");
							etapa_actual = atoi(temp);
						}
						else if(!strcmp("TIPO_CULTIVO",temp)){
							temp = strtok(NULL," ,.-");
							tipo_cultivo = atoi(temp);
						}
						temp = strtok(NULL," ,.-");

					}
					/////////////////////////////////////////////////////////
				}
				else{
					tarjeta_sd.flag.fallo_abrir_archivo = 1;
				}
				// File schliessen
				UB_Fatfs_CloseFile(&myFile);
			}
			// Media unmounten
			UB_Fatfs_UnMount(MMC_0);
		}
	}

	 //cultivo activo?
	if(!tarjeta_sd.flag.fallo_abrir_archivo && (estado_cultivo == -1 || etapa_actual == -1)){ // Se comprueba que los datos hayan sido leidos.
		// Se manda el msj de error.
		tarjeta_sd.flag.fallo_abrir_archivo = 1;

	}
	else if(!tarjeta_sd.flag.fallo_abrir_archivo && (estado_cultivo != -1 && etapa_actual != -1)){
		cultivo.flag.control_activo = estado_cultivo;
		cultivo.etapa_actual = etapa_actual;
		switch(tipo_cultivo){
			case 0:strcpy(cultivo.nombre,"TOMATE");;break;
			case 1:strcpy(cultivo.nombre,"MORRON");;break;
			case 2:strcpy(cultivo.nombre,"CUSTOM");;break;
			default:strcpy(cultivo.nombre,"");;break;
		}

//		if(!strcmp(nombre_cultivo,"TOMATE")){
//			//Configurar tomate
//			strcpy(cultivo.nombre,nombre_cultivo);
//		}
//		else if(!strcmp(nombre_cultivo,"MORRON")){
//			//Configurar morron
//			strcpy(cultivo.nombre,nombre_cultivo);
//		}
//		else if(!strcmp(nombre_cultivo,"CUSTOM")){
//			//Configurar custom
//			strcpy(cultivo.nombre,nombre_cultivo);
//		}
	}
}
	// cual?

	//configurar_cultivo_tomate();


int fecha_valida(TM_RTC_t fecha_control){
	/*	Funcion fecha_valida()
	 * 	Funcion encargada de retornar si la fecha ingresada en una etapa es valida o no.
	 * 	Si la fecha se encuentra en el pasado es rechazada, si es del futuro se acepta.
	 */

	if(fecha_control.year < datatime.year){
		return 0;
	}
	else if(fecha_control.month < datatime.month){
		return 0;
	}
	else if(fecha_control.date < datatime.date){
		return 0;
	}
	else if(fecha_control.hours < datatime.hours){
		return 0;
	}
	else if(fecha_control.minutes < datatime.minutes){
		return 0;
	}
	else
		return 1;
}

void configurar_cultivo_tomate(void){
	/*	Funcion configurar_cultivo_tomate()
	 * 	Funcion que inicializa los valores de control de cultivo.
	 *	Se configuran las fechas de las etapas y las variables objetivo para cada una de ellas.
	 *	Ademas se comprueba que las fechas ingresadas sean correctas, si todas lo son se activa el seguimiento.
	 */

//	uint8_t i,n_etapas_validas = 0;
//	char msj[30] = "";

	TM_RTC_GetDateTime(&datatime, TM_RTC_Format_BIN); // Se obtiene la hora actual.

	//
	//Se fijan las alarmas y luego se las guarda en la tarjeta SD como backup.
	//
	cultivo.etapa[0].hours = datatime.hours;
	cultivo.etapa[0].minutes = datatime.minutes+1;
	cultivo.etapa[0].seconds = datatime.seconds;
	cultivo.etapa[0].day = datatime.day;
	// Se asegura que la fecha sea valida.
//	if((datatime.day + 7) > TM_RTC_GetDaysInMonth(datatime.month,datatime.year) ){
//		cultivo.etapa[0].day = 7 - (TM_RTC_GetDaysInMonth(datatime.month,datatime.year) - datatime.day );
//	}
//	else
//		cultivo.etapa[0].day = datatime.day + 7;
	cultivo.etapa[0].alarmtype = TM_RTC_AlarmType_DayInMonth;

	cultivo.etapa[1].hours = datatime.hours;
	cultivo.etapa[1].minutes = datatime.minutes+2;
	cultivo.etapa[1].seconds = datatime.seconds;
	// Se asegura que la fecha sea valida.
//	if((datatime.day + 14) > TM_RTC_GetDaysInMonth(datatime.month,datatime.year) ){
//		cultivo.etapa[1].day = 14 - (TM_RTC_GetDaysInMonth(datatime.month,datatime.year) - datatime.day );
//	}
//	else
//		cultivo.etapa[1].day = datatime.day + 14;
	cultivo.etapa[1].day = datatime.day;
	cultivo.etapa[1].alarmtype = TM_RTC_AlarmType_DayInMonth;

	cultivo.etapa[2].hours = datatime.hours;
	cultivo.etapa[2].minutes = datatime.minutes+3;
	cultivo.etapa[2].seconds = datatime.seconds;
	cultivo.etapa[2].day = datatime.day;
	// Se asegura que la fecha sea valida.
//	if((datatime.day + 21) > TM_RTC_GetDaysInMonth(datatime.month,datatime.year) ){
//		cultivo.etapa[2].day = 21 - (TM_RTC_GetDaysInMonth(datatime.month,datatime.year) - datatime.day );
//	}
//	else
//		cultivo.etapa[2].day = datatime.day + 21;
	cultivo.etapa[2].alarmtype = TM_RTC_AlarmType_DayInMonth;

	//
	//  Se guardan los valores de las etapas en la SD
	//
	enviar_comando("C\r\n");
	backup_etapas(); // Se guardan los valoers de las etapas en la SD.
	enviar_comando("F\r\n");

	//
	//  Se cofigura la primer alarma.
	//
	TM_RTC_GetDateTime(&datatime, TM_RTC_Format_BIN); // Se obtiene la hora actual.
	cultivo.etapa_actual = 0;
	AlarmTime.hours = cultivo.etapa[0].hours;
	AlarmTime.minutes = cultivo.etapa[0].minutes;
	AlarmTime.seconds = cultivo.etapa[0].seconds;
	AlarmTime.day = cultivo.etapa[0].day;
	AlarmTime.alarmtype = TM_RTC_AlarmType_DayInMonth;
	TM_RTC_SetAlarm(TM_RTC_Alarm_B, &AlarmTime, TM_RTC_Format_BIN);

	// Variables del cultivo para cada etapa.
	cultivo.variables[0].limite_delta_temp = 3;
	cultivo.variables[0].max_temp_calentador = 20 ;
	cultivo.variables[0].max_temp_fan = 23;
	cultivo.variables[0].min_temp_calentador = 18;
	cultivo.variables[0].min_temp_fan = 21;

	cultivo.variables[1].limite_delta_temp = 2;
	cultivo.variables[1].max_temp_calentador = 19 ;
	cultivo.variables[1].max_temp_fan = 22;
	cultivo.variables[1].min_temp_calentador = 17;
	cultivo.variables[1].min_temp_fan = 20;

	cultivo.variables[2].limite_delta_temp = 2;
	cultivo.variables[2].max_temp_calentador = 19 ;
	cultivo.variables[2].max_temp_fan = 22;
	cultivo.variables[2].min_temp_calentador = 17;
	cultivo.variables[2].min_temp_fan = 20;
//
	strcpy(cultivo.nombre,"Tomate");
	cultivo.tipo = 0;

	cultivo.flag.control_activo = 1;
	cultivo.flag.cambio_etapa = 1;
//
//	TM_RTC_GetDateTime(&datatime, TM_RTC_Format_BIN); // Se obtiene la hora actual.
//
//	// Se comprueba que todas las etapas sean validas, es decir que no tengan una fecha anterior a la actual.
//	for(i=0;i<3;i++){
//		if(fecha_valida(cultivo.etapa[i]))
//			n_etapas_validas++;
//	}
//
//	if(n_etapas_validas == 3){
//		// Si el numero de etapas validas es el total se activa el control.
//		cultivo.flag.control_activo = 1;
//	}
//	else
//		cultivo.flag.control_activo = 0;
}

// Codigo para USART.
void enviar_comando(char *cmd){
	/*	Funcion enviar_comando
	 * Envia una cadena a traves del USART3
	 */
	uint16_t n = strlen(cmd);
	uint16_t i;
	for(i=0;i<n;i++){
		USART_SendData(USART3, cmd[i]); // Se envia el dato que ingreso.
		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET); // Se espera a que termine de enviar (NOTAR QUE DICE FLAG).
	}
}

static void USART3_Config(void)
{
	/*	Funcion USART3_Config
	 * 	Configura los pines necesarios para la conexion a traves de RS232.
	 * 	Ademas inicializa las interrupciones para el USART3 en modo recepcion.
	 */
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	/* USART IOs configuration ***********************************/
	/* Enable GPIOC clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	/* Connect PC10 to USART3_Tx */
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_USART3);
	/* Connect PC11 to USART3_Rx*/
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_USART3);
	/* Configure USART3_Tx and USART3_Rx as alternate function */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // estaba en 50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	/* USART configuration ***************************************/
	/* USART3 configured as follow:
- BaudRate = 115200 baud
- Word Length = 8 Bits
- One Stop Bit
- No parity
- Hardware flow control disabled (RTS and CTS signals)
- Receive and transmit enabled
	 */
	/* Enable USART3 clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3, &USART_InitStructure);
	/* Enable USART3 */
	USART_Cmd(USART3, ENABLE);

	//USART1 is more important than USART2, so it has lower sub priority number
	NVIC_InitTypeDef NVIC_InitStruct;

	NVIC_InitStruct.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 10;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStruct);

	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); // Se habilitan las interrupciones cuando se recibe un dato.
}

void activar_alarma(void){
	TM_RTC_GetDateTime(&datatime, TM_RTC_Format_BIN); // Se obtiene la hora actual.
	/* Set alarm B each 20th day in a month */
	/* Alarm will be first triggered 10 seconds later as time is configured for RTC */
	AlarmTime.hours = datatime.hours;
	AlarmTime.minutes = datatime.minutes+1;
	AlarmTime.seconds = datatime.seconds;
	AlarmTime.day = datatime.day;
	AlarmTime.alarmtype = TM_RTC_AlarmType_DayInMonth;
	TM_RTC_SetAlarm(TM_RTC_Alarm_B, &AlarmTime, TM_RTC_Format_BIN);
}

void backup_etapas(void){
	FIL myFile;   // Filehandler
	char temp_string[3][100];

	// Se guardan los horarios de las etapas.
	sprintf(temp_string[0],"-,ETAPA 0, HOURS %d, MINUTES %d, SECONDS %d, DAY %d",
			cultivo.etapa[0].hours,cultivo.etapa[0].minutes,cultivo.etapa[0].seconds, cultivo.etapa[0].day);
	sprintf(temp_string[1],",ETAPA 1, HOURS %d, MINUTES %d, SECONDS %d, DAY %d",
				cultivo.etapa[1].hours,cultivo.etapa[1].minutes,cultivo.etapa[1].seconds, cultivo.etapa[1].day);
	sprintf(temp_string[2],",ETAPA 2, HOURS %d, MINUTES %d, SECONDS %d, DAY %d.",
				cultivo.etapa[2].hours,cultivo.etapa[2].minutes,cultivo.etapa[2].seconds, cultivo.etapa[2].day);
	//sprintf(temp_string,"%d", cultivo.etapa.hours[1]);

	// Check ob Medium eingelegt ist
	if(UB_Fatfs_CheckMedia(MMC_0)==FATFS_OK) {
		//GPIO_SetBits(GPIOD,GPIO_Pin_13);
		// Media mounten
		if(UB_Fatfs_Mount(MMC_0)==FATFS_OK) {
			//GPIO_SetBits(GPIOD,GPIO_Pin_12);
			// File zum schreiben im root neu anlegen
			if(UB_Fatfs_OpenFile(&myFile, "0:/ETAPAS.txt", F_WR_CLEAR)==FATFS_OK) {
				// ein paar Textzeilen in das File schreiben
				UB_Fatfs_WriteString(&myFile,temp_string[0]);
				UB_Fatfs_WriteString(&myFile,temp_string[1]);
				UB_Fatfs_WriteString(&myFile,temp_string[2]);
				// File schliessen
				UB_Fatfs_CloseFile(&myFile);
				GPIO_SetBits(GPIOD,GPIO_Pin_14);
			}
			// Media unmounten
			UB_Fatfs_UnMount(MMC_0);
		}
	}else{
		GPIO_SetBits(GPIOD,GPIO_Pin_15);
	}

}

void log_etapas(void){
	FIL myFile;   // Filehandler
	char temp_string[70];
	enviar_comando("ADENTRO\r\n");
	sprintf(temp_string,"-,CULTIVO_ACTIVO %d, ETAPA_CULTIVO %d, TIPO_CULTIVO %d.",cultivo.flag.control_activo,cultivo.etapa_actual, cultivo.tipo);

	// Check ob Medium eingelegt ist
	if(UB_Fatfs_CheckMedia(MMC_0)==FATFS_OK) {
		//GPIO_SetBits(GPIOD,GPIO_Pin_13);
		// Media mounten
		if(UB_Fatfs_Mount(MMC_0)==FATFS_OK) {
			//GPIO_SetBits(GPIOD,GPIO_Pin_12);
			// File zum schreiben im root neu anlegen
			if(UB_Fatfs_OpenFile(&myFile, "0:/Log.txt", F_WR_CLEAR)==FATFS_OK) {
				// ein paar Textzeilen in das File schreiben
				UB_Fatfs_WriteString(&myFile,temp_string);
				// File schliessen
				UB_Fatfs_CloseFile(&myFile);
				enviar_comando("CHLOG\r\n");
				GPIO_SetBits(GPIOD,GPIO_Pin_15);
			}
			else{
				enviar_comando("FALLO\r\n");
			}
			// Media unmounten
			UB_Fatfs_UnMount(MMC_0);
		}
	}else{
		GPIO_SetBits(GPIOD,GPIO_Pin_12);
	}
}

void USART3_IRQHandler(void) {
	/*	Handler USART3_IRQHandler
	 * 	Se encarga de recibir los paquetes a traves del puerto serie.
	 * 	Se asegura que cada paquete tenga su formato correcto. Se esperan caracteres hasta 100ms,
	 * 	despues de este tiempo se toma como un fallo de recepcion.
	 */

	if (USART_GetITStatus(USART3, USART_IT_RXNE)) { // Se limpia la bandera de interrupcion (NOTAR QUE DICE IT)

		serial.aux = USART_ReceiveData(USART3); // Copia el utlimo dato que ingreso.

		if(serial.aux == ':' && !serial.flag.comienzo_paquete){ // Se guarda un paquete si comienza un mensaje con :
			serial.timeout = 100; // Se setea el tiempo maximo de espera.
			serial.contador = 0; // Se reinicia el contador.
			serial.flag.comienzo_paquete = 1; // Se informa el comienzo del paquete.
			serial.flag.fin_paquete = 0; // Se limpia la bandera de fin de paquete.
			strcpy(serial.buffer,""); // Limpia la string.
			strcpy(serial.cant_bytes,""); // Limpia la string.
			strcpy(serial.comando,""); // Limpia la string.
			strcpy(serial.datos,""); // Limpia la string.
		}
		else if((serial.aux == '!' && serial.flag.comienzo_paquete) || serial.contador == 256){ // Se termina paquete con !

			if(!serial.flag.fin_comando || !serial.flag.fin_cantbytes ){
				enviar_comando(":ERR,10,BAD_FORMAT!");
				serial.contador = 0; // Se reinica el contador.
				serial.flag.comienzo_paquete = 0; // Se reinicia la bandera.
				serial.flag.fin_comando = 0; // Se reinicia la bandera.
				serial.flag.fin_cantbytes = 0; // Se reinicia la bandera.
				serial.flag.fin_datos = 0; // Se reinicia la bandera.
				serial.flag.fin_paquete = 0; // Se informa el paquete disponible.
				serial.timeout = 0; // Se detiene el contador de tiempo de espera.
			}
			else{
				strncpy(serial.datos,serial.buffer,256); // Se guardan los datos recibidos.
				strncpy(serial.buffer,"",17); // Se limpia el buffer.
				serial.flag.fin_datos = 1; // Se informa que los datos estan disponibles.
				serial.contador = 0; // Se reinica el contador.
				serial.flag.comienzo_paquete = 0; // Se reinicia la bandera.
				serial.flag.fin_comando = 0; // Se reinicia la bandera.
				serial.flag.fin_cantbytes = 0; // Se reinicia la bandera.
				serial.flag.fin_datos = 0; // Se reinicia la bandera.
				serial.flag.fin_paquete = 1; // Se informa el paquete disponible.
				serial.timeout = 0; // Se detiene el contador de tiempo de espera.
			}
		}
		else if((serial.aux == ',' ) && serial.flag.comienzo_paquete){ // Se detecta el fin de campo.
			serial.timeout = 100; // Se setea el tiempo maximo de espera.
			if(!serial.flag.fin_comando && !serial.flag.fin_cantbytes && !serial.flag.fin_datos){ // Se detecta si es la parte de comando.
				strncpy(serial.comando,serial.buffer,17); // Se guarda el comando.
				strncpy(serial.buffer,"",17); // Se limpia el buffer.
				serial.flag.fin_comando = 1; // Se informa la llegada del comando.
			}
			else if(serial.flag.fin_comando && !serial.flag.fin_cantbytes && !serial.flag.fin_datos){ // Se detecta si es la parte de cantidad de bytes.
				strncpy(serial.cant_bytes,serial.buffer,17); // Se guarda la cantidad de bytes.
				strncpy(serial.buffer,"",17); // Se limpia el buffer.
				serial.flag.fin_cantbytes = 1;  // Se informa la llegada de la cantidad de bytes.
			}
			serial.contador = 0; // Se reinica el contador para guardar el proximo campo.
		}
		else if((serial.aux != ':' && serial.aux != '!' ) && serial.flag.comienzo_paquete){ // Se agrega un caracter al buffer.
			serial.timeout = 100; // Se setea el tiempo maximo de espera.
			serial.buffer[serial.contador] = serial.aux; // Se guarda el caracter.
			serial.contador++;
		}

		//Clear interrupt flag
		USART_ClearITPendingBit(USART3, USART_IT_RXNE); // Se limpia la bandera de interrupcion (NOTAR QUE DICE IT)
	}
}

void TM_RTC_AlarmBHandler(void) {
	/* If user needs this function, then they should be defined separatelly in your project */
	if(cultivo.etapa_actual < 2){
		enviar_comando("Cambiando etapa...\r\n");
		cultivo.etapa_actual++;
		cultivo.flag.cambio_etapa = 1; // Se avisa el cambio de etapa para cambiar el archivo de log.
		AlarmTime.hours = cultivo.etapa[cultivo.etapa_actual].hours;
		AlarmTime.minutes = cultivo.etapa[cultivo.etapa_actual].minutes;
		AlarmTime.seconds = cultivo.etapa[cultivo.etapa_actual].seconds;
		AlarmTime.day = cultivo.etapa[cultivo.etapa_actual].day;
		AlarmTime.alarmtype = TM_RTC_AlarmType_DayInMonth;
		TM_RTC_SetAlarm(TM_RTC_Alarm_B, &AlarmTime, TM_RTC_Format_BIN);
	}
	else{
		/* Set RTC alarm B, time in binary format */
		cultivo.etapa_actual = 0;
		cultivo.flag.control_activo = 0;
		cultivo.flag.cambio_etapa = 1; // Se avisa el cambio de etapa para cambiar el archivo de log.
		enviar_comando("ALARMA B DESCTIVADA\r\n");
		TM_RTC_DisableAlarm(TM_RTC_Alarm_B);
	}
	/* Disable Alarm so it will not trigger next month at the same date and time */
		//TM_RTC_DisableAlarm(TM_RTC_Alarm_B);
}




