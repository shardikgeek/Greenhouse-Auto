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
}


//****************************************************************************//
// Transition function "f_tomate"
//****************************************************************************//
void FSM_f_tomate (void)
{
  // You can add your code here
}


//****************************************************************************//
// Transition function "f_zanahoria"
//****************************************************************************//
void FSM_f_zanahoria (void)
{
  // You can add your code here
}


//****************************************************************************//
// Transition function "f_humedad"
//****************************************************************************//
void FSM_f_humedad (void)
{
  // You can add your code here
}


//****************************************************************************//
// Transition function "f_temp_ext"
//****************************************************************************//
void FSM_f_temp_ext (void)
{
  // You can add your code here
}


//****************************************************************************//
// Transition function "f_temp_int"
//****************************************************************************//
void FSM_f_temp_int (void)
{
  // You can add your code here
}


//****************************************************************************//
// Transition function "f_bomba"
//****************************************************************************//
void FSM_f_bomba (void)
{
  // You can add your code here
}


//****************************************************************************//
// Transition function "f_calentador"
//****************************************************************************//
void FSM_f_calentador (void)
{
  // You can add your code here
}


//****************************************************************************//
// Transition function "f_ventana"
//****************************************************************************//
void FSM_f_ventana (void)
{
  // You can add your code here
}


//****************************************************************************//
// Transition function "f_ventilador"
//****************************************************************************//
void FSM_f_ventilador (void)
{
  // You can add your code here
}


//****************************************************************************//
// Transition function "f_log"
//****************************************************************************//
void FSM_f_log (void)
{
  // You can add your code here
}


//****************************************************************************//
// Transition function "f_etapas"
//****************************************************************************//
void FSM_f_etapas (void)
{
  // You can add your code here
}


//****************************************************************************//
