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
	TIM5_Start(); // Inicializa el timer del DHT.
	USART3_Config();
	SystemInit(); // Activa el systick.
	SysTick_Config(SystemCoreClock / 1e3); // Configuracion del tiempo de la interrupcion (cada 1us).

	serial.contador = 0;
	serial.flag.comienzo_paquete = 0;
	serial.flag.fin_paquete = 0;
	serial.timeout = 0;
	sistema.flag.conexion_serial = 0;
	sistema.flag.modo_monitor_serial = 0;

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
	//
	//	if(dht_exterior.flag){
	//		if(dht_exterior.timeout >= 1){
	//			dht_exterior.timeout--;
	//		}
	//
	//		if(dht_exterior.timeout <= 0){
	//			dht_exterior.flag_timeout = 1;
	//		}
	//	}
	//
	//	if(dht_exterior.contador >= 1){
	//		dht_exterior.contador--;
	//	}
	//	else{
	//		dht_exterior.flag = 1;
	//		dht_exterior.contador = 2000;
	//		dht_exterior.flag_timeout = 0;
	//		dht_exterior.timeout = 1000; // Se le agrega un tiempo de timeout para esperar la funcion DHT11Start.
	//	}
	//
	//	// Rutina LDR
	//	if(ldr.contador >= 1){
	//		ldr.contador--;
	//	}
	//	else{
	//		ldr.flag.fin_contador = 1;
	//		ldr.contador = 100;
	//	}
	//
	//	// Ruitna YL-69
	//	if(yl.contador >= 1){
	//		yl.contador--;
	//	}
	//	else{
	//		yl.flag = 1;
	//		yl.contador = 500;
	//	}

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

