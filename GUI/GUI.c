/*
 * UI.c
 *
 *  Created on: Sep 15, 2019
 *      Author: nick_savva
 */

#include <GUI.h>
#include  "time.h"
#include "../GUI/Images/icons.h"
#include <math.h>

//Touch screen variables
TS_StateTypeDef ts;
char xTouchStr[10];

bool cleared = 0;

char time_buffer[8];
char date_buffer[12];

uint16_t roll_plot[480];
uint16_t pitch_plot[480];
uint16_t yaw_plot[480];

void initLCD(struct GPS_str GPS) {

	BSP_SDRAM_Init(); /* Initializes the SDRAM device */
	__HAL_RCC_CRC_CLK_ENABLE()
	; /* Enable the CRC Module */

	BSP_TS_Init(480, 272);

	BSP_LCD_Init();
	BSP_LCD_LayerDefaultInit(0, LCD_FB_START_ADDRESS);
	BSP_LCD_DisplayOn();

	BSP_LCD_SelectLayer(0);
	BSP_LCD_Clear(LCD_COLOR_BLACK);

	BSP_LCD_SetTextColor(LCD_COLOR_DARKBLUE);
	BSP_LCD_FillRect(0, 0, 480, 40);

	GPS.Altitude = 0.00;
	GPS.Day = 0;
	GPS.Hours = 0;
	GPS.Latitude = 0.00;
	GPS.Longitude = 0x0000;
	GPS.Minutes = 0;
	GPS.Month = 0;
	GPS.Seconds = 0;
	GPS.Speed = 0.00;
	GPS.Year = 0;
	GPS.fix_quality = 0;
	GPS.sattelite_no = 0;

}

char latt_str[48];
char long_str[10];
char speed_str[48];
char alt_str[48];
char sattelite_no_str[5];
char fix_str[48];
char throttle_str[2];

char quad_battery_str[48];
char tx_battery_str[48];

char roll_str[6];
char pitch_str[6];
char yaw_str[6];

/*	Current screen for state machine...
 * 	0 = main
 *  1 = settings
 *	2 = GPS info
 * 	3 = IMU graphs
 */
int screen = 0;

void drawUI(struct GPS_str GPS, struct IMU_str IMU, struct Misc_str Misc,
		float *enc_pid, uint8_t *tune_axis) {

	BSP_TS_GetState(&ts);

	sprintf(xTouchStr, "X: %3d", ts.touchX[0]);
	//BSP_LCD_DisplayStringAt(20, 20, (uint8_t *) xTouchStr, LEFT_MODE);

	sprintf(xTouchStr, "Y: %3d", ts.touchY[0]);
	//BSP_LCD_DisplayStringAt(20, 60, (uint8_t *) xTouchStr, LEFT_MODE);

	switch (screen) {
	case 0:
		drawMainScreen(GPS, IMU, Misc, enc_pid);
		break;
	case 1:
		drawSettingsScreen();
		break;
	case 3:
		drawPIDscreen(IMU, enc_pid, tune_axis);
		break;
	default:
		break;
	}

}

