/* Lab6_Quiz.c
 * 
 * Lab 6 Quiz
 * Name: Nicholas Imamshah
 * Section: 6957
 * TA Name: Daniel Gonzalez
 * Description: The purpose of this program is to generate PWMs to output a specific color to the uPad RGB LED.
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
	TCD0.CTRLB |= TC0_CCAEN_bm | TC0_CCBEN_bm; 
	TCD1.CTRLB |= TC1_CCAEN_bm;
	
	while (1)
	{
		
	}
}

/////////////////////////////////////FUNCTIONS///////////////////////////////////////
void tc_init(void)
{
	PORTD.DIRSET = 0x13;			// Set Port E Pin 2 as output
	PORTD.PIN0CTRL = PORT_INVEN_bm;
	PORTD.PIN1CTRL = PORT_INVEN_bm;
	PORTD.PIN4CTRL = PORT_INVEN_bm;
	
	TCD0.CTRLA = TC_CLKSEL_DIV1_gc;	// Prescaler: CLK
	TCD0.CTRLB = TC_WGMODE_SINGLESLOPE_gc;
	TCD0.CTRLE = TC_BYTEM_NORMAL_gc;
	
	TCD0.PER = 255;
	TCD0.CCA = 204;		// Red RGB Color
	TCD0.CCB = 0;		// Green RGB Color
	
	TCD1.CTRLA = TC_CLKSEL_DIV1_gc;
	TCD1.CTRLB = TC_WGMODE_SINGLESLOPE_gc;
	TCD1.CTRLE = TC_BYTEM_NORMAL_gc;
	
	TCD1.PER = 255;
	TCD1.CCA = 102;		// Blue RGB color
}

void play(uint16_t freq)
{
	uint16_t cca = (F_CPU / (freq * 2)) - 1;	// Uses fFRQ formula from doc8331
	TCD0.CCA = cca;
}