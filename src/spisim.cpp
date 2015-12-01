

#include <stdint.h>
#include <stdio.h>


#include "SPI.h"
#include "Wire.h"
#include "Arduino.h"

SimSerial Serial;
SimSPI SPI;
SimWire Wire;


// This should be moved into SimWire class and accessed through macro.
uint8_t TWBR;


SimSPI::SimSPI()
: SPI_dataReg( new DataRegister(this) )
{
	devicesArraySize = 4;
	devices = new ISPIDevice*[devicesArraySize];
}


void SimSPI::AddDevice( ISPIDevice *device )
{
	int i = 0;
	while( (i < devicesArraySize) && (devices[i] == NULL) )
		i++;

	if( i < devicesArraySize )
	{
		devices[i] = device;
		currentSlave = device;
	}
}

void SimSPI::begin()
{

}

void SimSPI::transfer( uint8_t data )
{
	if( currentSlave )
	{
		currentSlave->spiSlaveWrite( data );
	}
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
		currentSlave->spiSlaveWrite( data );
	}
}




/********************************
 *
 */

SimWire::SimWire()
{

}


void SimWire::AddDevice(II2CSlaveDevice* device, uint32_t address)
{
	this->device = device;
	this->deviceAddress = address;
}


void SimWire::begin()
{

}

void SimWire::beginTransmission(uint32_t address)
{
	printf("[SimWire::beginTransmission] $%02x\n", address);
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


void SimWire::write(uint8_t data)
{
//	printf("[SimWire::write] $%02x\n", data);
	// TODO: Send to the device with address from last beginTransmission
	this->device->writeData( data );
}



/********************************
 *
 */
void SimSerial::begin(uint32_t)
{

}


void SimSerial::println(uint32_t intValue)
{
	printf( "[Serial::println] %d\n", intValue );
}



uint8_t pgm_read_byte( const uint8_t* address ) { return *address; }
uint16_t pgm_read_word( const uint16_t* address ) { return *address; }


void delay( uint32_t /* len */ ) { }

uint16_t micros() { return 1u; }
uint16_t millis() { return 1u; }