void drawMainScreen(struct GPS_str GPS, struct IMU_str IMU,
		struct Misc_str Misc, float *enc_pid) {

	///////////////////////////////// GPS section //////////////////////////////////////////

	BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);

	BSP_LCD_SetFont(&FontAvenir12);

	BSP_LCD_DisplayStringAt(30, 50, (uint8_t *) "GPS", LEFT_MODE);
	BSP_LCD_DisplayStringAt(14, 80, (uint8_t *) "LONG.", LEFT_MODE);
	BSP_LCD_DisplayStringAt(14, 100, (uint8_t *) "LAT.", LEFT_MODE);
	BSP_LCD_DisplayStringAt(14, 140, (uint8_t *) "SPEED.", LEFT_MODE);
	BSP_LCD_DisplayStringAt(14, 120, (uint8_t *) "ALT.", LEFT_MODE);
	BSP_LCD_DisplayStringAt(14, 160, (uint8_t *) "SATS.", LEFT_MODE);
	BSP_LCD_DisplayStringAt(14, 180, (uint8_t *) "FIX", LEFT_MODE);

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
	BSP_LCD_DisplayStringAt(100, 140, (uint8_t *) speed_str, LEFT_MODE);

	float_to_string(GPS.Altitude, alt_str);
	BSP_LCD_DisplayStringAt(100, 120, (uint8_t *) alt_str, LEFT_MODE);

	//float_to_string(GPS.sattelite_no, sattelite_no_str);

	snprintf(sattelite_no_str, sizeof(sattelite_no_str), "%d",
			GPS.sattelite_no);

	BSP_LCD_DisplayStringAt(100, 160, (uint8_t *) sattelite_no_str, LEFT_MODE);

	float_to_string(GPS.fix_quality, fix_str);
	BSP_LCD_DisplayStringAt(100, 180, (uint8_t *) fix_str, LEFT_MODE);

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

	/////////////////// Time and Date ////////////////
	struct tm time;

	time.tm_hour = GPS.Hours + 1;
	time.tm_min = GPS.Minutes;
	time.tm_sec = GPS.Seconds;
	time.tm_mday = GPS.Day;
	time.tm_mon = GPS.Month - 1;
	time.tm_isdst = 0;

	strftime(time_buffer, 12, "%H:%M:%S", &time);

	strftime(date_buffer, 8, "%d/%m", &time);

	//BSP_LCD_DisplayStringAt(100, 10, (uint8_t *) date_buffer, LEFT_MODE);
	BSP_LCD_DisplayStringAt(140, 10, (uint8_t *) time_buffer, LEFT_MODE);

	if (Misc.connection) {
		BSP_LCD_DisplayStringAt(250, 10, (uint8_t *) "CON", LEFT_MODE);

	} else {
		BSP_LCD_DisplayStringAt(250, 10, (uint8_t *) "    ", LEFT_MODE);

	}

	if (Misc.airmode) {
		BSP_LCD_DisplayStringAt(300, 10, (uint8_t *) "AIR", LEFT_MODE);

	} else {
		BSP_LCD_DisplayStringAt(300, 10, (uint8_t *) "    ", LEFT_MODE);

	}

	/////////////// RPY /////////////////////////////////////////

	BSP_LCD_DrawCircle(400, 120, 40);
	BSP_LCD_SetTextColor(LCD_COLOR_DARKRED);
	BSP_LCD_FillCircle(400, 120, 39);

	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	BSP_LCD_FillCircle(400, 120, 5);

	BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
	BSP_LCD_DisplayStringAt(200, 90, (uint8_t *) "ROLL", LEFT_MODE);
	BSP_LCD_DisplayStringAt(200, 110, (uint8_t *) "PITCH", LEFT_MODE);
	BSP_LCD_DisplayStringAt(200, 130, (uint8_t *) "YAW", LEFT_MODE);

	float_to_string(IMU.roll, roll_str);
	float_to_string(IMU.pitch, pitch_str);
	float_to_string(IMU.yaw, yaw_str);

	BSP_LCD_DisplayStringAt(300, 90, (uint8_t *) roll_str, LEFT_MODE);
	BSP_LCD_DisplayStringAt(300, 110, (uint8_t *) pitch_str, LEFT_MODE);
	BSP_LCD_DisplayStringAt(300, 130, (uint8_t *) yaw_str, LEFT_MODE);

	if (Misc.kill) {

		BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
		BSP_LCD_SetTextColor(LCD_COLOR_RED);
		BSP_LCD_SetFont(&Font20);

		BSP_LCD_DisplayStringAt(0, 80, (uint8_t *) "EMERGENCY KILL ON",
				CENTER_MODE);
		cleared = 0;
	} else {
		if (!cleared) {
			BSP_LCD_Clear(LCD_COLOR_BLACK);
			BSP_LCD_SetTextColor(LCD_COLOR_DARKBLUE);
			BSP_LCD_FillRect(0, 0, 480, 40);
		}
		cleared = 1;
	}

	Button settings;

	settings.txt = "Settings";
	settings.txt_l = 4;
	settings.txt_offset = 25;
	settings.x = 380;
	settings.y = 220;
	settings.width = 160;
	settings.height = 40;
	settings.fontSize = 12;
	createButton(settings, &settings.press);


	BSP_TS_Get_GestureId(&ts);

	if (ts.gestureId == GEST_ID_MOVE_RIGHT) {
		BSP_LCD_Clear(LCD_COLOR_BLACK);
		screen = 1;
	}

	if (settings.press) {
		BSP_LCD_Clear(LCD_COLOR_BLACK);
		settings.press = false;
		screen = 1;
	}

}
void drawSettingsScreen() {

	Button pid_button;

	pid_button.txt = "PID";
	pid_button.txt_l = 4;
	pid_button.txt_offset = 55;
	pid_button.x = 150;
	pid_button.y = 40;
	pid_button.width = 240;
	pid_button.height = 50;
	pid_button.fontSize = 16;
	createButton(pid_button, &pid_button.press);

	//Check if PID button pressed
	if (pid_button.press) {
		pid_button.press = false;
		screen = 3;
		BSP_LCD_Clear(LCD_COLOR_BLACK);
	}

}

