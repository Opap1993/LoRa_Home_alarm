#ifndef PARAMETERS_H
#define PARAMETERS_H

// SEED BOARD VERSION 1.8.1

// ==== Soil ====
// Ground
// VCC
// A3 - Moisture - Yellow
// A2 - Temp - Green
// ========

// ==== Leaf ====
// Ground
// VCC
// A0 - Up
// A1 - Down
// ========

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#define SERIAL_BAUDRATE		115200
#define ANALOG_RESOLUTION		12

// ============= Energy Save
#define I2C_VCC_ENABLE_PIN		38
#define SENSORS_INIT_DELAY		5
#define I2C_VCC_ON_DELAY		2000
#define I2C_BOOTUP_DELAY		2000

#define ADC_SAMPLES				 1000
#define SENSORS_WARM_UP			 3000

// ============= LoRa WAN
#define API_KEY	"e490452821e09ede355f8d479b4fe4c0"
#define APP_EUI "70B3D57ED0023770"
//#define DEV_EUI "8cf9572000079af1" uncommend to manualy set keys
//#define DEV_ADDR "01 8c 3c 51" uncommend to manualy set keys

#define PAYLOAD_SIZE			10;
#define UPLINKS_TO_SEND_ON_BOOT	1;

#define	DEBUG			true

#define DEFAULT_ALARM_HOUR		1
#define DEFAULT_ALARM_MINUTE	0
#define DEFAULT_ALARM_SEC		0

#define DEFAULT_GPS_TIMEOUT		60000

#define BATTERY_POWER_PIN    A4
#define CHARGE_STATUS_PIN    A5

#define DOOR_SENSOR          8

struct bme_measurement {
	float temperature;
	float pressure;
	float humidity;	
};

struct gps_position {
	float lat;
	float lng;
};

struct sensors_measurements {
	uint16_t adc[4];
	bme_measurement bme;
	float battery;
	int sensorCounter = 0;
};

#endif