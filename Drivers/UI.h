/*
 * UI.h
 *
 *  Created on: Sep 15, 2019
 *      Author: nick_savva
 */

#ifndef UI_H_
#define UI_H_

#include "stm32f7xx_hal.h"
#include "../GUI/stm32746g_discovery.h"
#include "../GUI/stm32746g_discovery_lcd.h"
#include "../GUI/stm32746g_discovery_sdram.h"
#include "../GUI/stm32746g_discovery_ts.h"

void initLCD();
void drawMainScreen();


#endif /* UI_H_ */
