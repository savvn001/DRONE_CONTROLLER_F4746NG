
/*
 *  	A header file to hold the structs containing the data that is unpacked by reciever.
 * 		Have declared it here so NRF24_top and UI drivers can access it (using mutexes of course)
 *
 */

#ifndef UNPACK_VARIABLES_H_
#define UNPACK_VARIABLES_H_



struct IMU_str{

	uint16_t batteryLevel;
	float roll;
	float pitch;
	float yaw;

};

struct IMU_str IMU;


struct GPS_str
{
	uint8_t Day;
	uint8_t Month;
	uint8_t Year;

	uint8_t Hours;
	uint8_t Minutes;
	uint8_t Seconds;

	float Longitude;
	float Latitude;
	float Speed;
	float Altitude;

	uint8_t sattelite_no;
	uint8_t fix_quality;
};


struct GPS_str GPS;


#endif /* UNPACK_VARIABLES_H_ */
