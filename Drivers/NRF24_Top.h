/*
 * NRF24_Top.h
 *
 *  Created on: Sep 22, 2019
 *      Author: nick_savva
 */

#ifndef NRF24_TOP_H_
#define NRF24_TOP_H_

#include "../Drivers/MY_NRF24.h"

#include "usart.h"
#include "../Drivers/dwt_delay.h"
#include "../Drivers/unpack_variables.h"
#include <stdbool.h>
#include <math.h>

SPI_HandleTypeDef none;

//NRF24 Module variables
uint64_t TxpipeAddrs = 0x11223344AA;
unsigned char TxData[32];
uint8_t AckPayload[32];

void packData(uint32_t * array, bool airmode, bool kill, float *enc_pid);
void unpackAckPayload(struct GPS_str GPS, struct IMU_str IMU);

void unpackAckPayload_0();
void unpackAckPayload_1();

void NRF24_init(struct GPS_str GPS) {

	DWT_Init(); //For uS delays
	NRF24_begin(GPIOA, nrf_CSN_PIN, nrf_CE_PIN, none);
	nrf24_DebugUART_Init(huart1);

	//**** TRANSMIT - ACK ****//
	NRF24_stopListening();
	NRF24_openWritingPipe(TxpipeAddrs);

	//printRadioSettings();

	for (int i = 0; i < 31; ++i) {
		TxData[i] = 0;
		AckPayload[i] = 0;
	}

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

bool sendPayload() {

	//Transmit payload to drone
	if (NRF24_write(TxData, 32)) {

		NRF24_read(AckPayload, 32);

		//Check to see which packet we received
		switch (AckPayload[0]) {

		//This is first packet containing battery and IMU info
		case 0x03:

			unpackAckPayload_0();

			break;

			//Second packet containing the GPS info
		case 0xFF:

			unpackAckPayload_1();

			break;
		default:
			break;
		}

		return 1;
	} else {
		return 0;
	}

}

void packData(uint32_t * array, bool airmode, bool kill, float *enc_pid) {


//	array[1] = 4096 - array[1];
//	array[2] = 4096 - array[2];

	//L joystick X
	TxData[0] = array[0];
	TxData[1] = array[0] >> 8;

	//L joystick Y
	TxData[2] = array[3];
	TxData[3] = array[3] >> 8;

	//R joystick X
	TxData[4] = array[2];
	TxData[5] = array[2] >> 8;

	//R joystick Y
	TxData[6] = array[1];
	TxData[7] = array[1] >> 8;


	//Air mode bit
	if (airmode) {
		TxData[8] |= 1 << 0;
	} else {
		TxData[8] &= ~(1 << 0);
	}

	//Kill switch bit
	if (kill) {

		TxData[8] |= 1 << 1;
	} else {
		TxData[8] &= ~(1 << 1);
	}

	if(TxData[8]  > 0x03 ){
		while(1);
	}

		uint16_t roll_p_tx = round(enc_pid[0] * 100);
		uint16_t roll_i_tx = round(enc_pid[1] * 100) ;
		uint16_t roll_d_tx = round(enc_pid[2] * 100) ;

		TxData[9] = roll_p_tx;
		TxData[10] = roll_p_tx >> 8;

		TxData[11] = roll_i_tx;
		TxData[12] = roll_i_tx >> 8;

		TxData[13] = roll_d_tx;
		TxData[14] = roll_d_tx >> 8;

}

void unpackAckPayload_0() {

	IMU.batteryLevel = (AckPayload[1] & 0xFF) | (AckPayload[2] << 8);

	int16_t roll_rx = (AckPayload[3] & 0xFF) | (AckPayload[4] << 8);
	IMU.roll = roll_rx / 100;

	int16_t pitch_rx = (AckPayload[5] & 0xFF) | (AckPayload[6] << 8);
	IMU.pitch = pitch_rx / 100;

	int16_t yaw_rx_ = (AckPayload[7] & 0xFF) | (AckPayload[8] << 8);
	IMU.yaw = yaw_rx_ / 100;
}

void unpackAckPayload_1() {

	//All the 1 byte values
	GPS.sattelite_no = AckPayload[1];
	GPS.fix_quality = AckPayload[2];
	GPS.Day = AckPayload[3];
	GPS.Month = AckPayload[4];
	GPS.Year = AckPayload[5];
	GPS.Hours = AckPayload[6];
	GPS.Minutes = AckPayload[7];
	GPS.Seconds = AckPayload[8];
	//GPS speed
	uint16_t speed_rx = (AckPayload[9] & 0xFF) | (AckPayload[10] << 8);
	GPS.Speed = speed_rx / 100;

	//Longitude, latitude and altitude
	unsigned char temp[4] = { 0, 0, 0, 0 };

	temp[0] = AckPayload[11];
	temp[1] = AckPayload[12];
	temp[2] = AckPayload[13];
	temp[3] = AckPayload[14];

	memcpy(&GPS.Longitude, temp, sizeof(float));

	temp[0] = AckPayload[15];
	temp[1] = AckPayload[16];
	temp[2] = AckPayload[17];
	temp[3] = AckPayload[18];

	memcpy(&GPS.Latitude, temp, sizeof(float));

	temp[0] = AckPayload[19];
	temp[1] = AckPayload[20];
	temp[2] = AckPayload[21];
	temp[3] = AckPayload[22];

	memcpy(&GPS.Altitude, temp, sizeof(float));
}

#endif /* NRF24_TOP_H_ */