void task_manager(void){
	/*	Funcion task_manager
	 *	No recive ni devuelve un valor.
	 * 	Se encarga de llamar a las distintas tareas del programa.
	 *	Estas tareas se realizan o no dependiendo de su flag de evento.
	 */

	// Lectura de datos
	dht_interior_task();
	//	dht_exterior_task();
	//	ldr_task();
	//	yl69_task();

	serial_task();
	// Manejo de la interfaz de usuario.
	display_task();

	// Otras tareas.
	if(!sistema.flag.conexion_serial){ // Se realizan las tareas si no se esta conectado al serial
		led_task();
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

	if(display.flag){

		if(!sistema.flag.conexion_serial){
//			char buffer[16];

			if(display.estado <3){
				display.estado++;
			}
			else{
				display.estado = 0;
			}

			switch(display.estado){ // Se muestran distintos carteles cada vez que se vence el contador del display.
			case 0:{
				UB_LCD_2x16_String(0,0,"Probando    ");
				UB_LCD_2x16_String(0,1,"            ");
				display.flag = 0;
			};break;
			case 1:{
				if(serial.flag.fin_paquete){
					UB_LCD_2x16_String(0,1,serial.comando);
					UB_LCD_2x16_String(0,0,"                ");
				}
				else{
					UB_LCD_2x16_String(0,1,"Sin paquete     ");
					UB_LCD_2x16_String(0,0,"                ");
				}
				display.flag = 0;
			};break;
			case 2:{
				if(serial.flag.fin_paquete){
					UB_LCD_2x16_String(0,1,serial.cant_bytes);
					UB_LCD_2x16_String(0,0,"                ");
				}
				else{
					UB_LCD_2x16_String(0,1,"Sin paquete     ");
					UB_LCD_2x16_String(0,0,"                ");
				}
				display.flag = 0;
			};break;
			case 3:{
				if(serial.flag.fin_paquete){
					UB_LCD_2x16_String(0,1,serial.datos);
					UB_LCD_2x16_String(0,0,"                ");
				}
				else{
					UB_LCD_2x16_String(0,1,"Sin paquete     ");
					UB_LCD_2x16_String(0,0,"                ");
				}
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

void check_cultivo_task(){
	/*	cecck_cultivo_task()
	 * 	Esta funcion se encarga de cambiar los parametros del control del cultivo
	 * 	segun la etapa en la que se encuentre.
	 * 	Por Ej, en germinacion se situan ciertos parametros distintos que en cosecha.
	 */
	uint8_t i = 0;

	if(cultivo.flag.control_activo){
		TM_RTC_GetDateTime(&datatime, TM_RTC_Format_BIN); // Se obtiene la hora actual.

		for(i=0;i<3;i++){

			if(cultivo.etapa[i].year == datatime.year && cultivo.etapa[i].month == datatime.month && cultivo.etapa[i].date == datatime.date
					&& cultivo.etapa[i].hours == datatime.hours && cultivo.etapa[i].minutes == datatime.minutes){
				// Se establecen los valores de control de la etapa cuando se llega al dia.
				control.limite_delta_temp = cultivo.variables[i].limite_delta_temp;
				control.max_temp_fan = cultivo.variables[i].max_temp_fan;
				control.min_temp_fan = cultivo.variables[i].min_temp_fan;
				control.max_temp_calentador = cultivo.variables[i].max_temp_calentador;
				control.min_temp_calentador = cultivo.variables[i].min_temp_calentador;
				cultivo.etapa_actual = i;
				//				GPIO_ToggleBits(GPIOD,GPIO_Pin_15);
			}
		}


	}
}

// Tarea de serial.
void serial_task(void){
	/*	Funcion serial_task()
	 * 	Se encarga de manejar los paquetes que van ingresando al MCU.
	 * 	Llama a las distintas funciones que se encargan de ejecutar cada comando.
	 */
	if(serial.flag.fin_paquete){
		if(!strcmp(serial.comando,"STR") && sistema.flag.conexion_serial){
			GPIO_ToggleBits(GPIOD,GPIO_Pin_12);
			enviar_comando(":OKK,-,-!");
		}
		if(!strcmp(serial.comando,"ATR") && sistema.flag.conexion_serial){
			enviar_comando(":OKK,-,-!");
			enviar_comando("\nDHT: ");
			enviar_comando(dht_interior.temperatura_string);
		}
		if(!strcmp(serial.comando,"SKR") && sistema.flag.conexion_serial){
			GPIO_ToggleBits(GPIOD,GPIO_Pin_14);
			enviar_comando(":OKK,-,-!");
		}
		if(!strcmp(serial.comando,"INI") && !sistema.flag.conexion_serial){
			sistema.flag.conexion_serial = 1;
			enviar_comando(":OKK,-,-!");
		}
		if(!strcmp(serial.comando,"STP") && sistema.flag.conexion_serial){
			sistema.flag.conexion_serial = 0;
			enviar_comando(":OKK,-,-!");
		}
		if(!strcmp(serial.comando,"LED") && sistema.flag.conexion_serial){
			contador_led = atoi(serial.datos);
			enviar_comando(":OKK,-,-!");
		}
		if(!strcmp(serial.comando,"MON") && !strcmp(serial.datos,"ON") && sistema.flag.conexion_serial){
			sistema.flag.modo_monitor_serial = 1;
			enviar_comando(":OKK,-,-!");
		}
		if(!strcmp(serial.comando,"MON") && !strcmp(serial.datos,"OFF") && sistema.flag.conexion_serial){
			sistema.flag.modo_monitor_serial = 0;
			enviar_comando(":OKK,-,-!");
		}
	}
	serial.flag.fin_paquete = 0;

	if(sistema.flag.modo_monitor_serial && serial.flag.mostrar_valores){ // Si se esta en modo monitor se muestra el valor cada vez q se
		enviar_comando(" DHT: ");										//	vence el contador.
		enviar_comando(dht_interior.temperatura_string);
		serial.flag.mostrar_valores = 0;
	}
}

void cargar_datos(){
	/*	Funcion cargar_datos()
	 * 	Esta funcion carga los datos por defecto al sistema de control.
	 * 	De momento los valores se toman de unos arreglos ya predefinidos, la idea es que sean leidos por memoria externa.
	 */

	//Temperaturas por defecto.
	control.limite_delta_temp = 5;
	control.max_temp_fan = 26;
	control.min_temp_fan = 25;
	control.max_temp_calentador = 18;
	control.min_temp_calentador = 17;
	control.flag.bomba_encendida = 0;
	control.flag.fan_encendido = 0;
	control.flag.calentador_encendido = 0;

	configurar_cultivo_tomate();
}

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

	uint8_t i,n_etapas_validas = 0;

	//Fechas de comienzo del cultivo para cada etapa.
	cultivo.etapa[0].year = 18;
	cultivo.etapa[0].month = 10;
	cultivo.etapa[0].date = 1;
	cultivo.etapa[0].hours = 14;
	cultivo.etapa[0].minutes = 10;

	cultivo.etapa[1].year = 18;
	cultivo.etapa[1].month = 10;
	cultivo.etapa[1].date = 1;
	cultivo.etapa[1].hours = 14;
	cultivo.etapa[1].minutes = 11;

	cultivo.etapa[2].year = 18;
	cultivo.etapa[2].month = 10;
	cultivo.etapa[2].date = 1;
	cultivo.etapa[2].hours = 14;
	cultivo.etapa[2].minutes = 12;

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

	strcpy(cultivo.nombre,"Tomate");

	TM_RTC_GetDateTime(&datatime, TM_RTC_Format_BIN); // Se obtiene la hora actual.

	// Se comprueba que todas las etapas sean validas, es decir que no tengan una fecha anterior a la actual.
	for(i=0;i<3;i++){
		if(fecha_valida(cultivo.etapa[i]))
			n_etapas_validas++;
	}

	if(n_etapas_validas == 3){
		// Si el numero de etapas validas es el total se activa el control.
		cultivo.flag.control_activo = 1;
	}
	else
		cultivo.flag.control_activo = 0;
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
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
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
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 10;
	NVIC_Init(&NVIC_InitStruct);

	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); // Se habilitan las interrupciones cuando se recibe un dato.
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





