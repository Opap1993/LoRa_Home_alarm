// Network.h

#ifndef _NETWORK_h
#define _NETWORK_h
#include "../Codec/Codec.h"
#include "../../Parameters.h"
#include "../EnergySave/EnergySave.h"
#include "../LoRaWan/LoRaWan.h"

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif


class NetworkClass
{
	protected:
	Stream *debug;
	LoRaWanClass *lora;
	
	char Appkey[33] = API_KEY;
	char AppEUI[17] = APP_EUI;
	
	bool joined = false;
	
	uint16_t unconfirmed_sent = 0;
	uint16_t confirmed_interval;
	uint16_t failed_before_rejon;
	uint16_t ack_failed = 0;
	
	bool send_gps = true;
	
	public:
	void init(Stream *serial, LoRaWanClass *loraWan);
	bool connected();
	bool setup(unsigned char timeout_s);
	bool join(unsigned char timeout_s);
	bool ping();
	bool transmitMeasurement(sensors_measurements measurement);
	void printPacket(byte *payload, int size);
};

extern NetworkClass Network;

#endif

