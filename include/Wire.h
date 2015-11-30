#ifndef __WIRE_H__
#define __WIRE_H__

#include <stdint.h>

class SimWire
{
public:
	SimWire();
	void begin();
	void beginTransmission(uint32_t lala);
	void endTransmission();
	void requestFrom(uint32_t address, uint8_t mode);
	uint8_t read();
	void write(uint8_t);
};

extern uint8_t TWBR;

extern SimWire Wire;


#endif
