// Codec.h

#ifndef _CODEC_h
#define _CODEC_h
#include "../../Parameters.h"

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

class CodecClass
{
	protected:
	sensors_measurements measurements;
	
	Stream *debug;
	
	public:
	enum encoding_type {
		MEASUREMENT,
		PING 
	};
	
	void init(Stream *serial);
	void setMeasurements(sensors_measurements sensors_m);
	void setPosition(gps_position gposition);
	uint8_t encodeAdcChannel(byte *payload, uint8_t start, int index);
	uint8_t encodeBatteryVoltage(byte *payload, uint8_t start);
	uint8_t encodeChannels(byte *payload, uint8_t start);
	uint8_t encodeBME(byte *payload, uint8_t start);
	uint8_t encodeCounter(byte *payload, uint8_t start);
	uint8_t encodeExternalSensor(byte *payload, uint8_t start);
	uint8_t encodeSupplyVoltage(byte *payload, uint8_t start);
	uint8_t encodePrefix(byte *payload, encoding_type encoding);
	uint8_t encode(byte *payload, encoding_type encoding, int size);
};

extern CodecClass Codec;

#endif

