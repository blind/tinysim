

#include <stdint.h>
#include <stdio.h>


#include "SPI.h"
#include "Wire.h"
#include "Arduino.h"

SimSPI SPI;
SimWire Wire;

SimSPI::SimSPI()
: SPI_dataReg( new DataRegister(this) )
{
}

void SimSPI::begin()
{

}

void SimSPI::transfer( uint8_t data )
{
//	printf( "SPI: sending %02x \n", data );
}


void SimSPI::setDataMode( uint8_t mode )
{
	printf( "SPI: data mode $%02x \n", mode );
}

void SimSPI::setClockDivider( uint32_t divider )
{
	printf( "SPI: clock divider $%08x \n", divider );
}


void SimSPI::writeReg( DataRegister *reg, uint8_t data)
{
	if( reg == SPI_dataReg )
	{
		//printf("Write data: %02x\n", data);
	}
}



uint8_t TWBR;

SimWire::SimWire()
{

}

void SimWire::begin()
{

}

void SimWire::beginTransmission(uint32_t lala)
{
	printf("[SimWire::beginTransmission] $%02x\n", lala);
}

void SimWire::endTransmission()
{
	printf("[SimWire::endTransmission]\n");
}

void SimWire::requestFrom(uint32_t address, uint8_t mode)
{
	printf("[SimWire::requestFrom] $%02x $%02x\n", address, mode);
}

uint8_t SimWire::read()
{
	return 0u;
}


void SimWire::write(uint8_t)
{
}



void SimSerial::begin(uint32_t)
{

}


void SimSerial::println(uint32_t intValue)
{
	printf( "[Serial::println] %d\n", intValue );
}

SimSerial Serial;




uint8_t pgm_read_byte( const uint8_t* address ) { return *address; }
uint16_t pgm_read_word( const uint16_t* address ) { return *address; }


void delay( uint32_t /* len */ ) { }

uint16_t micros() { return 1u; }
uint16_t millis() { return 1u; }


