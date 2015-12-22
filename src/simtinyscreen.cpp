#include  "simtinyscreen.h"

#include <cstdio>


SimTinyScreen::SimTinyScreen()
{
	bufferIndex = 0;
	regs[0] = 0xffu;
	regs[1] = 0xffu;
	regs[5] = 0xffu;
	bufferIndex = 0;
	columnStart = 0;
	columnEnd = 95;

	rowStart = 0;
	rowEnd = 63;

	columnPtr = 0;
	columnEnd = 0;
}

void SimTinyScreen::i2cWriteData( uint8_t data )
{
	if( busState == 0 ) {
		regIdx = data & 31;
	} else {
		regs[regIdx] = data;
	}
	busState ^= 1;
}


uint8_t SimTinyScreen::i2cReadData( uint8_t reg )
{
	// This is just a guess on how it works.
	busState = 0;
	return regs[reg]; // Upper bits are buttons, 
}

uint8_t SimTinyScreen::spiSlaveWrite( uint8_t data )
{
	if( regs[0] & 1 ) {
		WriteDataByte( data );
	} else {
		WriteCommandByte( data );
	}
	// Since MISO is not connected, return ones.
	return 0xffu;
}

static uint16_t GetColorByCombiningBuffer( const uint8_t* buffer, uint8_t mode, bool reversed )
{
	uint16_t color = 0u;
	switch( mode ) {
	case 0:
		if( !reversed ) {
			// Normal RGB 
			uint8_t data = *buffer;
			color = (data&0xe0)<<8u;
			color |= (data&0x1c) << 6u;
			color |= (data&0x3) << 3u;
		} else {
			// Revered, BGR
			uint8_t data = *buffer;
			color = (data&0x03)<<14u; // R
			color |= (data&0x1c) << 6u;		// G
			color |= (data&0xe0) >> 2u;		// B
		}
		break;

	case 1:
		{
			uint8_t b0 = *buffer;
			uint8_t b1 = *(buffer+1);

			if( !reversed ) {
				// Normal rgb
				color = b0<<8 | b1;
			} else {
				// Reversed RGB
				color = ((b1&0x1f)<<11u) | ((b0>>3u)&0x1f) | (b1&0xe0) | (b0&0x07)<<8u;
			}
		}
		break;

	case 2: 
		{
			uint8_t b0 = *buffer;
			uint8_t b1 = *(buffer+1);
			uint8_t b2 = *(buffer+2);

			if( !reversed ) {
				color = ((b0&0x3f) << 10) | ((b1&0x3f)<<5) | ((b2&0x3e)>>1);
			} else {
				color = ((b2&0x3f) << 10) | ((b1&0x3f)<<5) | ((b0&0x3e)>>1);
			}
		}
		break;

	default:
		// Log invalid state.
		break;
	}

	return color;
}


void SimTinyScreen::WriteDataByte( uint8_t data )
{
	uint8_t colorMode = (colorModeRemapReg >> 6) & 3u;
	bool reverseColors = 0 != (colorModeRemapReg & (1u<<2));
	bool writeColor = false;
	uint16_t finalColor; 

	if( colorMode > 2 )
	{
		fprintf( stderr, "Invalid color mode." );
		return;
	}

	tmpColorBuffer[colorWriteCounter++] = data;
	// By coincidence, the colormode is the same as bytes required per color -1.
	if( colorWriteCounter > colorMode )
	{
		finalColor = GetColorByCombiningBuffer(tmpColorBuffer, colorMode, reverseColors);
		writeColor = true;
	}

	if( writeColor ) {
		uint16_t index;
		// Column addressing bit, if set, reverse lines.
		if( colorModeRemapReg & (1u<<1) ) {
			index = rowPtr * 96 + (95-columnPtr);
		} else {
			index = rowPtr * 96 + columnPtr;
		}
		screenBuffer[index] = finalColor;
		colorWriteCounter = 0;


		// Address increment mode, if set increment row first.
		if( colorModeRemapReg & (1u) ) {
			if( ++rowPtr > rowEnd ) {
				rowPtr = rowStart;
				if( ++columnPtr > columnEnd ) {
					columnPtr = columnStart;
				}
			}

		} else {
			if( ++columnPtr > columnEnd ) {
				columnPtr = columnStart;
				if( ++rowPtr > rowEnd ) {
					rowPtr = rowStart;
				}
			}
		}
	}
}


