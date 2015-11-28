#include <SDL2/SDL.h>
#include "tinysim.h"

#include <stdio.h>
#include <stdint.h>

static uint16_t myFrameBuffer [96*64];

#define STAR_COUNT 100

struct Star
{
	int16_t x; // Fixed point 8.8
	int16_t y; // 
	int16_t speed; // Fixed point 8.8
};

static struct Star stars[STAR_COUNT];


static void clearscreen()
{
	memset( myFrameBuffer, 0, sizeof(myFrameBuffer ) );
}

static void DrawStars()
{
	struct Star *star = stars;
	for( int i = 0; i < STAR_COUNT; ++i, star++ )
	{
		star->x -= star->speed;
		int16_t x = star->x >> 8;
		if( (x >= 0) && (x < 96) )
			myFrameBuffer[ x + star->y * 96] = 0xffff;
	}
}

static void MyGameUpdate()
{
	clearscreen();

	DrawStars();

	TinySimDrawFrameBuffer( myFrameBuffer, 0 );
}

static void InitMyGame()
{
	for( int i = 0; i < STAR_COUNT; ++i )
	{
		stars[i].x = rand() & 0xffff;
		stars[i].y = rand() & 0x3f;
		stars[i].speed = rand() & 0x3ff;
	}
}


int main(void)
{
	InitMyGame();

	int result = TinySimRun( MyGameUpdate );

	if( result != 0 )
	{
		printf("Could not start Tiny Sim\n");
		return 0;
	}
	return 0;
}

