
#include <stdio.h>

#include "Wire.h"

/********************************
 *
 */

SimWire::SimWire( bool verbose )
: verbose_( verbose )
{

}


bool SimWire::AddDevice(II2CSlaveDevice* device, uint32_t address)
{
	bool result = false;

	if( devices_.find( address ) == devices_.end() )
	{
		devices_[address] = device;
		result = true;
	}
	else
	{
		printf("Device with address %02x already exists\n", address);
	}

	return result;
}


void SimWire::begin()
{

}

void SimWire::beginTransmission(uint32_t address)
{
	if( verbose_ )
		printf("[SimWire::beginTransmission] $%02x\n", address);

	current_slave_ = devices_[address];
}

void SimWire::endTransmission()
{
	if( verbose_ )
		printf("[SimWire::endTransmission]\n");
	current_slave_ = NULL;
}

void SimWire::requestFrom(uint32_t address, uint8_t reg)
{
	if( verbose_ )
		printf("[SimWire::requestFrom] $%02x $%02x\n", address, reg);

	// TODO: We must check if there is an actual slave for that address.
	current_slave_ = devices_[address];
	readReg_ = reg;
}

uint8_t SimWire::read()
{
	// TODO: Verify that bus is in read mode.
	if( current_slave_ != NULL ) {
		return current_slave_->i2cReadData( readReg_ );
	} else {
		printf("No slave selected, returning ones");
		return 0xffu;
	}
}


void SimWire::write(uint8_t data)
{
	// TODO: Verify that bus is in write mode.
	if( current_slave_ != NULL ) {
		current_slave_->i2cWriteData( data );
	} else {
		printf("No slave selected i2c writes to the void.");
	}
}

