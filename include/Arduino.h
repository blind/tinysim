#ifndef __ARDUINO_H__
#define __ARDUINO_H__

#include <cstdint>
#include <stddef.h>
#include <cstdlib>

#include <inttypes.h>


#define _BV(x) (1u<<(x))


typedef uint8_t byte;

class Print
{
public:
};


class  SimSerial
{
public:
	void begin(uint32_t speed);
	void println(uint32_t speed);
};

extern SimSerial Serial;

//extern void putString(int row, int col, int color, const char* msg, const FONT_INFO fontInfo);

extern uint16_t micros();
extern uint16_t millis();
extern void delay(uint32_t millis);

#endif
