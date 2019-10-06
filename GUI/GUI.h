/*
 * UI.h
 *
 *  Created on: Sep 15, 2019
 *      Author: nick_savva
 */

#ifndef GUI_H_
#define GUI_H_

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
void drawUI(struct GPS_str GPS, struct IMU_str IMU, struct Misc_str Misc, float *enc_pid, uint8_t *tune_axis);
void drawSettingsScreen();


void drawPIDscreen(struct IMU_str IMU,  float *enc_pid, uint8_t *tune_axis);
void drawPIDroll(struct IMU_str IMU,  float *enc_pid);
void drawPIDpitch(struct IMU_str IMU,  float *enc_pid);

void drawMainScreen(struct GPS_str GPS, struct IMU_str IMU, struct Misc_str Misc, float *enc_pid) ;

bool float_to_string(float f, char r[]);
int n_tu(int number, int count);
float map(float x, float in_min, float in_max, float out_min, float out_max);
void drawFloatString(char *array, int16_t xpos, int16_t ypos);



typedef struct Button_Ribbon{

	uint16_t x;
	uint16_t y;
	uint16_t width;
	uint16_t height;
	uint8_t fontSize;

	bool press;

	char *txt;
	char txt_l;
	uint16_t txt_offset;


} Button;

void createButton(Button button_str, bool *pressed);


typedef struct graph_{

	uint16_t x;
	uint16_t y;
	uint16_t width;
	uint16_t height;
	uint8_t fontSize;

	bool press;

	char *txt;
	char txt_l;
	uint16_t txt_offset;


} Graph;

void createGraph(float input, uint16_t *plot_data);
#endif /* GUI_H_ */
