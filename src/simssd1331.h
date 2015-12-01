#ifndef __SIMSSD1331_H__
#define __SIMSSD1331_H__

#include <stdint.h>

#include "SPI.h"
#include "Wire.h"

class SimSSD1331 : public ISPIDevice, public II2CSlaveDevice
{
public:
	SimSSD1331();


	// Write data to SPI slave, return data written by slave.
	virtual uint8_t spiSlaveWrite( uint8_t );


	// I2C Slave 
	virtual void writeData( uint8_t );
	virtual uint8_t readData( uint8_t ) ;


private:
	// Control register

	uint8_t expectedByteCount;

	uint8_t currentCommand; // 


	uint8_t columnPtr;
	uint8_t rowPtr;


	// Internal registers...

	uint8_t columnStart;
	uint8_t columnEnd;

	uint8_t rowStart;
	uint8_t rowEnd;

	uint16_t screenBuffer[96*64];

	uint16_t bufferIndex;
	uint8_t commandBuffer[512];

};

#endif

