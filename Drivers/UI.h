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
#include <stdio.h>
#include <stdbool.h>

#include <stdlib.h>
#include <string.h>
void initLCD();
void drawMainScreen(bool cnt);
void float_to_string(float f, char r[]);
int n_tu(int number, int count);
#endif /* UI_H_ */
