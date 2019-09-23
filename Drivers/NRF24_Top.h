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

SPI_HandleTypeDef none;

//NRF24 Module variables
uint64_t TxpipeAddrs = 0x11223344AA;
char TxData[32];
char AckPayload[32];

void packData(uint32_t * array, bool airmode, bool kill);
void unpackAckPayload(struct GPS_str GPS, struct IMU_str IMU);

void NRF24_init() {

	DWT_Init(); //For uS delays
	NRF24_begin(GPIOA, nrf_CSN_PIN, nrf_CE_PIN, none);
	nrf24_DebugUART_Init(huart1);

	//**** TRANSMIT - ACK ****//
	NRF24_stopListening();
	NRF24_openWritingPipe(TxpipeAddrs);

	//printRadioSettings();

	for (int i = 0; i < 31; ++i) {
		TxData[i] = 0;
	}
}

bool sendPayload() {

	//Transmit payload to drone
	if (NRF24_write(TxData, 32)) {

		NRF24_read(AckPayload, 32);

		return 1;
	} else {
		return 0;
	}

}

void packData(uint32_t * array, bool airmode, bool kill) {

	//L Joystick X
	//First byte = first byte of A0
	TxData[0] = *array;
	//Second bytes = last byte of A0
	TxData[1] = *array >> 8;

	array++;

	TxData[2] = *array;
	TxData[3] = *array >> 8;

	array++;

	TxData[4] = *array;
	TxData[5] = *array >> 8;

	array++;

	TxData[6] = *array;
	TxData[7] = *array >> 8;

	//Air mode bit
	if (airmode) {
		TxData[8] |= 1 << 0;
	} else {
		TxData[8] &= ~(1 << 0);
	}

	//Kill switch
	if (kill) {

		TxData[8] |= 1 << 1;
	} else {
		TxData[8] &= ~(1 << 1);
	}

//	uint16_t roll_p_tx = round(roll_p * 100);
//	uint16_t roll_i_tx = round(roll_i * 100) ;
//	uint16_t roll_d_tx = round(roll_d * 100) ;
//
//	TxData[9] = roll_p_tx;
//	TxData[10] = roll_p_tx >> 8;
//
//	TxData[11] = roll_i_tx;
//	TxData[12] = roll_i_tx >> 8;
//
//	TxData[13] = roll_d_tx;
//	TxData[14] = roll_d_tx >> 8;

}

void unpackAckPayload(struct GPS_str GPS, struct IMU_str IMU) {

	//Check if this is packet 0 or packet 1
	if (AckPayload[0] == 0x00) {

		IMU.batteryLevel = (AckPayload[1] & 0xFF) | (AckPayload[2] << 8);

		int16_t roll_rx = (AckPayload[3] & 0xFF) | (AckPayload[4] << 8);
		IMU.roll = roll_rx / 100;

		int16_t pitch_rx = (AckPayload[5] & 0xFF) | (AckPayload[6] << 8);
		IMU.pitch = pitch_rx / 100;

		int16_t yaw_rx_ = (AckPayload[7] & 0xFF) | (AckPayload[8] << 8);
		IMU.yaw = yaw_rx_ / 100;
	}
	//This is packet 1, only sent every 1s
	if (AckPayload[0] == 0xFF) {

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
		GPS.Longitude = (AckPayload[11] & 0xFFFF) | (AckPayload[12] << 8)
				| (AckPayload[13] << 16) | (AckPayload[14] << 24);

		GPS.Latitude = (AckPayload[15] & 0xFFFF) | (AckPayload[16] << 8)
				| (AckPayload[17] << 16) | (AckPayload[18] << 24);

		GPS.Latitude = (AckPayload[19] & 0xFFFF) | (AckPayload[20] << 8)
				| (AckPayload[21] << 16) | (AckPayload[22] << 24);

	}

}

#endif /* NRF24_TOP_H_ */
