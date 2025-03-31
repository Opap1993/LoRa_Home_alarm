//
//
//

#include "Sensors.h"

void SensorsClass::init(Stream *serial, EnergySaveClass *energySave)
{
	debug = serial;
	energy = energySave;
	analogReadResolution(12);
}

void SensorsClass::readBME() {
	bme280.setForcedMode();
	
	measurements.bme.temperature = bme280.getTemperature();
	measurements.bme.humidity = bme280.getHumidity();
	measurements.bme.pressure = bme280.getPressure();
}

void SensorsClass::initI2CDevices(){
	bme280.init();
}

void SensorsClass::readAll(int samples, unsigned long ms) {
	energy->enableI2CDevices();
	delay(1000);
	initI2CDevices();
	
	warmUp(ms);
	readBatteryVoltage();
	readBME();
	readAdc(samples);
	
	energy->disableI2CDevices();
	
	print();
}

void SensorsClass::warmUp(unsigned long delay_ms) {
	unsigned long delay_start = millis();
	debug->print(F("Response Time :"));
	debug->println(delay_ms);
	while( (millis() - delay_start) < delay_ms );
}

float SensorsClass::readBatteryVoltage(void)
{
	pinMode(CHARGE_STATUS_PIN, OUTPUT);
	digitalWrite(CHARGE_STATUS_PIN, LOW);
	delay(100);
	measurements.battery = (float) analogRead(BATTERY_POWER_PIN) / 4095  * 3.3 * 11.0;
	pinMode(CHARGE_STATUS_PIN, INPUT);
	
	return measurements.battery;
}

void SensorsClass::readAdc(int samples) {
	int count = samples;
	
	unsigned long sums[4] = {0};
	
	#if DEBUG
	debug->println(F("Read All Sensors"));
	#endif
	
	for (int i = 0; i < count; i++) {
		sums[0] += analogRead(A0);
		sums[1] += analogRead(A1);
		sums[2] += analogRead(A2);
		sums[3] += analogRead(A3);
	}

	measurements.adc[0] = round(sums[0] / samples);
	measurements.adc[1] = round(sums[1] / samples);
	measurements.adc[2] = round(sums[2] / samples);
	measurements.adc[3] = round(sums[3] / samples);
}

void SensorsClass::sensorPulse(){
	SerialUSB.println(F("Pulse"));
	measurements.sensorCounter += 1;
}

bool SensorsClass::doorCheck(){ //door proper closing check
	if(measurements.sensorCounter % 2 == 0){
		return true;
	}
	else {
		return false;
	}
}

void SensorsClass::print(){
	
	debug->print(F("Battery :"));
	debug->println(measurements.battery);
	
	debug->print(F("BME Temperature :"));
	debug->println(measurements.bme.temperature);
	
	debug->print(F("BME Humidity :"));
	debug->println(measurements.bme.humidity);
	
	debug->print(F("BME Pressure :"));
	debug->println(measurements.bme.pressure);
	
	debug->print(F("Channel 0:"));
	debug->println(measurements.adc[0]);
	
	debug->print(F("Channel 1:"));
	debug->println(measurements.adc[1]);
	
	debug->print(F("Channel 2:"));
	debug->println(measurements.adc[2]);
	
	debug->print(F("Channel 3:"));
	debug->println(measurements.adc[3]);
	
	debug->print(F("sensorCounter:"));
	debug->println(measurements.sensorCounter);

}

sensors_measurements SensorsClass::getMeasurements() {
	return measurements;
}

SensorsClass Sensors;

