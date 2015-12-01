
#include <stdio.h>

#include "Wire.h"

/********************************
 *
 */

SimWire::SimWire( bool verbose )
: verbose_( verbose )
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
	if( verbose_ )
		printf("[SimWire::beginTransmission] $%02x\n", address);
}

void SimWire::endTransmission()
{
	if( verbose_ )
		printf("[SimWire::endTransmission]\n");
}

void SimWire::requestFrom(uint32_t address, uint8_t count)
{
	if( verbose_ )
		printf("[SimWire::requestFrom] $%02x $%02x\n", address, count);
}

uint8_t SimWire::read()
{
	return this->device->i2cReadData( );
}


void SimWire::write(uint8_t data)
{
//	printf("[SimWire::write] $%02x\n", data);
	// TODO: Send to the device with address from last beginTransmission
	this->device->i2cWriteData( data );
}

