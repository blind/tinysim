#include "Arduino.h"
#include <Wire.h>
#include <SPI.h>

#include <TinyScreen.h>

TinyScreen display = TinyScreen(TinyScreenPlus);

#define STAR_COUNT 300

struct Star
{
	int16_t x; // Fixed point 8.8
	int16_t y; //
	int16_t speed; // Fixed point 8.8
};

static struct Star stars[STAR_COUNT];


static uint16_t myFrameBuffer [96*64];


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
	display.startData();
	display.writeBuffer((uint8_t*)myFrameBuffer, sizeof(myFrameBuffer));
	display.endTransfer();
}



static void fill_with_colors()
{
	uint16_t color = 0;
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
	fill_with_colors();
	DrawStars();
	SendFrameBufferToDisplay();
}

void setup()
{
	for( int i = 0; i < STAR_COUNT; ++i )
	{
		stars[i].x = rand() & 0x7fff;
		stars[i].y = rand() & 0x3f;
		stars[i].speed = rand() & 0x3ff;
	}

	display.begin();
	display.setBrightness(10); // 0-15

	display.setBitDepth(TSBitDepth16);

	// column start
	display.setX(0, 95);

	// row start
	display.setY(0, 63);
}
