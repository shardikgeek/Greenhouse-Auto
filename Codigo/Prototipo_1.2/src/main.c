#include "main.h"

int main(void){

	/*	PP3 Ejercicio 1
	 *	Se debe mostrar un cartel, en un display LCD.
	 *
	 */
	UB_LCD_2x16_Init(); // Inicializacion del display.
	inicializar_leds();
	adc_inicializar();
	TIM5_Start(); // Inicializa el timer del DHT.

	cargar_datos(); // Se cargan los datos por default.

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

	// Rutina Tiempo
 	if(tiempo.contador >=1){
 		tiempo.contador--;
 	}
 	else{
 		tiempo.flag.fin_contador = 1;
 		tiempo.contador = 1000;
 	}

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
		display.contador = 3000;
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

	tiempo_task();

	// Lectura de datos
	dht_interior_task();
	dht_exterior_task();
	ldr_task();
	yl69_task();

	// Manejo de la interfaz de usuario.
	display_task();

	// Otras tareas.
	led_task();
	control_temp_task();
}

void tiempo_task(){
	/*	Funcion tiempo_task()
	*	No recive ni devuelve un valor.
	*	Tarea que simula un reloj por software. Se la llama cada
	*	1s y de ahi inrementa los valores.
	*/

	if(tiempo.flag.fin_contador){
		if(tiempo.segundos < 60){
			tiempo.segundos++;
		}
		else if (tiempo.segundos == 60){

			if(tiempo.minutos <60){
				tiempo.minutos++;
				tiempo.segundos = 0;
			}

			else if(tiempo.minutos == 60){
				if(tiempo.hora < 24){
					tiempo.minutos = 0;
					tiempo.segundos = 0;
					tiempo.hora++;
				}
				else{
					tiempo.hora = 0;
					tiempo.minutos = 0;
					tiempo.segundos = 0;
				}
			}
		}
	}

	tiempo.flag.fin_contador = 0;

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

		if(display.estado <5){
			display.estado++;
		}
		else{
			display.estado = 0;
		}

		switch(display.estado){
		case 0:{
			sprintf(buffer,"%d   ",ldr.adc_cuentas);
			UB_LCD_2x16_String(0,0,"LDR:       ");
			UB_LCD_2x16_String(12,0,buffer);
			UB_LCD_2x16_String(0,1,"       ");
			sprintf(buffer,"%d:%d:%d",tiempo.hora,tiempo.minutos,tiempo.segundos);
			UB_LCD_2x16_String(8,1,buffer); // Texto en la linea 1
		};break;

		case 1:{
			sprintf(buffer,"%d   ",yl.adc_cuentas);
			UB_LCD_2x16_String(0,0,"YL-69:     ");
			UB_LCD_2x16_String(12,0,buffer);
			UB_LCD_2x16_String(0,1,"       ");
			sprintf(buffer,"%d:%d:%d",tiempo.hora,tiempo.minutos,tiempo.segundos);
			UB_LCD_2x16_String(8,1,buffer); // Texto en la linea 1
		};break;

		case 2:{
			switch(dht_exterior.estado){
			case STATE_DHT_CHECKSUM_GOOD:{
				UB_LCD_2x16_String(0,0,"Temp:       "); // Texto en la linea 1
				UB_LCD_2x16_String(12,0,dht_exterior.temperatura_string); // Texto en la linea 1
				UB_LCD_2x16_String(0,1,"Exter  "); // Texto en la linea 1
				sprintf(buffer,"%d:%d:%d",tiempo.hora,tiempo.minutos,tiempo.segundos);
				UB_LCD_2x16_String(8,1,buffer); // Texto en la linea 1
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
			switch(dht_interior.estado){
			case STATE_DHT_CHECKSUM_GOOD:{
				UB_LCD_2x16_String(0,0,"Temp:       "); // Texto en la linea 1
				UB_LCD_2x16_String(12,0,dht_interior.temperatura_string); // Texto en la linea 1
				UB_LCD_2x16_String(0,1,"Inter  "); // Texto en la linea 1
				sprintf(buffer,"%d:%d:%d",tiempo.hora,tiempo.minutos,tiempo.segundos);
				UB_LCD_2x16_String(8,1,buffer); // Texto en la linea 1
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
			if(dht_interior.estado == STATE_DHT_CHECKSUM_GOOD && dht_exterior.estado == STATE_DHT_CHECKSUM_GOOD){
				char aux[16];
				uint16_t diferencia = dht_exterior.temperatura - dht_interior.temperatura;
				//				uint16_t numerador = diferencia/10;
				//				uint16_t decimal = diferencia/100;
				sprintf(aux,"Temp:%d         ",diferencia);
				UB_LCD_2x16_String(0,0,"Diferencia      "); // Texto en la linea 1
				UB_LCD_2x16_String(0,1,aux); // Texto en la linea 1
				sprintf(buffer,"%d:%d:%d",tiempo.hora,tiempo.minutos,tiempo.segundos);
				UB_LCD_2x16_String(8,1,buffer); // Texto en la linea 1
				display.flag = 0;
			}
			else{
				UB_LCD_2x16_String(0,0,"Diferencia      "); // Texto en la linea 1
				UB_LCD_2x16_String(0,1,"DHT fail        "); // Texto en la linea 1
				display.flag = 0;
			}
		};break;

		case 5:{
			UB_LCD_2x16_String(0,0,"Hora           "); // Texto en la linea 1
			sprintf(buffer,"%d:%d:%d",tiempo.hora,tiempo.minutos,tiempo.segundos);
			UB_LCD_2x16_String(0,1,"       ");
			UB_LCD_2x16_String(8,1,buffer); // Texto en la linea 1
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

void cargar_datos(){
	// Hora
	tiempo.hora = 12;
	tiempo.minutos = 0;
	tiempo.segundos = 0;

	//Temperatura
	control.limite_delta_temp = 5;
	control.max_temp_fan = 26;
	control.min_temp_fan = 25;
	control.max_temp_calentador = 18;
	control.min_temp_calentador = 17;
	control.flag.bomba_encendida = 0;
	control.flag.fan_encendido = 0;
	control.flag.calentador_encendido = 0;
}







