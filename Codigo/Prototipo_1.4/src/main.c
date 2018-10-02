#include "main.h"

int main(void){

	/*	PP3 Ejercicio 1
	 *	Se debe mostrar un cartel, en un display LCD.
	 *
	 */
	UB_LCD_2x16_Init(); // Inicializacion del display.
	inicializar_leds();
	adc_inicializar();
	TIM5_Start(); 			// Inicializa el timer del DHT.

	RCC_Configuration(); 	// Habilitación de los timers.
	USART3_Configuration();	// Configuración del USART.
	NVIC_Config();          // Configuracion del NVIC.
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);

	cargar_datos(); // Se cargan los datos por default.

	TM_RTC_Init(TM_RTC_ClockSource_External); // Se inicializa el RTC.
	TM_RTC_Interrupts(TM_RTC_Int_1s); //Se activan las interrupciones del RTC.

	SystemInit(); // Activa el systick.
	SysTick_Config(SystemCoreClock / 1e3); // Configuracion del tiempo de la interrupcion (cada 1us).

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

	// Rutina DHT
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
		led.contador = 1000;
	}

	// Rutina display.
	if(display.contador >= 1){
		display.contador--;
	}
	else{
		display.flag = 1;
		display.contador = 2000;
	}

	// Rutina cultivo
	if(cultivo.contador >= 1){
		cultivo.contador--;
	}
	else{
		cultivo.flag.control_activo = 1;
	}
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

	// Manejo de la interfaz de usuario.
	display_task();

	// Otras tareas.
	led_task();

	check_cultivo_task();

	if(cultivo.flag.control_activo){
		control_temp_task();
	}
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
	/*	Funcion ldr_task()
	*	No recive ni devuelve un valor.
	*	Tarea que lee un valor de ADC establecido en adc.h
	*/
	
	if(display.flag){
		char buffer[16];

		if(display.estado <6){
			display.estado++;
		}
		else{
			display.estado = 0;
		}

		switch(display.estado){ // Se muestran distintos carteles cada vez que se vence el contador del display.
		case 0:{
			// Se muestra el valor del LDR.
			sprintf(buffer,"%d   ",ldr.adc_cuentas);
			UB_LCD_2x16_String(0,0,"LDR:       ");
			UB_LCD_2x16_String(12,0,buffer);
			UB_LCD_2x16_String(0,1,"                ");
		};break;

		case 1:{
			// Se muestra el valor del YL-69.
			sprintf(buffer,"%d   ",yl.adc_cuentas);
			UB_LCD_2x16_String(0,0,"YL-69:     ");
			UB_LCD_2x16_String(12,0,buffer);
		};break;

		case 2:{
			// Se muestra el valot del dht exterior.
			switch(dht_exterior.estado){
			case STATE_DHT_CHECKSUM_GOOD:{
				UB_LCD_2x16_String(0,0,"Temp:       "); // Texto en la linea 1
				UB_LCD_2x16_String(12,0,dht_exterior.temperatura_string); // Texto en la linea 1
				UB_LCD_2x16_String(0,1,"Exter  "); // Texto en la linea 1
				display.flag = 0;
			};break;
			case STATE_DHT_CHECKSUM_BAD:{
				UB_LCD_2x16_String(0,0,"Error de CHEcK  "); // Texto en la linea 1
				UB_LCD_2x16_String(0,1,"Exterior        "); // Texto en la linea 1
				display.flag = 0;
			};break;
			case STATE_DHT_DESCONECTADO:{
				UB_LCD_2x16_String(0,0,"DHT desconectado"); // Texto en la linea 1
				UB_LCD_2x16_String(0,1,"Exterior        "); // Texto en la linea 1
				display.flag = 0;
			};break;
			case STATE_DHT_TIMEOUT:{
				UB_LCD_2x16_String(0,0,"DHT Timeout     "); // Texto en la linea 1
				UB_LCD_2x16_String(0,1,"Exterior        "); // Texto en la linea 1
				display.flag = 0;
			};break;
			default:{
				UB_LCD_2x16_String(0,0,"Err  desconocido"); // Texto en la linea 1
				UB_LCD_2x16_String(0,1,"Exterior        "); // Texto en la linea
				display.flag = 0;
			};break;}

		};break;

		case 3:{
			// Se muestra el valor del dht interior.
			switch(dht_interior.estado){
			case STATE_DHT_CHECKSUM_GOOD:{
				UB_LCD_2x16_String(0,0,"Temp:       "); // Texto en la linea 1
				UB_LCD_2x16_String(12,0,dht_interior.temperatura_string); // Texto en la linea 1
				UB_LCD_2x16_String(0,1,"Inter       "); // Texto en la linea 1
				display.flag = 0;
			};break;
			case STATE_DHT_CHECKSUM_BAD:{
				UB_LCD_2x16_String(0,0,"Error de CHEcK  "); // Texto en la linea 1
				UB_LCD_2x16_String(0,1,"Interior        "); // Texto en la linea 1
				display.flag = 0;
			};break;
			case STATE_DHT_DESCONECTADO:{
				UB_LCD_2x16_String(0,0,"DHT desconectado"); // Texto en la linea 1
				UB_LCD_2x16_String(0,1,"Interior        "); // Texto en la linea 1
				display.flag = 0;
			};break;
			case STATE_DHT_TIMEOUT:{
				UB_LCD_2x16_String(0,0,"DHT Timeout     "); // Texto en la linea 1
				UB_LCD_2x16_String(0,1,"Interior        "); // Texto en la linea 1
				display.flag = 0;
			};break;
			default:{
				UB_LCD_2x16_String(0,0,"Err  desconocido"); // Texto en la linea 1
				UB_LCD_2x16_String(0,1,"Interior        "); // Texto en la linea
				display.flag = 0;
			};break;}
		};break;

		case 4:{
			// Se muestra la diferencia de temperatura entre el dht interior y el exterior.
			if(dht_interior.estado == STATE_DHT_CHECKSUM_GOOD && dht_exterior.estado == STATE_DHT_CHECKSUM_GOOD){
				char aux[16];
				uint16_t diferencia = dht_exterior.temperatura - dht_interior.temperatura;
				//				uint16_t numerador = diferencia/10;
				//				uint16_t decimal = diferencia/100;
				sprintf(aux,"Temp:%d         ",diferencia);
				UB_LCD_2x16_String(0,0,"Diferencia      "); // Texto en la linea 1
				UB_LCD_2x16_String(0,1,aux); // Texto en la linea 1
				display.flag = 0;
			}
			else{
				UB_LCD_2x16_String(0,0,"Diferencia      "); // Texto en la linea 1
				UB_LCD_2x16_String(0,1,"DHT fail        "); // Texto en la linea 1
				display.flag = 0;
			}
		};break;

		case 5:{
			// Se muestra la hora actual.
			UB_LCD_2x16_String(0,0,"Hora           "); // Texto en la linea 1
			UB_LCD_2x16_String(0,1,"     ");
			TM_RTC_GetDateTime(&datatime, TM_RTC_Format_BIN); // Se obtiene la hora actual.
			sprintf(buffer,"%d:%d:%d",datatime.hours,datatime.minutes,datatime.seconds);
			UB_LCD_2x16_String(5,1,buffer); // Texto en la linea 1
		};break;

		case 6:{
			// Se muestra el estado del cultivo.
			if(cultivo.flag.control_activo){
				UB_LCD_2x16_String(0,0,cultivo.nombre); // Texto en la linea 1
				sprintf(buffer,"Etapa %d         ",cultivo.etapa_actual);
				UB_LCD_2x16_String(0,1,buffer);

			}
			else{
				UB_LCD_2x16_String(0,0,"Ningun cultivo  "); // Texto en la linea 1
				UB_LCD_2x16_String(0,1,"Activo          ");
			}

		};break;

		default:{
			UB_LCD_2x16_String(0,0,"Display-fail");
			UB_LCD_2x16_String(0,1,"            ");
			display.flag = 0;
		};break;
		}
		display.flag = 0;

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

void check_cultivo_task(){
	/*	cecck_cultivo_task()
	 * 	Esta funcion se encarga de cambiar los parametros del control del cultivo
	 * 	segun la etapa en la que se encuentre.
	 * 	Por Ej, en germinacion se situan ciertos parametros distintos que en cosecha.
	 */
	uint8_t i = 0;

	if(cultivo.flag.control_activo){
		TM_RTC_GetDateTime(&datatime, TM_RTC_Format_BIN); // Se obtiene la hora actual.

		for(i=0;i<2;i++){

			if(cultivo.etapa[i].year == datatime.year && cultivo.etapa[i].month == datatime.month && cultivo.etapa[i].date == datatime.date
					&& cultivo.etapa[i].hours == datatime.hours){
				// Se establecen los valores de control de la etapa cuando se llega al dia.
				cultivo.etapa_actual = i;
//				GPIO_ToggleBits(GPIOD,GPIO_Pin_15);
			}
		}
	}
}

void cargar_datos(){

	//Temperatura
	control.limite_delta_temp = 5;
	control.max_temp_fan = 26;
	control.min_temp_fan = 25;
	control.max_temp_calentador = 18;
	control.min_temp_calentador = 17;
	control.flag.bomba_encendida = 0;
	control.flag.fan_encendido = 0;
	control.flag.calentador_encendido = 0;

	//Cultivo
	cultivo.etapa[0].year = 18;
	cultivo.etapa[0].month = 9;
	cultivo.etapa[0].date = 25;
	cultivo.etapa[0].hours = 11;

	cultivo.etapa[1].year = 18;
	cultivo.etapa[1].month = 9;
	cultivo.etapa[1].date = 25;
	cultivo.etapa[1].hours = 12;
	cultivo.contador = 14000;
	cultivo.flag.control_activo = 0;
	strcpy(cultivo.nombre,"Tomate");
}


void RCC_Configuration(void)
{
	/* --------------------------- System Clocks Configuration -----------------*/
	/* USART3 habilitación del timer */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	/* GPIOB habilitación del timer */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
}

void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/*-------------------------- Configuración GPIO ----------------------------*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	/* Conección de los pines USART al AF */
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_USART3);
}

static void NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Enable the USARTx Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 10;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void USART3_Configuration(void)
{
	USART_InitTypeDef USART_InitStructure;

	/* USARTx configuration ------------------------------------------------------*/
	/* USARTx configurada como sigue:
        - BaudRate = 9600 baud
        - Largo de palabra = 8 Bits
        - Un Bit de stop
        - Sin paridad
        - COntrol de flujo por hardware deshabilitado (RTS and CTS signals)
        - Recepcion y transmision habilitadas
	 */
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;

	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART3, &USART_InitStructure);

	USART_Cmd(USART3, ENABLE);
}

void USART3_IRQHandler(void) {
	//Check if interrupt was because data is received
	if (USART_GetITStatus(USART3, USART_IT_RXNE)) {
		//Do your stuff here

		cultivo.flag.control_activo = (cultivo.flag.control_activo)?0:1;

		//Clear interrupt flag
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
	}
}




