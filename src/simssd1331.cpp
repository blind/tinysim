#include  "simssd1331.h"

#include <cstdio>


SimSSD1331::SimSSD1331()
{
	bufferIndex = 0;
}


uint8_t SimSSD1331::spiSlaveWrite( uint8_t data )
{

	if( expectedByteCount == 0 )
	{
		printf("Incoming command: \033[34m%02x\033[0m\n", data);

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
	else
	{
		// TODO: Save bytes to buffer. execute command when expected byte count is 0.
		expectedByteCount--;
	}

	return 0;
}