void SimTinyScreen::WriteCommandByte( uint8_t data )
{
	commandBuffer[bufferIndex++] = data;

	if( expectedByteCount_ == 0 )
	{
		switch( data )
		{
		case 0x15: // Set column start and end address
			expectedByteCount_ = 2; // two more bytes.
			break;


		case 0x75: // Set row start and end address
			expectedByteCount_ = 2; // two more bytes.
			break;


		case 0x81: // Set contrast for "A" segment (blue?)
		case 0x82: // Set contrast for "B" segment (green?)
		case 0x83: // Set contrast for "C" segment (red?)
		case 0x87: // Set master current attenuation factor
			expectedByteCount_ = 1;
			break;


		// Set second pre charge speed
		case 0x8A: // segment A
		case 0x8B: // segment B
		case 0x8C: // segment C
			expectedByteCount_ = 1;
			break;


		case 0xA0:	// Set driver remap and color depth
		case 0xA1:	// Set display start list register by row
		case 0xA2:	// Set vertical offset by Column
			expectedByteCount_ = 1;
			break;


		// Display modes
		case 0xA4:	// Normal display
		case 0xA5:	// Entire display ON, all pixels turn on at GS63
		case 0xA6:	// Entire display OFF, all pixels turn OFF
		case 0xA7:	// Inverse display
			expectedByteCount_ = 0;
			break;

		// Set mux ratio to N+1
		case 0xA8:
			expectedByteCount_ = 1;
			break;


		// Configure dim mode setting
		case 0xAB:
			expectedByteCount_ = 5;
			break;

		case 0xAD: // Selecte extern Vcc supply
			expectedByteCount_ = 1;
			break;

		case 0xAC: // Display on in dim mode
		case 0xAE: // Display OFF (sleep mode)
		case 0xAF: // Display ON in normal mode.
			expectedByteCount_ = 0;
			break;

		case 0xB0: // Enable power save mode.
			expectedByteCount_ = 1;
			break;

		case 0xB1: // Phase 1 period in N DCLK.
			expectedByteCount_ = 1;
			break;

		case 0xB3: // Define the divide ratio of display clocks
			expectedByteCount_ = 1;
			break;


		case 0xB8: // Pluse width of DCLK
			expectedByteCount_ = 32;
			break;

		case 0xB9: // Reset built in gray scale table
			expectedByteCount_ = 0;
			// TODO: handle command
			break;

		case 0xBB: // Set pre-charge voltage level
			expectedByteCount_ = 1;
			break;

		case 0xBE: // Set Com deselect voltage level
			expectedByteCount_ = 1;
			break;

		case 0xFD: // MCU protection status
			expectedByteCount_ = 1;
			break;


		// Accellerated graphics commands
		case 0x21: // Draw Line
			expectedByteCount_ = 7;
			break;

		case 0x22: // Drawing rectangle
			expectedByteCount_ = 10;
			break;

		case 0x23: // Copy
			expectedByteCount_ = 6;
			break;

		case 0x24: // Dim window
			expectedByteCount_ = 4;
			break;

		case 0x25: // Clear window
			expectedByteCount_ = 4;
			break;

		case 0x26: // Fill enable/disable
			expectedByteCount_ = 1;
			break;

		case 0x27: // Set number of column as horizontal scroll offset.
			expectedByteCount_ = 5;
			break;

		case 0x2E: // Deactivate scrolling
			expectedByteCount_ = 0;
			// TODO: Handle command
			break;

		case 0x2F: // Activate scrolling
			expectedByteCount_ = 0;
			// TODO: Handle command
			break;


		default:
			// Unknown command, what to do?
			break;
		}	
	}


	// TODO: Save bytes to buffer. execute command when expected byte count is 0.
	if( expectedByteCount_ == 0 )
	{
		ExecuteCommandInBuffer( );
		bufferIndex = 0;
	} else {
		--expectedByteCount_;
	}
}


void SimTinyScreen::ExecuteCommandInBuffer()
{
	auto buffPtr = commandBuffer;
	auto command = *buffPtr++; 

	switch( command )
	{
	case 0x15:
		{
			// Set column address 
			columnStart = *buffPtr++;
			columnEnd = *buffPtr++;
			columnPtr = columnStart;
			printf("set column start-end %d - %d\n", columnStart, columnEnd);
			colorWriteCounter = 0u;
		}
		break;

	case 0x75:
		{
			// Set row address 
			rowStart = *buffPtr++;
			rowEnd = *buffPtr++;
			rowPtr = rowStart;
			printf("set row start-end %d - %d\n", rowStart, rowEnd);
			colorWriteCounter = 0u;
		}
		break;

	case 0xa0:
		{
			// Remap and color depth settings.
			colorModeRemapReg = *buffPtr;
			colorWriteCounter = 0u;
		}
		break;


	default:
		printf("SSD1331 command not implemented: $%02x", command);
		{
			int i = 1; 
			while( i < bufferIndex ) {
				printf(", $%02x", commandBuffer[i++] );
			}
			printf("\n" );
			
		}
		break;
	}
}