#include "libs/EnergySave/EnergySave.h"
#include "libs/Sensors/Sensors.h"
#include "libs/Network/Network.h"
#include "libs/LoRaWan/LoRaWan.h"
#include "libs/RTCZero/RTCZero.h"
unsigned long interval = 180000;
RTCZero rtc;
bool timeInterruptFlag = true; //time interrupt flag for interval maintenance

void setup()
{
	SerialUSB.println("Setup");
	EnergySave.init(&SerialUSB);
	Sensors.init(&SerialUSB, &EnergySave);
	EnergySave.lowPowerPins(false);
	EnergySave.disableI2CDevices();
    pinMode(DOOR_SENSOR, OUTPUT); //alarm
	pinMode(3,INPUT_PULLUP);
	delay(500);
	attachInterrupt(digitalPinToInterrupt(3), countPulse, CHANGE); //Pulse pin
	delay(500);
	EnergySave.setPinSleep(countPulse);
	delay(200);
	EnergySave.setMCUSleepMode(onWakeUp);
	EnergySave.setAlarmDateTime(DEFAULT_ALARM_HOUR, DEFAULT_ALARM_MINUTE, DEFAULT_ALARM_SEC);
	
	LoraWan.init(&SerialUSB);
	//LoraWan.setId(DEV_ADDR,DEV_EUI,APP_EUI); uncommend to manualy set keys
	Network.init(&SerialUSB, &LoraWan);
	Network.setup(10);
	
	
}

void loop()
{
	
	if (timeInterruptFlag) {
		timeInterruptFlag = false;
		Sensors.readAll(ADC_SAMPLES, SENSORS_WARM_UP);
	if (Network.connected()) {
		sendMeasurement(); //sends uplink and checks for alarm
        alarmRing();
		} else {
			Network.setup(10);
		if (Network.connected()) {
				sendMeasurement();
                alarmRing();
			}
	}
	
	EnergySave.setTime(0,0,0);
	EnergySave.standByMode();
	}
}


void sendMeasurement() {
	//Sensors.readAll(ADC_SAMPLES, SENSORS_WARM_UP);
	Network.transmitMeasurement(Sensors.getMeasurements());
}

void countPulse() {
	Sensors.sensorPulse();
}


void onWakeUp(){
	timeInterruptFlag = true;
}

void alarmRing(){
    while (Sensors.doorCheck()){
        digitalWrite(DOOR_SENSOR, HIGH);
    }
}