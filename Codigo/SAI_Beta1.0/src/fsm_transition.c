//****************************************************************************//
//  MSP430 state machine
//  fsm_transition.c
//
//  Describtion:
//    A simple state machine for the MSP430
//    You can add your own code in here!!!
//
//  Generated with Excel Table
//  Date:  11/26/2018        Time:  17:47:52
//
//****************************************************************************//

#include "fsm.h"
#include "stm32_ub_lcd_2x16.h" // Inclusion de la libreria para manejar el LCD



//****************************************************************************//
// Transition function "f_pausar"
//****************************************************************************//
void FSM_f_pausar (void)
{
  // You can add your code here

	if(menu_flags.f_pausa){
		menu_flags.f_pausa = 0;
	}
	else{
		menu_flags.f_pausa = 1;
	}

	//display.refresh_time = 1001;
}


//****************************************************************************//
// Transition function "f_cancelar"
//****************************************************************************//
void FSM_f_cancelar (void)
{
  // You can add your code here
	if(menu_flags.f_cancelar){
		menu_flags.f_cancelar = 0;
	}
	else{
		menu_flags.f_cancelar = 1;
	}
}


//****************************************************************************//
// Transition function "f_tomate"
//****************************************************************************//
void FSM_f_tomate (void)
{
  // You can add your code here
	if(menu_flags.f_tomate){
		menu_flags.f_tomate = 0;
	}
	else{
		menu_flags.f_tomate = 1;
	}
}


//****************************************************************************//
// Transition function "f_zanahoria"
//****************************************************************************//
void FSM_f_zanahoria (void)
{
  // You can add your code here
	if(menu_flags.f_zanahoria){
		menu_flags.f_zanahoria = 0;
	}
	else{
		menu_flags.f_zanahoria = 1;
	}
}


//****************************************************************************//
// Transition function "f_humedad"
//****************************************************************************//
void FSM_f_humedad (void)
{
  // You can add your code here
	if(menu_flags.f_humedad){
		menu_flags.f_humedad = 0;
	}
	else{
		menu_flags.f_humedad = 1;
	}
}


//****************************************************************************//
// Transition function "f_temp_ext"
//****************************************************************************//
void FSM_f_temp_ext (void)
{
  // You can add your code here
	if(menu_flags.f_temp_ext){
		menu_flags.f_temp_ext = 0;
	}
	else{
		menu_flags.f_temp_ext = 1;
	}
}


//****************************************************************************//
// Transition function "f_temp_int"
//****************************************************************************//
void FSM_f_temp_int (void)
{
  // You can add your code here
	if(menu_flags.f_temp_int){
		menu_flags.f_temp_int = 0;
	}
	else{
		menu_flags.f_temp_int = 1;
	}
}


//****************************************************************************//
// Transition function "f_bomba"
//****************************************************************************//
void FSM_f_bomba (void)
{
  // You can add your code here
	if(menu_flags.f_bomba){
		menu_flags.f_bomba = 0;
	}
	else{
		menu_flags.f_bomba = 1;
	}
}


//****************************************************************************//
// Transition function "f_calentador"
//****************************************************************************//
void FSM_f_calentador (void)
{
  // You can add your code here
	if(menu_flags.f_calentador){
		menu_flags.f_calentador = 0;
	}
	else{
		menu_flags.f_calentador = 1;
	}
}


//****************************************************************************//
// Transition function "f_ventana"
//****************************************************************************//
void FSM_f_ventana (void)
{
  // You can add your code here
	if(menu_flags.f_ventana){
		menu_flags.f_ventana = 0;
	}
	else{
		menu_flags.f_ventana = 1;
	}
}


//****************************************************************************//
// Transition function "f_ventilador"
//****************************************************************************//
void FSM_f_ventilador (void)
{
  // You can add your code here
	if(menu_flags.f_ventilador){
		menu_flags.f_ventilador = 0;
	}
	else{
		menu_flags.f_ventilador = 1;
	}
}


//****************************************************************************//
// Transition function "f_log"
//****************************************************************************//
void FSM_f_log (void)
{
  // You can add your code here
	if(menu_flags.f_log){
		menu_flags.f_log = 0;
	}
	else{
		menu_flags.f_log = 1;
	}
}


//****************************************************************************//
// Transition function "f_etapas"
//****************************************************************************//
void FSM_f_etapas (void)
{
  // You can add your code here
	if(menu_flags.f_etapas){
		menu_flags.f_etapas = 0;
	}
	else{
		menu_flags.f_etapas = 1;
	}
}


//****************************************************************************//
