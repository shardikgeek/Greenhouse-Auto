//****************************************************************************//
//  MSP430 state machine
//  fsm.c
//
//  Describtion:
//    A simple state machine for the MSP430
//    Do not change code in here!!!
//
//  Generated with Excel Table
//  Date:  11/12/2018        Time:  00:02:51
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
  NULL, MENU_PRINCIPAL_1, NULL, MENU_DESACTIVADO, NULL, MENU_DESACTIVADO, NULL, MENU_DESACTIVADO, NULL, MENU_DESACTIVADO,
  NULL, MENU_PRINCIPAL_1, NULL, MENU_1_1, NULL, MENU_PRINCIPAL_1, NULL, MENU_PRINCIPAL_2, NULL, MENU_DESACTIVADO,
  NULL, MENU_PRINCIPAL_2, NULL, MENU_2_1, NULL, MENU_PRINCIPAL_1, NULL, MENU_PRINCIPAL_3, NULL, MENU_DESACTIVADO,
  NULL, MENU_PRINCIPAL_3, NULL, MENU_3_1, NULL, MENU_PRINCIPAL_2, NULL, MENU_PRINCIPAL_3, NULL, MENU_DESACTIVADO,
  NULL, MENU_1_1, FSM_do_menu_1_1, MENU_1_1, NULL, MENU_1_1, NULL, MENU_1_2, NULL, MENU_PRINCIPAL_1,
  NULL, MENU_1_2, FSM_do_menu_1_2, MENU_1_2, NULL, MENU_1_1, NULL, MENU_1_3, NULL, MENU_PRINCIPAL_1,
  NULL, MENU_1_3, FSM_do_menu_1_3, MENU_1_3, NULL, MENU_1_2, NULL, MENU_1_3, NULL, MENU_PRINCIPAL_1,
  NULL, MENU_2_1, FSM_do_menu_2_1, MENU_2_1, NULL, MENU_2_1, NULL, MENU_2_2, NULL, MENU_PRINCIPAL_2,
  NULL, MENU_2_2, FSM_do_menu_2_2, MENU_2_2, NULL, MENU_2_1, NULL, MENU_2_3, NULL, MENU_PRINCIPAL_2,
  NULL, MENU_2_3, FSM_do_menu_2_3, MENU_2_3, NULL, MENU_2_2, NULL, MENU_2_3, NULL, MENU_PRINCIPAL_2,
  NULL, MENU_3_1, FSM_do_menu_3_1, MENU_3_1, NULL, MENU_3_1, NULL, MENU_3_2, NULL, MENU_PRINCIPAL_3,
  NULL, MENU_3_2, FSM_do_menu_3_2, MENU_3_2, NULL, MENU_3_1, NULL, MENU_3_3, NULL, MENU_PRINCIPAL_3,
  NULL, MENU_3_3, FSM_do_menu_3_3, MENU_3_3, NULL, MENU_3_2, NULL, MENU_3_3, NULL, MENU_3_3
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
