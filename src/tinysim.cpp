#include <SDL.h>

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "tinysim.h"
#include "simtinyscreen.h"
#include "Wire.h"
#include "Arduino.h"

static SDL_Window *window;
static SDL_Surface *surface;

static void TinySimQuit();
static int TinySimInit();
static void TinySimBlit8( const uint8_t* );
static void TinySimBlit16( const uint16_t* );

static int scale = 3;


extern void loop();
extern void setup();


SimSerial Serial;
SimSPI SPI;
SimWire Wire;

static SimTinyScreen simdisplay;

int main(void)
{
	printf("Starting TinySim\n");
	SPI.AddDevice( &simdisplay );
	Wire.AddDevice( &simdisplay, 0x20 );

	int result = TinySimRun( );

	if( result != 0 )
	{
		printf("Could not start Tiny Sim\n");
		return 0;
	}
	return 0;
}


int TinySimRun( )
{
	// Call gameFunction 60 times/s and stuff.

	if( 0 != TinySimInit() )
	{
		return 1;
	}

	setup();

	int done = 0;
	while( !done )
	{
		uint32_t start = SDL_GetTicks();
		loop();

		uint16_t* dispBuff = simdisplay.GetScreenBuffer();
		TinySimBlit16( dispBuff );

		SDL_Surface *dstSurface = SDL_GetWindowSurface(window);
		SDL_BlitScaled( surface, NULL, dstSurface, NULL );
		SDL_UpdateWindowSurface(window);

		SDL_Event event;
		while( SDL_PollEvent(&event) )
		{
			if( event.type == SDL_QUIT )
			{
				printf("Leaving TinySim, see you later!\n");
				done = 1;
			}
		}
		uint32_t timePassed = SDL_GetTicks() - start;
		if( timePassed < 16 )
			SDL_Delay( 16-timePassed );
	}

	TinySimQuit();
	return 0;
}


void TinySimDrawFrameBuffer( const void* buffer, int mode )
{
	// Draw a full 96*64 pixels to the SDL window.
	// mode 0 > 16 bitperpixel
	// mode 1 > 8 bit per pixel 3,3,2

	if( mode == 0 )
	{
		TinySimBlit16( (const uint16_t*)buffer );
	}
	else
	{
		TinySimBlit8( (const uint8_t*)buffer );
	}
}


static void TinySimBlit16( const uint16_t* buffer )
{
	uint32_t *frameBuffer = (uint32_t*)surface->pixels;

	for(int row = 0 ; row < surface->h; ++row )
	{
		uint32_t *rowData = frameBuffer;
		for( int x = 0 ; x < surface->w; ++x )
		{
			uint16_t pixelData = *buffer++;
			// We need to convert the pixel data here.
			// Since I'm not sure about the pixel format
			// on the Tiny Arcade, I will pretend that I
			// know

			uint8_t r = (pixelData>>8) & 0xf8;
			uint8_t g = (pixelData>>3) & 0xfc;
			uint8_t b = (pixelData<<3) & 0xf8;

			*rowData++ = r<<16 | g << 8 | b;
		}
		frameBuffer = (uint32_t*)((uint8_t*)frameBuffer + surface->pitch);
	}
}

static void TinySimBlit8( const uint8_t* buffer )
{
	uint32_t *frameBuffer = (uint32_t*)surface->pixels;

	for(int row = 0 ; row < surface->h; ++row )
	{
		uint32_t *rowData = frameBuffer;
		for( int x = 0 ; x < surface->w; ++x )
		{
			uint16_t pixelData = *buffer++;
			// We need to convert the pixel data here.
			*rowData++ = pixelData;
		}
		frameBuffer = (uint32_t*)((uint8_t*)frameBuffer + surface->pitch);
	}
}


static int TinySimInit()
{
	int result = result = SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO );
	if( 0 != result )
	{
		fprintf(stderr, "Could not initialize SDL");
		return 1;
	}

	window = SDL_CreateWindow( "TinySim"
		, SDL_WINDOWPOS_UNDEFINED
		, SDL_WINDOWPOS_UNDEFINED
		, 96*scale
		, 64*scale
		, SDL_WINDOW_SHOWN );

	if( window == NULL )
	{
		fprintf(stderr, "Could not create SDL window");
		TinySimQuit();
		return 2;
	}

	SDL_SetWindowTitle( window, "Tiny Sim" );

	surface = SDL_CreateRGBSurface( 0, 96, 64, 32, 0, 0, 0, 0 );

	return result;
}


static void TinySimQuit()
{
	if( window != NULL )
	{
		SDL_DestroyWindow( window );
		SDL_FreeSurface( surface );
	}

	SDL_Quit();
}
