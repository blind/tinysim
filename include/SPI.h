#ifndef __SPI_H__
#define __SPI_H__

#include <cstdint>

class DataRegister;

class SimSPI
{
public:
	SimSPI();

	void begin();
	void transfer( uint8_t something );

	void setDataMode( uint8_t mode );
	void setClockDivider( uint32_t divider );


	void writeReg( DataRegister *reg, uint8_t data);

	// Members
	DataRegister* SPI_dataReg;

	uint8_t SPI_IF;

};

class DataRegister
{
public:
	DataRegister( SimSPI* owner) { this->spi = owner; }
	void operator=(uint8_t data)
	{
		spi->writeReg( this, data );
	}

private:
	SimSPI* spi;
};

extern SimSPI SPI;


#define SPI_MODE0 0
#define SPI_CLOCK_DIV2 0
#define SPDR *SPI.SPI_dataReg
#define SPSR 0xff
#define SPIF (SPI.SPI_IF)

#endif
