

#include <chrono>
#include <cstdint>
#include <cstdio>


#include "SPI.h"
#include "Wire.h"
#include "Arduino.h"





// This should be moved into SimWire class and accessed through macro.
uint8_t TWBR = 0u;


SimSPI::SimSPI()
: SPI_dataReg( new DataRegister(this) )
{
	devicesArraySize = 4;
	devices = new ISPIDevice*[devicesArraySize];
	for (int i = 0; i < devicesArraySize; i++) {
		devices[i] = NULL;
	}
}


void SimSPI::AddDevice( ISPIDevice *device )
{
	int i = 0;
	while( (i < devicesArraySize) && (devices[i] != NULL) )
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
	if( (reg == SPI_dataReg) && (currentSlave != NULL) )
	{
		currentSlave->spiSlaveWrite( data );
	}
}






/********************************
 *
 */
void SimSerial::begin(uint32_t)
{

}


void SimSerial::println(uint32_t intValue)
{
//	printf( "[Serial::println] %d\n", intValue );
}



uint8_t pgm_read_byte( const uint8_t* address ) { return *address; }
uint16_t pgm_read_word( const uint16_t* address ) { return *address; }


void delay( uint32_t /* len */ )
{
	// This needs to execute SDL loop for len ms.
}

uint16_t micros()
{
	auto now = std::chrono::high_resolution_clock::now().time_since_epoch();
	return (uint16_t)std::chrono::duration_cast<std::chrono::microseconds>(now).count();
}
uint16_t millis()
{
	auto now = std::chrono::high_resolution_clock::now().time_since_epoch();
	return (uint16_t)std::chrono::duration_cast<std::chrono::milliseconds>(now).count();
}
