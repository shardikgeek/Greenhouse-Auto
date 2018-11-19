//--------------------------------------------------------------
// File     : main.c
// Datum    : 06.04.2013
// Version  : 1.1
// Autor    : UB
// EMail    : mc-4u(@)t-online.de
// Web      : www.mikrocontroller-4u.de
// CPU      : STM32F4
// IDE      : CooCox CoIDE 1.7.0
// Module   : CMSIS_BOOT, M4_CMSIS_CORE
// Funktion : Demo der FATFS-SDIO-Library
// Hinweis  : Diese zwei Files muessen auf 8MHz stehen
//              "cmsis_boot/stm32f4xx.h"
//              "cmsis_boot/system_stm32f4xx.c"
// Modificar ub_sdcard.h para seleccionar los modos de 1 o 4 bits
//--------------------------------------------------------------

#include "main.h"
#include "stm32_ub_fatfs.h"

void inicializar_leds(void);

int main(void)
{
  FIL myFile;   // Filehandler

  SystemInit(); // Quarz Einstellungen aktivieren

  // Init vom FATFS-System
  UB_Fatfs_Init();
  inicializar_leds();

  // Check ob Medium eingelegt ist
  if(UB_Fatfs_CheckMedia(MMC_0)==FATFS_OK) {
		GPIO_SetBits(GPIOD,GPIO_Pin_13);
	// Media mounten
    if(UB_Fatfs_Mount(MMC_0)==FATFS_OK) {
    	GPIO_SetBits(GPIOD,GPIO_Pin_12);
      // File zum schreiben im root neu anlegen
      if(UB_Fatfs_OpenFile(&myFile, "0:/UB_File.txt", F_WR_NEW)==FATFS_OK) {
    	// ein paar Textzeilen in das File schreiben
        UB_Fatfs_WriteString(&myFile,"Test der WriteString-Funktion");
        UB_Fatfs_WriteString(&myFile,"hier Zeile zwei");
        UB_Fatfs_WriteString(&myFile,"ENDE");
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

  while(1)
  {

  }
}

void inicializar_leds(void){
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

