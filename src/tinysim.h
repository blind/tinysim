#ifndef _TINYSIM_H
#define _TINYSIM_H

#ifdef __cplusplus
extern "C" {
#endif


typedef void (*GameUpdateFunc)();

// Call gameUpdate function 60 times/sec
// return 0 if the game was run and exited ok.
// return 1 if the initialization failed.
extern int TinySimRun( GameUpdateFunc gameUpdate );

// Read input.
// bit 0 - button b
// bit 1 - button a
extern void TinySimGetInput( int *left, int *right, int *buttons);

// Draw a full 96*64 pixels to the SDL window.
// mode 0 > 16 bitperpixel
// mode 1 > 8 bit per pixel 3,3,2
extern void TinySimDrawFrameBuffer( const void* buffer, int mode );

#ifdef __cplusplus
}
#endif
#endif // _TINYSIM_H
