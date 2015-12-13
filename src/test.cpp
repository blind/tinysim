//#include "tinysim.h"
#include <cstdlib>
#include <memory.h>

#include "Arduino.h"
#include <Wire.h>
#include <SPI.h>


#define SCREEN_I2C_ADDR	0x20
#define GPIO_EXP_REG_DATA 0x0
#define GPIO_EXP_REG_DIR 0x01
#define GPIO_EXP_REG_PULLUP 0x02
#define GPIO_EXP_REG_PULLDOWN 0x03


#define PIN_DC		0x01
#define PIN_CS		0x02
#define PIN_SHDN	0x04
#define PIN_RES		0x08
#define PIN_BTN1	0x10
#define PIN_BTN2	0x20
#define PIN_BTN3	0x40
#define PIN_BTN4	0x80

#define STAR_COUNT 300

struct Star
{
	int16_t x; // Fixed point 8.8
	int16_t y; // 
	int16_t speed; // Fixed point 8.8
};

static struct Star stars[STAR_COUNT];


static uint16_t myFrameBuffer [96*64];


static void clear_screen()
{
	memset( myFrameBuffer, 0, sizeof(myFrameBuffer ) );
}

static void DrawStars()
{
	struct Star *star = stars;
	for( int i = 0; i < STAR_COUNT; ++i, star++ )
	{
		star->x -= star->speed;
		star->x &= 0x7fff;
		int16_t x = star->x >> 8;
		if( (x >= 0) && (x < 96) )
			myFrameBuffer[ x + star->y * 96] = 0xffff;
	}
}

static void SendFrameBufferToDisplay()
{
	Wire.beginTransmission(SCREEN_I2C_ADDR);
	Wire.write( 0 );  // First write selects register
	Wire.write( 0x1 );  // Bit 0 selects command/data write, bit 1 Chip select, active low
	Wire.endTransmission();


	uint8_t *buff = (uint8_t*) myFrameBuffer;
	for(int i = sizeof(myFrameBuffer); i > 0 ; --i)
	{
		SPDR = *buff++;
		while(!(SPSR & _BV(SPIF)));
	}

	Wire.beginTransmission(SCREEN_I2C_ADDR);
	Wire.write( 0 );  // First write selects register
	Wire.write( 0xff );  // Bit 0 selects command/data write, bit 1 Chip select, active low
	Wire.endTransmission();
}



static void fill_with_colors()
{
	uint16_t color;
	for( int row = 0; row < 64; ++row )
	{
		for( int col = 0; col < 96; ++col )
		{
			uint16_t bscol = color;
			bscol = ((bscol&0xff) << 8) | (bscol >> 8); 
			myFrameBuffer[row*96+col] = bscol;

			color += 1;
		}
	}
}

void loop()
{
	clear_screen();
	fill_with_colors();
	DrawStars();
	SendFrameBufferToDisplay();
}

static void setup_gpio_expander()
{
	Wire.beginTransmission(SCREEN_I2C_ADDR);

	// Setup pullup register
	Wire.write( GPIO_EXP_REG_PULLUP );  // First write selects register
	Wire.write( (PIN_BTN1|PIN_BTN2|PIN_BTN3|PIN_BTN4) );  // Top four bits are buttons,


	Wire.write( GPIO_EXP_REG_DATA );  // First write selects register
	Wire.write( ~0x03 );  // Bit 0 selects command/data write, bit 1 Chip select, active low


	Wire.write( GPIO_EXP_REG_DATA );  // First write selects register
	Wire.write( ~0x03 );  // Bit 0 selects command/data write, bit 1 Chip select, active low
	Wire.endTransmission();

}

void setup()
{
	for( int i = 0; i < STAR_COUNT; ++i )
	{
		stars[i].x = rand() & 0x7fff;
		stars[i].y = rand() & 0x3f;
		stars[i].speed = rand() & 0x3ff;
	}

	Wire.beginTransmission(SCREEN_I2C_ADDR);
	Wire.write( GPIO_EXP_REG_DATA );  // First write selects register
	Wire.write( ~0x03 );  // Bit 0 selects command/data write, bit 1 Chip select, active low
	Wire.endTransmission();


	SPI.transfer( 0xA0 ); // remap
	SPI.transfer( (1<<6) );

	// column start
	SPI.transfer( 0x15 );
	SPI.transfer( 0 );
	SPI.transfer( 95 );

	// row start
	SPI.transfer( 0x75 );
	SPI.transfer( 0 );
	SPI.transfer( 63 );

	Wire.beginTransmission(SCREEN_I2C_ADDR);
	Wire.write( GPIO_EXP_REG_DATA );  // First write selects register
	Wire.write( ~0x03 );  // Bit 0 selects command/data write, bit 1 Chip select, active low
	Wire.endTransmission();

}

