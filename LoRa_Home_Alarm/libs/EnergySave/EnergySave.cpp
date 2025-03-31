#include "EnergySave.h"
#include "../LoRaWan/LoRaWan.h"

void EnergySaveClass::init(Stream *serial)
{
	debug = serial;
	
}

void EnergySaveClass::setMCUSleepMode(voidFuncPtr alarmMatchCallback){
	#if DEBUG
	debug->println("Setup MCU Time Sleep Mode");
	#endif
	
	rtc.begin();

	rtc.setTime(0, 0, 0);
	
	rtc.setDate(0, 0, 2000);

	rtc.enableAlarm(rtc.MATCH_HHMMSS); // MATCH_HHMMSS
	
	rtc.attachInterrupt(alarmMatchCallback);
	
	SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;

}

void EnergySaveClass::setPinSleep(voidFuncPtr pulseCallback) {
	#if DEBUG
	debug->println("Setup MCU Pin Sleep Mode");
	#endif
	

	NVMCTRL->CTRLB.bit.SLEEPPRM = 3;

	
	pinMode(3,INPUT_PULLUP);
	attachInterrupt(digitalPinToInterrupt(3), pulseCallback, CHANGE);
	EIC->WAKEUP.bit.WAKEUPEN6 = 1;
	
	GCLK->CLKCTRL.bit.CLKEN = 0; //disable GCLK module
	while (GCLK->STATUS.bit.SYNCBUSY);

	GCLK->CLKCTRL.reg = (uint16_t) (GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK6 | GCLK_CLKCTRL_ID( GCM_EIC )) ;  //EIC clock switched on GCLK6
	while (GCLK->STATUS.bit.SYNCBUSY);

	GCLK->GENCTRL.reg = (GCLK_GENCTRL_GENEN | GCLK_GENCTRL_SRC_OSCULP32K | GCLK_GENCTRL_ID(6));  //source for GCLK6 is OSCULP32K
	while (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY);

	GCLK->GENCTRL.bit.RUNSTDBY = 1;  //GCLK6 run standby
	while (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY);

	SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
}

void EnergySaveClass::standByMode(){
	#if DEBUG
	debug->println("Stand By Mode");
	#endif
	pinMode(A1,OUTPUT);
	digitalWrite(A1,HIGH);
	delay(500);
	digitalWrite(A1,LOW);
	
	rtc.standbyMode();
	
}

void EnergySaveClass::setTime(uint8_t h, uint8_t m, uint8_t s) {
	rtc.setTime(h, m, s);
}

void EnergySaveClass::setAlarmDateTime(uint8_t h, uint8_t m, uint8_t s){
	rtc.setTime(0, 0, 0);
	rtc.setAlarmTime(h, m, s);
	debug->print("EnergySave Set  Alarm ");
	debug->print(h);
	debug->print(":");
	debug->print(m);
	debug->print(":");
	debug->println(s);
}

void EnergySaveClass::lowPowerPins(bool led_removed){
	#if DEBUG
	debug->println("Low power pins");
	#endif
	for (unsigned char i = 0; i < 26; i++) // important, set all pins to HIGH to save power
	{
		if( i != 18 && i != 19 && i !=3){
			pinMode(i, OUTPUT);
			digitalWrite(i, HIGH);
		}
	}
	
	if(!led_removed){
		digitalWrite(13, LOW);
		pinMode(A1,OUTPUT);
		digitalWrite(A1,HIGH);
		delay(500);
		digitalWrite(A1,LOW);
	}
}

void EnergySaveClass::enableI2CDevices(){
	#if DEBUG
	debug->println("Enable I2C Devices");
	#endif
	pinMode(I2C_VCC_ENABLE_PIN, OUTPUT);
	digitalWrite(I2C_VCC_ENABLE_PIN, HIGH);
}

void EnergySaveClass::disableI2CDevices()
{
	#if DEBUG
	debug->println("Disable I2C devices");
	#endif
	pinMode(I2C_VCC_ENABLE_PIN, OUTPUT);
	digitalWrite(I2C_VCC_ENABLE_PIN, LOW);
}

void EnergySaveClass::countPulse(){
	
}


EnergySaveClass EnergySave;