int pitch_index = 0;
int yaw_index = 0;
char roll_p_str[4];
char roll_i_str[4];
char roll_d_str[4];
char pitch_p_str[4];
char pitch_i_str[4];
char pitch_d_str[4];
void drawPIDscreen(struct IMU_str IMU, float *enc_pid, uint8_t *tune_axis) {

	Button pid_roll_button;

	pid_roll_button.txt = "Roll";
	pid_roll_button.txt_l = 4;
	pid_roll_button.txt_offset = 25;
	pid_roll_button.x = 0;
	pid_roll_button.y = 240;
	pid_roll_button.width = 120;
	pid_roll_button.height = 35;
	pid_roll_button.fontSize = 12;
	createButton(pid_roll_button, &pid_roll_button.press);

	Button pid_pitch_button;

	pid_pitch_button.txt = "Pitch";
	pid_pitch_button.txt_l = 4;
	pid_pitch_button.txt_offset = 25;
	pid_pitch_button.x = 120;
	pid_pitch_button.y = 240;
	pid_pitch_button.width = 120;
	pid_pitch_button.height = 35;
	pid_pitch_button.fontSize = 12;
	createButton(pid_pitch_button, &pid_pitch_button.press);

	Button pid_yaw_button;

	pid_yaw_button.txt = "yaw";
	pid_yaw_button.txt_l = 4;
	pid_yaw_button.txt_offset = 25;
	pid_yaw_button.x = 240;
	pid_yaw_button.y = 240;
	pid_yaw_button.width = 120;
	pid_yaw_button.height = 35;
	pid_yaw_button.fontSize = 12;
	createButton(pid_yaw_button, &pid_yaw_button.press);

	Button pid_all_button;

	pid_all_button.txt = "All";
	pid_all_button.txt_l = 4;
	pid_all_button.txt_offset = 25;
	pid_all_button.x = 360;
	pid_all_button.y = 240;
	pid_all_button.width = 120;
	pid_all_button.height = 35;
	pid_all_button.fontSize = 12;
	createButton(pid_all_button, &pid_all_button.press);

	BSP_LCD_DisplayStringAt(420, 50, (uint8_t *) "P:", LEFT_MODE);
	BSP_LCD_DisplayStringAt(420, 80, (uint8_t *) "I:", LEFT_MODE);
	BSP_LCD_DisplayStringAt(420, 110, (uint8_t *) "D:", LEFT_MODE);

	/*******************	 Plot roll angles 	*****************/
	if (pid_roll_button.press) {
		pid_roll_button.press = false;
		*tune_axis = 0;
		BSP_LCD_Clear(LCD_COLOR_BLACK);

	}

	if (pid_pitch_button.press) {
		pid_pitch_button.press = false;
		*tune_axis = 3;
		BSP_LCD_Clear(LCD_COLOR_BLACK);
	}

	switch (*tune_axis) {
	case 0:

		float_to_string(enc_pid[0], roll_p_str);
		drawFloatString(roll_p_str, 440, 50);

		float_to_string(enc_pid[1], roll_i_str);
		drawFloatString(roll_i_str, 440, 80);

		float_to_string(enc_pid[2], roll_d_str);
		drawFloatString(roll_d_str, 440, 110);

		createGraph(0, roll_plot);

		break;

	case 3:

		float_to_string(enc_pid[3], pitch_p_str);
		drawFloatString(pitch_p_str, 440, 50);

		float_to_string(enc_pid[4], pitch_i_str);
		drawFloatString(pitch_i_str, 440, 80);

		float_to_string(enc_pid[5], pitch_d_str);
		drawFloatString(pitch_d_str, 440, 110);

		createGraph(0, pitch_plot);

	default:
		break;
	}
}

