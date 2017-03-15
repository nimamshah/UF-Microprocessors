/* Lab6_PartA.c
 * 
 * Lab 6 Part A
 * Name: Nicholas Imamshah
 * Section: 6957
 * TA Name: Daniel Gonzalez
 * Description: The purpose of this program is to generate the C6 note using the XMEGA's TC system.
 */
//////////////////////////////////////INCLUDES///////////////////////////////////////
#include <avr/io.h>
#include "ebi_init.h"
#include "ebi_driver.h"

//////////////////////////////////INITIALIZATIONS////////////////////////////////////
#define F_CPU 2000000

/////////////////////////////////////PROTOTYPES//////////////////////////////////////
void tc_init(void);
void play(uint16_t freq);

///////////////////////////////////MAIN FUNCTION/////////////////////////////////////
int main(void)
{
	ebi_init();
	tc_init();
	
	while (1)
	{
		play(1046.50);
		if (__far_mem_read(IO_PORT) & 0x01)	// If Switch0 True, play note
		{
			TCE0.CTRLB |= TC0_CCBEN_bm;
		}
		else								// Else, turn off
		{
			TCE0.CTRLB = TC_WGMODE_FRQ_gc;
		}
	}
}

/////////////////////////////////////FUNCTIONS///////////////////////////////////////
void tc_init(void)
{
	PORTE.DIRSET = 0x02;			// Set Port E Pin 2 as output
	
	TCE0.CTRLA = TC_CLKSEL_DIV1_gc;	// Prescaler: CLK
	
	TCE0.CTRLB = TC0_CCBEN_bm |		// Enable CCB, FRQ mode
				 TC_WGMODE_FRQ_gc;
	TCE0.CTRLE = TC_BYTEM_NORMAL_gc;
	
}

void play(uint16_t freq)
{
	uint16_t cca = (F_CPU / (freq * 2)) - 1;	// Uses fFRQ formula from doc8331
	TCE0.CCA = cca;
}