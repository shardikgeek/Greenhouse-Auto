//****************************************************************************//
//  MSP430 state machine
//  fsm.c
//
//  Describtion:
//    A simple state machine for the MSP430
//    Do not change code in here!!!
//
//  Generated with Excel Table
//  Date:  11/26/2018        Time:  17:47:43
//
//****************************************************************************//

#include <stdint.h>
#include <stdio.h>
#include "fsm.h"

//****************************************************************************//
// Global variables
//****************************************************************************//
uint8_t  ActState;

const FSM_STATE_TABLE StateTable [NR_STATES][NR_EVENTS] =
{
  NULL, CULTIVO, NULL, MENU_DESACTIVADO, NULL, MENU_DESACTIVADO, NULL, MENU_DESACTIVADO, NULL, MENU_DESACTIVADO,
  NULL, MENU_DESACTIVADO, NULL, NUEVO_CULTIVO, NULL, CULTIVO, NULL, VARIABLES, NULL, MENU_DESACTIVADO,
  NULL, MENU_DESACTIVADO, NULL, HUMEDAD, NULL, CULTIVO, NULL, CONTROL, NULL, MENU_DESACTIVADO,
  NULL, MENU_DESACTIVADO, NULL, BOMBA, NULL, VARIABLES, NULL, EXPORTAR, NULL, MENU_DESACTIVADO,
  NULL, MENU_DESACTIVADO, NULL, LOG, NULL, CONTROL, NULL, EXPORTAR, NULL, MENU_DESACTIVADO,
  NULL, MENU_DESACTIVADO, NULL, TOMATE, NULL, NUEVO_CULTIVO, NULL, PAUSAR, NULL, CULTIVO,
  NULL, MENU_DESACTIVADO, FSM_f_pausar, PAUSAR, NULL, NUEVO_CULTIVO, NULL, CANCELAR, NULL, CULTIVO,
  NULL, MENU_DESACTIVADO, FSM_f_cancelar, CANCELAR, NULL, PAUSAR, NULL, CANCELAR, NULL, CULTIVO,
  NULL, MENU_DESACTIVADO, FSM_f_tomate, TOMATE, NULL, TOMATE, NULL, ZANAHORIA, NULL, NUEVO_CULTIVO,
  NULL, MENU_DESACTIVADO, FSM_f_zanahoria, ZANAHORIA, NULL, TOMATE, NULL, ZANAHORIA, NULL, NUEVO_CULTIVO,
  NULL, MENU_DESACTIVADO, FSM_f_humedad, HUMEDAD, NULL, HUMEDAD, NULL, TEMP_EXT, NULL, VARIABLES,
  NULL, MENU_DESACTIVADO, FSM_f_temp_ext, TEMP_EXT, NULL, HUMEDAD, NULL, TEMP_INT, NULL, VARIABLES,
  NULL, MENU_DESACTIVADO, FSM_f_temp_int, TEMP_INT, NULL, TEMP_EXT, NULL, TEMP_INT, NULL, VARIABLES,
  NULL, MENU_DESACTIVADO, FSM_f_bomba, BOMBA, NULL, BOMBA, NULL, CALENTADOR, NULL, CONTROL,
  NULL, MENU_DESACTIVADO, FSM_f_calentador, CALENTADOR, NULL, BOMBA, NULL, VENTANA, NULL, CONTROL,
  NULL, MENU_DESACTIVADO, FSM_f_ventana, VENTANA, NULL, CALENTADOR, NULL, VENTILADOR, NULL, CONTROL,
  NULL, MENU_DESACTIVADO, FSM_f_ventilador, VENTILADOR, NULL, VENTANA, NULL, VENTILADOR, NULL, CONTROL,
  NULL, MENU_DESACTIVADO, FSM_f_log, LOG, NULL, LOG, NULL, ETAPAS, NULL, EXPORTAR,
  NULL, MENU_DESACTIVADO, FSM_f_etapas, ETAPAS, NULL, LOG, NULL, ETAPAS, NULL, EXPORTAR
};


//****************************************************************************//
// Initialize state machine
//****************************************************************************//
void FSM_Init (void)
{
  ActState = MENU_DESACTIVADO;
}


//****************************************************************************//
// Event function "menu"
//****************************************************************************//
void FSM_menu (void)
{
  if (StateTable[ActState][MENU].ptrFunct != NULL)
    StateTable[ActState][MENU].ptrFunct();

  ActState = StateTable[ActState][MENU].NextState;
}


//****************************************************************************//
// Event function "enter"
//****************************************************************************//
void FSM_enter (void)
{
  if (StateTable[ActState][ENTER].ptrFunct != NULL)
    StateTable[ActState][ENTER].ptrFunct();

  ActState = StateTable[ActState][ENTER].NextState;
}


//****************************************************************************//
// Event function "arriba"
//****************************************************************************//
void FSM_arriba (void)
{
  if (StateTable[ActState][ARRIBA].ptrFunct != NULL)
    StateTable[ActState][ARRIBA].ptrFunct();

  ActState = StateTable[ActState][ARRIBA].NextState;
}


//****************************************************************************//
// Event function "abajo"
//****************************************************************************//
void FSM_abajo (void)
{
  if (StateTable[ActState][ABAJO].ptrFunct != NULL)
    StateTable[ActState][ABAJO].ptrFunct();

  ActState = StateTable[ActState][ABAJO].NextState;
}


//****************************************************************************//
// Event function "back"
//****************************************************************************//
void FSM_back (void)
{
  if (StateTable[ActState][BACK].ptrFunct != NULL)
    StateTable[ActState][BACK].ptrFunct();

  ActState = StateTable[ActState][BACK].NextState;
}


//****************************************************************************//
