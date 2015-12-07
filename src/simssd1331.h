#ifndef __SIMSSD1331_H__
#define __SIMSSD1331_H__

#include <stdint.h>

#include "SPI.h"
#include "Wire.h"

class SimSSD1331 : public ISPIDevice, public II2CSlaveDevice
{
public:
	SimSSD1331();


	uint16_t* GetScreenBuffer() { return screenBuffer; };



	// Write data to SPI slave, return data written by slave.
	virtual uint8_t spiSlaveWrite( uint8_t );


	// I2C Slave 
	virtual void i2cWriteData( uint8_t );
	virtual uint8_t i2cReadData( ) ;


private:
	// I2C GPIO expander stuff.

	void WriteDataByte( uint8_t );
	void WriteCommandByte( uint8_t );

	int8_t busState;
	int8_t regIdx;
	int8_t regs[32];

	// SSD1331 controller

	void ExecuteCommandInBuffer();

	// Control register

	uint8_t expectedByteCount;

	uint8_t currentCommand; // 

	uint8_t colorWriteCounter;
	uint16_t finalColor;


	uint8_t columnPtr;
	uint8_t rowPtr;


	// Internal registers...

	uint8_t colorModeRemapReg;

	uint8_t columnStart;
	uint8_t columnEnd;

	uint8_t rowStart;
	uint8_t rowEnd;

	uint16_t screenBuffer[96*64];

	uint16_t bufferIndex;
	uint8_t commandBuffer[512];
	uint8_t tmpColorBuffer[3];
};

#endif

