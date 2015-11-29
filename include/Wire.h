#ifndef __WIRE_H__
#define __WIRE_H__

#include <stdint.h>

class Wire_t
{
public:
	Wire_t();
	void begin();
	void beginTransmission(uint32_t lala);
	void endTransmission();
	void requestFrom(uint32_t address, uint8_t mode);
	uint8_t read();
	void write(uint8_t);
};

extern uint8_t TWBR;

extern Wire_t Wire;


#endif
