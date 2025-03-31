// Sensors.h

#ifndef _SENSORS_h
#define _SENSORS_h

#include "../EnergySave/EnergySave.h"
#include "../BME280/BME280.h"

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class SensorsClass
{
 protected:
	Stream *debug;
    EnergySaveClass *energy;
	
	sensors_measurements measurements;
	BME280 bme280;
	
	
 public:
	void init(Stream *serial, EnergySaveClass *energySave);
	void readBME();
	void initI2CDevices();
	void readAll(int samples, unsigned long ms);
	void warmUp(unsigned long delay_ms);
	float readBatteryVoltage(void);
	void readAdc(int samples);
	void print();
	void sensorPulse();
	bool doorCheck();
	sensors_measurements getMeasurements();
};

extern SensorsClass Sensors;

#endif

