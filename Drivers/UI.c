/*
 * UI.c
 *
 *  Created on: Sep 15, 2019
 *      Author: nick_savva
 */

#include "UI.h"

#include "../GUI/Images/icons.h"


//Touch screen variables
TS_StateTypeDef ts;
bool cleared = 0;
void initLCD() {

	BSP_SDRAM_Init(); /* Initializes the SDRAM device */
	__HAL_RCC_CRC_CLK_ENABLE(); /* Enable the CRC Module */

	BSP_TS_Init(480, 272);

	BSP_LCD_Init();
	BSP_LCD_LayerDefaultInit(0, LCD_FB_START_ADDRESS);
	BSP_LCD_DisplayOn();

	BSP_LCD_SelectLayer(0);
	BSP_LCD_Clear(LCD_COLOR_BLACK);

	BSP_LCD_SetTextColor(LCD_COLOR_DARKBLUE);
	BSP_LCD_FillRect(0, 0, 480, 40);



}

char latt_str[48];
char long_str[10];
char speed_str[48];
char alt_str[48];

char throttle_str[2];

char quad_battery_str[48];
char tx_battery_str[48];

char roll_str[6];
char pitch_str[6];
char yaw_str[6];

/*	Current screen for state machine...
 * 	0 = main
 *  1 = IMU graphs
 *	2 = GPS info
 * 	3 = settings
 */
int screen;

void drawUI() {

	switch (screen) {
	case 0:
		//drawMainScreen();
		break;
	case 1:
		drawIMUScreen();

	default:
		break;
	}

}

