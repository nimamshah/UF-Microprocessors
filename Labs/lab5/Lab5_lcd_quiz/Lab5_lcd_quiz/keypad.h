/* keypad.c
 * 
 * Keypad in C
 * Name: Nicholas Imamshah
 * Section: 6957
 * TA Name: Daniel Gonzalez
 * Description: The purpose of this program is to interface the XMEGA processor 
 *		with an external Keypad.
 */
//////////////////////////////////////INCLUDES///////////////////////////////////////
#include <avr/io.h>

//////////////////////////////////INITIALIZATIONS////////////////////////////////////
#define F_CPU 2000000

uint8_t keys[] = {0x1, 0x4, 0x7, 0xE, 0x2, 0x5, 0x8, 0x0, 0x3, 0x6, 0x9, 0xF, 0xA, 0xB, 0xC, 0xD};

/////////////////////////////////////PROTOTYPES//////////////////////////////////////
void keypad_init(void);
uint8_t keyscan(void);
void keyhold(void);

/////////////////////////////////////FUNCTIONS///////////////////////////////////////
void keypad_init(void)
{
	PORTF.PIN7CTRL = PORT_OPC_PULLUP_gc;		// Set OPC to Pull-Up for all Keypad pins
	PORTF.PIN6CTRL = PORT_OPC_PULLUP_gc;
	PORTF.PIN5CTRL = PORT_OPC_PULLUP_gc;
	PORTF.PIN4CTRL = PORT_OPC_PULLUP_gc;
	PORTF.PIN3CTRL = PORT_OPC_PULLUP_gc;
	PORTF.PIN2CTRL = PORT_OPC_PULLUP_gc;
	PORTF.PIN1CTRL = PORT_OPC_PULLUP_gc;
	PORTF.PIN0CTRL = PORT_OPC_PULLUP_gc;
	
	PORTF.DIRSET = 0x0F;			// Set LSNibble of PortF as Output
}

uint8_t keyscan(void)
{
	uint8_t input, index, line, key = 0xFF, i = 0;
	for (i = 0; i < 4; i++)	// Iterate columns
	{
		line = ~(0x01 << i) & 0x0F;		// Iterate shift 0x08 by i and not to hit each col
		PORTF.OUT = line;			// Output value for col
		asm volatile ("nop");
		input = PORTF.IN & 0xF0;	// Read Input and bitmask off Output bits
		
		if (input < 0xF0)
		{
			switch (input)
			{
				case 0xE0:
					index = 0x00;
					break;
				case 0xD0:
					index = 0x01;
					break;
				case 0xB0:
					index = 0x02;
					break;
				case 0x70:
					index = 0x03;
					break;
			}
			key = keys[index+4*i];
			return key;
		}
	}
	return key;
}

void keyhold(void) 
{
	while ((PORTF.IN & 0xF0) < 0xF0);
}