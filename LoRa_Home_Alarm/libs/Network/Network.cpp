//
//
//

#include "Network.h"

void NetworkClass::init(Stream *serial, LoRaWanClass *loraWan)
{
	debug = serial;
	lora = loraWan;

	Codec.init(debug);
	
	unsigned long uplink_interval =  (DEFAULT_ALARM_HOUR * 3600 + DEFAULT_ALARM_MINUTE * 60  + DEFAULT_ALARM_SEC) * 1000;
	
	confirmed_interval = round(43200000 / uplink_interval);
	failed_before_rejon = round(21600000 / uplink_interval);
	
	debug->print(F("Uplink Interval :"));
	debug->println(uplink_interval);
	
	debug->print(F("Confirmed Interval :"));
	debug->println(confirmed_interval);
	
	debug->print(F("Rejoin After Ack Failed :"));
	debug->println(failed_before_rejon);
}

bool NetworkClass::connected() {
	return joined;
}

bool NetworkClass::setup(unsigned char timeout_s) {
	joined = false;
	
	debug->println(F("LoRa Setup"));
	lora->wakeup();
	
	lora->setId(NULL, NULL, AppEUI);
	lora->setKey(NULL, NULL, Appkey);
	
	lora->setDeciveMode(LWOTAA);
	lora->setAdaptiveDataRate(false);
	lora->setDataRate(DR0, EU868);
	
	lora->setChannel(0, 868.1);
	lora->setChannel(1, 868.3);
	lora->setChannel(2, 868.5);
	
	lora->setReceiceWindowFirst(0, 868.1);
	lora->setReceiceWindowSecond(869.5, DR0);

	lora->setDutyCycle(false);
	lora->setJoinDutyCycle(false);
	
	lora->setPower(14);
	
	joined = join(timeout_s);
	
	lora->sleep();
	
	return joined;
}

bool NetworkClass::join(unsigned char timeout_s){
	boolean joined = false;
	
	lora->wakeup();
	
	joined = lora->setOTAAJoin(JOIN, timeout_s);
	
	lora->sleep();
	
	if (joined) {
		SerialUSB.println("------------ Joined Succeed ---------- ");
		} else {
		SerialUSB.println("------------ Join Failed ---------- ");
	}
	
	return joined;
}


bool NetworkClass::ping() {
	uint8_t size = 0;
	byte buffer[2] = {0};
	
	if (!joined) {
		setup(15);
	}
	
	if (!joined) {
		return false;
	}
	
	debug->println("Ping");
	
	
	lora->wakeup();
	
	size = Codec.encode(buffer,Codec.PING, 2);
	lora->transferPacket(buffer, size, 5);
	
	lora->sleep();
	return true;
}

bool NetworkClass::transmitMeasurement(sensors_measurements measurements) {
	uint8_t size = 0;
	byte buffer[30] = {0};
	bool confirmed = false;
	unconfirmed_sent++;
	
	if (!joined) {
		setup(15);
	}
	
	if (!joined) {
		return false;
	}
	
	Codec.setMeasurements(measurements);
	
	
	Codec.encode(buffer, Codec.MEASUREMENT, 30);
	
	debug->print("Packet Size:");
	debug->println(size);
	
	lora->wakeup();
	
	if(unconfirmed_sent < confirmed_interval) {
		debug->print(F("Transmit Unconfirmed :"));
		debug->println(unconfirmed_sent);
		
		lora->transferPacket(buffer, size, 5);
		} else {
		confirmed = lora->transferPacketWithConfirmed(buffer, size , 5);
		
		if(confirmed) {
			unconfirmed_sent = 0;
			
			debug->println("Confirmed");
			} else {
			ack_failed += 1;
			debug->print(F("Ack Failed :"));
			debug->println(ack_failed);
			
			if(ack_failed >= failed_before_rejon) {
				ack_failed = 0;
				unconfirmed_sent = 0;
				joined = false;
			}
		}
	}
	
	
	lora->sleep();
	
	printPacket(buffer, size);
	return confirmed;
}

void NetworkClass::printPacket(byte *payload, int size) {
	for (int i = 0; i < size; i++) {
		debug->print(payload[i]);
		debug->print(" ");
	}
	debug->println();
}


NetworkClass Network;