void drawMainScreen(struct GPS_str GPS, struct IMU_str IMU, struct Misc_str Misc) {

	///////////////////////////////// GPS section //////////////////////////////////////////


	BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);

	BSP_LCD_SetFont(&FontAvenir12);

	BSP_LCD_DisplayStringAt(30, 50, (uint8_t *) "GPS", LEFT_MODE);
	BSP_LCD_DisplayStringAt(14, 80, (uint8_t *) "LONG.", LEFT_MODE);
	BSP_LCD_DisplayStringAt(14, 100, (uint8_t *) "LAT.", LEFT_MODE);
	BSP_LCD_DisplayStringAt(14, 120, (uint8_t *) "SPEED.", LEFT_MODE);
	BSP_LCD_DisplayStringAt(14, 140, (uint8_t *) "ALT.", LEFT_MODE);

	//int throttle_map = map(throttle, 875, 3300, 0, 100);


	//float_to_string(throttle_map, throttle_str);
	BSP_LCD_DisplayStringAt(14, 240, (uint8_t *) "THRT", LEFT_MODE);
	BSP_LCD_DisplayStringAt(100, 240, (uint8_t *) throttle_str, LEFT_MODE);


	//GPS variables

	//snprintf(long_str, sizeof(long_str), "%f", -234.242);
	float_to_string(GPS.Longitude, long_str);
	BSP_LCD_DisplayStringAt(100, 80, (uint8_t *) long_str, LEFT_MODE);

	float_to_string(GPS.Latitude, latt_str);
	BSP_LCD_DisplayStringAt(100, 100, (uint8_t *) latt_str, LEFT_MODE);

	float_to_string(GPS.Speed, speed_str);
	BSP_LCD_DisplayStringAt(100, 120, (uint8_t *) speed_str, LEFT_MODE);

	float_to_string(GPS.Altitude, alt_str);
	BSP_LCD_DisplayStringAt(100, 140, (uint8_t *) alt_str, LEFT_MODE);

	///////////////////////////////// Top bar //////////////////////////////////////////

	BSP_LCD_SetBackColor(LCD_COLOR_DARKBLUE);

	BSP_LCD_DisplayStringAt(80, 10, (uint8_t *) "QUAD", RIGHT_MODE);
	BSP_LCD_DisplayStringAt(30, 10, (uint8_t *) "%", RIGHT_MODE);
	BSP_LCD_DisplayStringAt(14, 10, (uint8_t *) "TX", LEFT_MODE);
	BSP_LCD_DisplayStringAt(70, 10, (uint8_t *) "%", LEFT_MODE);

	float_to_string(IMU.batteryLevel, quad_battery_str);
	BSP_LCD_DisplayStringAt(45, 10, (uint8_t *) quad_battery_str, RIGHT_MODE);

	itoa(69, tx_battery_str, 10);
	BSP_LCD_DisplayStringAt(44, 10, (uint8_t *) tx_battery_str, LEFT_MODE);

	BSP_LCD_DisplayStringAt(140, 10, (uint8_t *) "10:43", LEFT_MODE);

	if (Misc.connection) {
		BSP_LCD_DisplayStringAt(220, 10, (uint8_t *) "CON", LEFT_MODE);

	} else{
		BSP_LCD_DisplayStringAt(220, 10, (uint8_t *) "    ", LEFT_MODE);

	}

	if (Misc.airmode) {
		BSP_LCD_DisplayStringAt(280, 10, (uint8_t *) "AIR", LEFT_MODE);

	} else{
		BSP_LCD_DisplayStringAt(280, 10, (uint8_t *) "    ", LEFT_MODE);

	}

	/////////////// RPY /////////

	BSP_LCD_DrawCircle(400, 120, 40);
	BSP_LCD_SetTextColor(LCD_COLOR_DARKRED);
	BSP_LCD_FillCircle(400, 120, 39);

	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	BSP_LCD_FillCircle(400, 120, 5);

	BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
	BSP_LCD_DisplayStringAt(330, 200, (uint8_t *) "ROLL", LEFT_MODE);
	BSP_LCD_DisplayStringAt(330, 220, (uint8_t *) "PITCH", LEFT_MODE);
	BSP_LCD_DisplayStringAt(330, 240, (uint8_t *) "YAW", LEFT_MODE);

	float_to_string(IMU.roll, roll_str);
	float_to_string(IMU.pitch, pitch_str);
	float_to_string(IMU.yaw, yaw_str);

	BSP_LCD_DisplayStringAt(20, 200, (uint8_t *) "      ", RIGHT_MODE);
	BSP_LCD_DisplayStringAt(20, 220, (uint8_t *) "      ", RIGHT_MODE);
	BSP_LCD_DisplayStringAt(20, 240, (uint8_t *) "      ", RIGHT_MODE);

	BSP_LCD_DisplayStringAt(20, 200, (uint8_t *) roll_str, RIGHT_MODE);
	BSP_LCD_DisplayStringAt(20, 220, (uint8_t *) pitch_str, RIGHT_MODE);
	BSP_LCD_DisplayStringAt(20, 240, (uint8_t *) yaw_str, RIGHT_MODE);



	if (Misc.kill) {

		BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
		BSP_LCD_SetTextColor(LCD_COLOR_RED);
		BSP_LCD_SetFont(&Font20);

		BSP_LCD_DisplayStringAt(0, 80,
				(uint8_t *) "EMERGENCY KILL ON", CENTER_MODE);
		cleared = 0;
	} else {
		if (!cleared) {
			BSP_LCD_Clear(LCD_COLOR_BLACK);
			BSP_LCD_SetTextColor(LCD_COLOR_DARKBLUE);
			BSP_LCD_FillRect(0, 0, 480, 40);
		}
		cleared = 1;
	}

}

void drawIMUScreen() {

}

/*** Convert float to string ***/
void float_to_string(float f, char r[]) {
	long long int length, length2, i, number, position, sign;
	float number2;

	sign = -1;   // -1 == positive number
	if (f < 0) {
		sign = '-';
		f *= -1;
	}

	number2 = f;
	number = f;
	length = 0;  // Size of decimal part
	length2 = 0; // Size of tenth

	/* Calculate length2 tenth part */
	while ((number2 - (float) number) != 0.0
			&& !((number2 - (float) number) < 0.0)) {
		number2 = f * (n_tu(10.0, length2 + 1));
		number = number2;

		length2++;
	}

	/* Calculate length decimal part */
	for (length = (f > 1) ? 0 : 1; f > 1; length++)
		f /= 10;

	position = length;
	length = length + 1 + length2;
	number = number2;
	if (sign == '-') {
		length++;
		position++;
	}

	for (i = length; i >= 0; i--) {
		if (i == (length))
			r[i] = '\0';
		else if (i == (position))
			r[i] = '.';
		else if (sign == '-' && i == 0)
			r[i] = '-';
		else {
			r[i] = (number % 10) + '0';
			number /= 10;
		}
	}
}

/** Number on countu **/

int n_tu(int number, int count) {
	int result = 1;
	while (count-- > 0)
		result *= number;

	return result;
}


float map(int x, int in_min, int in_max, int out_min, int out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
