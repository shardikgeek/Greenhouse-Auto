/* Include core modules */
#include "stm32f4xx.h"
#include "stm32_ub_lcd_2x16.h" // Inclusion de la libreria para manejar el LCD
/* Include my libraries here */
#include "defines.h"
#include "tm_stm32f4_ds1307.h"
#include "tm_stm32f4_delay.h"
#include <stdio.h>

int main(void) {
    char str[16];
    TM_DS1307_Time_t time;
    uint8_t last;

    /* Initialize system */
    SystemInit();

    UB_LCD_2x16_Init(); // Inicializacion del display.
    UB_LCD_2x16_String(1,1,"TEST"); // Texto en la linea 1

    /* Initialize DS1307 */
    if (TM_DS1307_Init() != TM_DS1307_Result_Ok) {
        /* Red LED on */

        /* Show on LCD */
        //TM_ILI9341_Puts(10, 10, "ERROR", &TM_Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_ORANGE);

    	UB_LCD_2x16_String(1,0,"ERROR"); // Texto en la linea 1

        while (1);
    }

    /* Set date and time */
    /* Day 7, 26th May 2014, 02:05:00 */
//    time.hours = 21;
//    time.minutes = 18;
//    time.seconds = 0;
//    time.date = 26;
//    time.day = 1;
//    time.month = 5;
//    time.year = 14;
//    TM_DS1307_SetDateTime(&time);

    /* Set output pin to 4096 Hz */
    TM_DS1307_EnableOutputPin(TM_DS1307_OutputFrequency_4096Hz);

    while (1) {
        /* Get date and time */
        TM_DS1307_GetDateTime(&time);

        /* Display on LCD */
        sprintf(str, "%d:%d:%d",time.hours,time.minutes,time.seconds);
        UB_LCD_2x16_String(1,0,str); // Texto en la linea 1

        Delayms(250);
    }
}
