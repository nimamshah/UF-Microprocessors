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
#define PF_OPC 0x18

uint8_t keys[] = {0x1, 0x4, 0x7, 0xE, 0x2, 0x5, 0x8, 0x0, 0x3, 0x6, 0x9, 0xF, 0xA, 0xB, 0xC, 0xD};

/////////////////////////////////////PROTOTYPES//////////////////////////////////////
void keypad_init(void);
uint8_t keyscan(void);
uint8_t scan(uint8_t);

/////////////////////////////////////FUNCTIONS///////////////////////////////////////
void keypad_init(void)
{
	PORTF.PIN7CTRL = PF_OPC;		// Set OPC to Pull-Up for all Keypad pins
	PORTF.PIN6CTRL = PF_OPC;
	PORTF.PIN5CTRL = PF_OPC;
	PORTF.PIN4CTRL = PF_OPC;
	PORTF.PIN3CTRL = PF_OPC;
	PORTF.PIN2CTRL = PF_OPC;
	PORTF.PIN1CTRL = PF_OPC;
	PORTF.PIN0CTRL = PF_OPC;
	
	PORTF.DIRSET = 0x0F;			// Set LSNibble of PortF as Output
}

uint8_t keyscan(void)
{
	uint8_t line = 0x0F;
	uint8_t input, index, key;
	for (uint8_t i = 0; i < 4; i++)	// Iterate columns
	{
		line &= ~(0x01 << i);		// Iterate shift 0x08 by i and not to hit each col
		PORTF.OUT = line;			// Output value for col
		asm("nop");
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
			while ((PORTF.IN & 0xF0) < 0xF0);
			break;
		} else 
		{
			key = 0xFF;			
		}
	}
	return key;
}