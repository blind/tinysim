#ifndef __WIRE_H__
#define __WIRE_H__

#include <stdint.h>


class II2CSlaveDevice
{
public:
	virtual void i2cWriteData( uint8_t ) = 0;
	virtual uint8_t i2cReadData( ) = 0;
};


class SimWire
{
public:
	SimWire(bool verbose = false);

	void AddDevice(II2CSlaveDevice*, uint32_t address);

	void begin();
	void beginTransmission(uint32_t lala);
	void endTransmission();
	void requestFrom(uint32_t address, uint8_t mode);
	uint8_t read();
	void write(uint8_t);


private:
	// For now we have only one device, so that is what I'm testing.
	II2CSlaveDevice* device;
	uint32_t deviceAddress;

	bool verbose_;

};

extern uint8_t TWBR;

extern SimWire Wire;


#endif
