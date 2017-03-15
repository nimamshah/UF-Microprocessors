/* final.c
 * 
 * Name: Nicholas Imamshah
 * Section: 6957
 * TA Name: Daniel Gonzalez
 * Description: The purpose of this program is to finish off Microprocessors at UF
 */
//////////////////////////////////////INCLUDES///////////////////////////////////////
#include <avr/io.h>
#include "dac_driver.h"

//////////////////////////////////INITIALIZATIONS////////////////////////////////////
#define F_CPU 2000000
#define BSel 12
#define BScale 3

/////////////////////////////////////PROTOTYPES//////////////////////////////////////
void adjust(char c);
void usart_init(void);
char IN_CHAR(void);

uint16_t vout = 0;

///////////////////////////////////MAIN FUNCTION/////////////////////////////////////
int main(void)
{
	usart_init();
	dac_init();
	while (!dac_data_empty());
	dac_write(0);
	
	char c;
	while (1)
	{
		c = IN_CHAR();
		adjust(c);
	}
}

/////////////////////////////////////FUNCTIONS///////////////////////////////////////
void adjust(char c)
{
	asm volatile ("nop");
	while (!dac_data_empty());
	if (c == 'h' || c == 'H')
	{
		// higher
		if (vout < (4095-409))
		{
			vout += 409;
			dac_write(vout);
		}
	} else if (c == 'l' || c == 'L')
	{
		// lower
		if (vout > (0+408))
		{
			vout -= 409;
			dac_write(vout);
		}
	}
}

void usart_init(void)
{
	PORTD_DIRCLR = 0x04;
	PORTQ_DIRSET = 0xA;
	PORTQ_OUTCLR = 0xA;
	
	USARTD0.CTRLB = USART_RXEN_bm;
	USARTD0.CTRLC = USART_PMODE_ODD_gc | USART_CHSIZE_8BIT_gc;
	
	USARTD0.BAUDCTRLA = (BSel & 0xFF);
	USARTD0.BAUDCTRLB = ((BScale << 4) & 0xF0) | ((BSel >> 8) & 0x0F);
}

char IN_CHAR(void)
{
	while (!(USARTD0_STATUS & USART_RXCIF_bm));
	asm volatile ("nop");
	return USARTD0_DATA;
}