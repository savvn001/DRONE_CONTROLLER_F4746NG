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
#include "../Drivers/unpack_variables.h"

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct Misc_str{

	bool connection;
	bool kill;
	bool airmode;

};

struct Misc_str Misc;



void initLCD(struct GPS_str GPS);
void drawUI();
void drawIMUScreen();
void drawMainScreen(struct GPS_str GPS, struct IMU_str IMU, struct Misc_str Misc, float *enc_pid) ;

bool float_to_string(float f, char r[]);
int n_tu(int number, int count);
float map(int x, int in_min, int in_max, int out_min, int out_max);
#endif /* UI_H_ */