int cur_index = 0;

void createGraph(float input, uint16_t *plot_data) {

	int16_t px_y;

	for (int var = 10; var <= 190; var += 45) {
		BSP_LCD_DrawHLine(0, var, 400);
	}

	BSP_LCD_DisplayStringAt(0, 115, (uint8_t *) "0", LEFT_MODE);
	BSP_LCD_DisplayStringAt(0, 10, (uint8_t *) "90", LEFT_MODE);
	BSP_LCD_DisplayStringAt(0, 220, (uint8_t *) "-90", LEFT_MODE);

	px_y = map(input, -45, 45, 10, 190);

	//Newest data goes on last free element
	if (cur_index < 400) {
		plot_data[cur_index] = px_y;
		cur_index++;
	} else {
		//Shift array left
		int temp = 0;

		temp = roll_plot[0];
		for (int i = 0; i < 480 - 1; i++) {
			plot_data[i] = plot_data[i + 1];
		}
		plot_data[400 - 1] = temp;

		//Add newest element
		plot_data[379] = px_y;
	}

	for (int j = 0; j < 400; ++j) {
		BSP_LCD_DrawPixel(j, plot_data[j], LCD_COLOR_GREEN);
	}

}

void createButton(Button button_str, bool *pressed) {

	switch (button_str.fontSize) {
	case 12:
		BSP_LCD_SetFont(&FontAvenir12);
		break;
	case 16:
		BSP_LCD_SetFont(&FontAvenir16);
		break;
	default:
		break;
	}

	BSP_LCD_DrawRect(button_str.x, button_str.y, button_str.width,
			button_str.height);
	BSP_LCD_DisplayStringAt(button_str.x + button_str.txt_offset,
			button_str.y + (button_str.height / 3), (uint8_t *) button_str.txt,
			LEFT_MODE);

	if (ts.touchDetected) {

		if (ts.touchX[0] > button_str.x
				&& ts.touchX[0] < (button_str.x + button_str.width)
				&& ts.touchY[0] > button_str.y
				&& ts.touchY[0] < (button_str.y + button_str.height)) {
			*pressed = true;

		}
	} else {
		*pressed = false;
	}

}

/*** Convert float to string ***/
bool float_to_string(float f, char r[]) {
	long long int length, length2, i, number, position, sign;
	float number2;

	//If number is invalid for whatever reason exit to avoid a crash
	if (isnan(f)) {
		return 0;
	}

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

	return 1;
}

/** Number on countu **/

int n_tu(int number, int count) {
	int result = 1;
	while (count-- > 0)
		result *= number;

	return result;
}

float map(float x, float in_min, float in_max, float out_min, float out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

char *dtostrf(double val, signed char width, unsigned char prec, char *sout) {
	char fmt[20];
	sprintf(fmt, "%%%d.%df", width, prec);
	sprintf(sout, fmt, val);
	return sout;
}

void drawFloatString(char *array, int16_t xpos, int16_t ypos) {
	BSP_LCD_DisplayChar(xpos, ypos, array[0]);
	BSP_LCD_DisplayChar(xpos + 10, ypos, array[1]);
	BSP_LCD_DisplayChar(xpos + 20, ypos, array[2]);
	BSP_LCD_DisplayChar(xpos + 30, ypos, array[3]);
}
