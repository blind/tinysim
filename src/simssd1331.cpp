#include  "simssd1331.h"

#include <cstdio>



SimSSD1331::SimSSD1331()
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

void SimSSD1331::i2cWriteData( uint8_t data )
{
	if( busState == 0 )
	{
		regIdx = data & 31;
	}
	else
	{
		regs[regIdx] = data;
	}
	busState ^= 1;
}


uint8_t SimSSD1331::i2cReadData( )
{
	busState ^= 1;
	return regs[regIdx]; // Upper bits are buttons, 
}



uint8_t SimSSD1331::spiSlaveWrite( uint8_t data )
{
//	printf("Incoming data: $%02x\n", data);


	if( regs[0] & 1 )
	{
		// Write data
		WriteDataByte( data );

	}
	else
	{
		// Write command
		WriteCommandByte( data );
	}

	// Since MISO is not connected, return ones.
	return 0xffu;
}

void SimSSD1331::WriteDataByte( uint8_t data )
{
	uint8_t colorMode = (colorModeRemapReg >> 6) & 3u;

	if( colorMode == 0 )
	{
		// 256 color mode
		if( 0 == (colorModeRemapReg & (1u<<2)) )
		{
			// Normal RGB 
			uint16_t color = (data&0xe0)<<8u;
			color |= (data&0x1c) << 6u;
			color |= (data&0x3) << 3u;

			uint16_t index = rowPtr * 96 + columnPtr;
			screenBuffer[index] = color;
		}
		else
		{
			// Revered, BGR
			uint16_t color = (data&0x03)<<14u; // R
			color |= (data&0x1c) << 6u;		// G
			color |= (data&0xe0) >> 2u;		// B

			uint16_t index = rowPtr * 96 + columnPtr;
			screenBuffer[index] = color;

		}

		// red
	}




	// This is the default, adding in other order depending
	// on state will come later, when needed.
	if( ++columnPtr > columnEnd )
	{
		columnPtr = columnStart;
		if( ++rowPtr > rowEnd ) rowPtr = rowStart;
	}
}


void SimSSD1331::WriteCommandByte( uint8_t data )
{
	commandBuffer[bufferIndex++] = data;

	if( expectedByteCount == 0 )
	{
		switch( data )
		{
		case 0x15: // Set column start and end address
			expectedByteCount = 2; // two more bytes.
			break;


		case 0x75: // Set row start and end address
			expectedByteCount = 2; // two more bytes.
			break;


		case 0x81: // Set contrast for "A" segment (blue?)
			expectedByteCount = 1;
			break;

		case 0x82: // Set contrast for "B" segment (green?)
			expectedByteCount = 1;
			break;

		case 0x83: // Set contrast for "C" segment (red?)
			expectedByteCount = 1;
			break;

		case 0x87: // Set master current attenuation factor
			expectedByteCount = 1;
			break;


		// Set second pre charge speed
		case 0x8A: // segment A
			expectedByteCount = 1;
			break;

		case 0x8B: // segment B
			expectedByteCount = 1;
			break;

		case 0x8C: // segment C
			expectedByteCount = 1;
			break;


		// Set driver remap and color depth
		case 0xA0:
			expectedByteCount = 1;
			break;

		case 0xA1:	// Set display start list register by row
			expectedByteCount = 1;
			break;


		case 0xA2:	// Set vertical offset by Column
			expectedByteCount = 1;
			break;


		// Display modes
		case 0xA4:
		case 0xA5:
		case 0xA6:
		case 0xA7:
			expectedByteCount = 0;
			// TODO: Handle command
			break;

		// Set mux ratio to N+1
		case 0xA8:
			expectedByteCount = 1;
			break;


		// Configure dim mode setting
		case 0xAB:
			expectedByteCount = 5;
			break;

		case 0xAD: // Selecte extern Vcc supply
			expectedByteCount = 1;
			break;

		case 0xAC: // Display on in dim mode
		case 0xAE: // Display OFF (sleep mode)
		case 0xAF: // Display ON in normal mode.
			expectedByteCount = 0;
			// TODO: Handle command
			break;

		case 0xB0: // Enable power save mode.
			expectedByteCount = 1;
			break;

		case 0xB1: // Phase 1 period in N DCLK.
			expectedByteCount = 1;
			break;

		case 0xB3: // Define the divide ratio of display clocks
			expectedByteCount = 1;
			break;


		case 0xB8: // Pluse width of DCLK
			expectedByteCount = 32;
			break;

		case 0xB9: // Reset built in gray scale table
			expectedByteCount = 0;
			// TODO: handle command
			break;

		case 0xBB: // Set pre-charge voltage level
			expectedByteCount = 1;
			break;

		case 0xBE: // Set Com deselect voltage level
			expectedByteCount = 1;
			break;

		case 0xFD: // MCU protection status
			expectedByteCount = 1;
			break;


		// Accellerated graphics commands
		case 0x21: // Draw Line
			expectedByteCount = 7;
			break;

		case 0x22: // Drawing rectangle
			expectedByteCount = 10;
			break;

		case 0x23: // Copy
			expectedByteCount = 6;
			break;

		case 0x24: // Dim window
			expectedByteCount = 4;
			break;

		case 0x25: // Clear window
			expectedByteCount = 4;
			break;

		case 0x26: // Fill enable/disable
			expectedByteCount = 1;
			break;

		case 0x27: // Set number of column as horizontal scroll offset.
			expectedByteCount = 5;
			break;

		case 0x2E: // Deactivate scrolling
			expectedByteCount = 0;
			// TODO: Handle command
			break;

		case 0x2F: // Activate scrolling
			expectedByteCount = 0;
			// TODO: Handle command
			break;


		default:
			// Unknown command, what to do?
			break;
		}	
	}


	// TODO: Save bytes to buffer. execute command when expected byte count is 0.
	if( expectedByteCount == 0 )
	{
		ExecuteCommandInBuffer( );
		bufferIndex = 0;
	}
	else
	{
		--expectedByteCount;
	}
}


void SimSSD1331::ExecuteCommandInBuffer()
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
			// printf("set column start-end %d - %d\n", columnStart, columnEnd);
		}
		break;

	case 0x75:
		{
			// Set row address 
			rowStart = *buffPtr++;
			rowEnd = *buffPtr++;
			rowPtr = rowStart;
			// printf("set row start-end %d - %d\n", rowStart, rowEnd);
		}
		break;

	case 0xa0:
		{
			// Remap and color depth settings.
			colorModeRemapReg = *buffPtr++;
		}
		break;


	default:
		printf("SSD1331 command not implemented: $%02x", command);
		{
			int i = 1; 
			while( i < bufferIndex )
				printf(", $%02x", commandBuffer[i++] );
			printf("\n" );
			
		}
		break;
	}
}