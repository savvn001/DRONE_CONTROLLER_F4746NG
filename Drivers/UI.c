/*
 * UI.c
 *
 *  Created on: Sep 15, 2019
 *      Author: nick_savva
 */


#include "UI.h"
#include <stdio.h>

#include "../GUI/Images/icons.h"
#include "../GUI/Images/knob.h"

//Touch screen variables
TS_StateTypeDef ts;

void initLCD(){


	//LCD Initialisation functions

	BSP_SDRAM_Init(); /* Initializes the SDRAM device */
	__HAL_RCC_CRC_CLK_ENABLE(); /* Enable the CRC Module */

	BSP_TS_Init(480, 272);

	BSP_LCD_Init();
	BSP_LCD_LayerDefaultInit(0, LCD_FB_START_ADDRESS);
	BSP_LCD_SetLayerVisible(0, ENABLE);
	BSP_LCD_DisplayOn();

	BSP_LCD_SelectLayer(0);
	BSP_LCD_Clear(LCD_COLOR_WHITE);

//	BSP_LCD_SetTextColor(LCD_COLOR_DARKBLUE);
//	BSP_LCD_FillRect(0, 0, 480, 30);

	for(int i =0; i < i + 100; i++){
		BSP_LCD_DrawPixel(50,i,LCD_COLOR_BLACK);

	}


}

void drawMainScreen(){




	///////////////////////////////// GPS section //////////////////////////////////////////


	BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);

	BSP_LCD_SetFont(&FontAvenir12);

	BSP_LCD_DisplayStringAt(30, 50, (uint8_t *) "GPS", LEFT_MODE);


	BSP_LCD_DisplayStringAt(14, 80, (uint8_t *) "LONG.", LEFT_MODE);

	BSP_LCD_DisplayStringAt(14, 100, (uint8_t *) "LAT.", LEFT_MODE);

	BSP_LCD_DisplayStringAt(14, 120, (uint8_t *) "SPEED.", LEFT_MODE);

	BSP_LCD_DisplayStringAt(14, 140, (uint8_t *) "ALT.", LEFT_MODE);

	//GPS variables

	BSP_LCD_DisplayStringAt(100, 80, (uint8_t *) "-5.3", LEFT_MODE);

	BSP_LCD_DisplayStringAt(100, 100, (uint8_t *) "6.43", LEFT_MODE);

	BSP_LCD_DisplayStringAt(100, 120, (uint8_t *) "14KM/H", LEFT_MODE);

	BSP_LCD_DisplayStringAt(100, 140, (uint8_t *) "24M", LEFT_MODE);

	///////////////////////////////// Top bar //////////////////////////////////////////

	BSP_LCD_SetBackColor(LCD_COLOR_DARKBLUE);

	BSP_LCD_DisplayStringAt(80, 10, (uint8_t *) "QUAD", RIGHT_MODE);
	BSP_LCD_DisplayStringAt(30, 10, (uint8_t *) "%", RIGHT_MODE);


	BSP_LCD_DisplayStringAt(14, 10, (uint8_t *) "TX", LEFT_MODE);
	BSP_LCD_DisplayStringAt(54, 10, (uint8_t *) "%", LEFT_MODE);



}
