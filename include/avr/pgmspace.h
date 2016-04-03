#ifndef __PGMSPACE_H__
#define __PGMSPACE_H__

#include <stdint.h>

#define PROGMEM
#define PSTR(str) (str)

extern uint8_t pgm_read_byte( const uint8_t* address );
extern uint16_t pgm_read_word( const uint16_t* address );

#endif
