

#include <stdint.h>
#include <stdio.h>


#include "SPI.h"
#include "Wire.h"
#include "Arduino.h"

SPI_t SPI;

Wire_t Wire;

SPI_t::SPI_t()
: SPI_dataReg( new DataRegister(this) )
{
}

void SPI_t::begin()
{

}



void SPI_t::transfer( uint8_t data )
{
	printf( "SPI: sending %02x \n", data );
}


void SPI_t::setDataMode( uint8_t mode )
{
	printf( "SPI: data mode %d \n", mode );
}

void SPI_t::setClockDivider( uint32_t divider )
{
	printf( "SPI: clock divider %d \n", divider );
}


void SPI_t::writeReg( DataRegister *reg, uint8_t data)
{
	if( reg == SPI_dataReg )
	{
		printf("Write data: %02x\n", data);
	}
}



uint8_t TWBR;

Wire_t::Wire_t()
{

}

void Wire_t::begin()
{

}

void Wire_t::beginTransmission(uint32_t lala)
{

}

void Wire_t::endTransmission()
{

}

void Wire_t::requestFrom(uint32_t address, uint8_t mode)
{

}

uint8_t Wire_t::read()
{
	return 0u;
}


void Wire_t::write(uint8_t)
{

}



void Serial_t::begin(uint32_t speed)
{

}


void Serial_t::println(uint32_t speed)
{
	printf( "%d", speed );
}

Serial_t Serial;




uint8_t pgm_read_byte( const uint8_t* address ) { return *address; }
uint16_t pgm_read_word( const uint16_t* address ) { return *address; }


void delay( uint32_t /* len */ )
{ }


uint16_t micros() { return 1u; }
uint16_t millis() { return 1u; }


