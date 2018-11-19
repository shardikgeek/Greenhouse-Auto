//****************************************************************************//
//  MSP430 state machine
//  fsm.h
//
//  Describtion:
//    A simple state machine for the MSP430
//
//  Generated with Excel Table
//  Date:  11/12/2018        Time:  00:02:54
//
//****************************************************************************//

#ifndef FSM_H
#define FSM_H

#include <stdint.h>
#include <stdio.h>
#include "stm32_ub_lcd_2x16.h"

//****************************************************************************//
// State table typedef
//****************************************************************************//
typedef struct
{
  void (*ptrFunct) (void);
  uint8_t NextState;
} FSM_STATE_TABLE;

extern uint8_t ActState;


#define NR_EVENTS 5
#define MENU  0
#define ENTER  1
#define ARRIBA  2
#define ABAJO  3
#define BACK  4

#define NR_STATES 13
#define MENU_DESACTIVADO  0
#define MENU_PRINCIPAL_1  1
#define MENU_PRINCIPAL_2  2
#define MENU_PRINCIPAL_3  3
#define MENU_1_1  4
#define MENU_1_2  5
#define MENU_1_3  6
#define MENU_2_1  7
#define MENU_2_2  8
#define MENU_2_3  9
#define MENU_3_1  10
#define MENU_3_2  11
#define MENU_3_3  12


//****************************************************************************//
// Function prototypes
//****************************************************************************//
// Initialize state machine
void FSM_Init (void);

// Event function "menu"
void FSM_menu (void);

// Event function "enter"
void FSM_enter (void);

// Event function "arriba"
void FSM_arriba (void);

// Event function "abajo"
void FSM_abajo (void);

// Event function "back"
void FSM_back (void);

// Transition function "do_menu_1_1"
void FSM_do_menu_1_1 (void);

// Transition function "do_menu_1_2"
void FSM_do_menu_1_2 (void);

// Transition function "do_menu_1_3"
void FSM_do_menu_1_3 (void);

// Transition function "do_menu_2_1"
void FSM_do_menu_2_1 (void);

// Transition function "do_menu_2_2"
void FSM_do_menu_2_2 (void);

// Transition function "do_menu_2_3"
void FSM_do_menu_2_3 (void);

// Transition function "do_menu_3_1"
void FSM_do_menu_3_1 (void);

// Transition function "do_menu_3_2"
void FSM_do_menu_3_2 (void);

// Transition function "do_menu_3_3"
void FSM_do_menu_3_3 (void);


//****************************************************************************//

#endif /* FSM_H */

//****************************************************************************//
