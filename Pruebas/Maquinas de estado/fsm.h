//****************************************************************************//
//  MSP430 state machine
//  fsm.h
//
//  Describtion:
//    A simple state machine for the MSP430
//
//  Generated with Excel Table
//  Date:  11/26/2018        Time:  17:47:54
//
//****************************************************************************//

#ifndef FSM_H
#define FSM_H

#include <stdint.h>
#include <stdio.h>

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

#define NR_STATES 19
#define MENU_DESACTIVADO  0
#define CULTIVO  1
#define VARIABLES  2
#define CONTROL  3
#define EXPORTAR  4
#define NUEVO_CULTIVO  5
#define PAUSAR  6
#define CANCELAR  7
#define TOMATE  8
#define ZANAHORIA  9
#define HUMEDAD  10
#define TEMP_EXT  11
#define TEMP_INT  12
#define BOMBA  13
#define CALENTADOR  14
#define VENTANA  15
#define VENTILADOR  16
#define LOG  17
#define ETAPAS  18


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

// Transition function "f_pausar"
void FSM_f_pausar (void);

// Transition function "f_cancelar"
void FSM_f_cancelar (void);

// Transition function "f_tomate"
void FSM_f_tomate (void);

// Transition function "f_zanahoria"
void FSM_f_zanahoria (void);

// Transition function "f_humedad"
void FSM_f_humedad (void);

// Transition function "f_temp_ext"
void FSM_f_temp_ext (void);

// Transition function "f_temp_int"
void FSM_f_temp_int (void);

// Transition function "f_bomba"
void FSM_f_bomba (void);

// Transition function "f_calentador"
void FSM_f_calentador (void);

// Transition function "f_ventana"
void FSM_f_ventana (void);

// Transition function "f_ventilador"
void FSM_f_ventilador (void);

// Transition function "f_log"
void FSM_f_log (void);

// Transition function "f_etapas"
void FSM_f_etapas (void);


//****************************************************************************//

#endif /* FSM_H */

//****************************************************************************//
