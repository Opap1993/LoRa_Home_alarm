// EnergySave.h

#ifndef _ENERGYSAVE_h
#define _ENERGYSAVE_h
#include "../../Parameters.h"
#include "../RTCZero/RTCZero.h"

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

class EnergySaveClass
{
	protected:
	RTCZero rtc;
	Stream *debug;
	
	int alarm_hour = 0;
	int alarm_sec = 0;
	int alarm_min = 0;

	public:
	void init(Stream *debug);
	void setMCUSleepMode(voidFuncPtr alarmMatchCallback);
	void setPinSleep(voidFuncPtr pulseCallback);
	void standByMode();
	void setTime(uint8_t h, uint8_t m, uint8_t s);
	void setAlarmDateTime(uint8_t h, uint8_t min, uint8_t s);
	void lowPowerPins(bool led_removed);
	void enableI2CDevices();
	void disableI2CDevices();
	static void countPulse();
};

extern EnergySaveClass EnergySave;

#endif

