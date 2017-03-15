/*//////////////////////////////////////////////////////////////////////////////////
 * simple_if_statements.c
 *
 *  Modified: 28 Oct 2015
 *  Author:  Dr. Schwartz, Daniel Frank and Josh Weaver
 *  Purpose: To provide a simple example for if statements.  Read PortF and check 
 *           bit 7.  If it is high, it will turn on PortQ's on-board LEDs.  If it is
 *           low, it will turn them off.
 *           
 *           NOTE: This code is meant to work with the uTinkerer, **NOT** the uPAD.
 *			       PortQ's LEDs are active low.
 *///////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////INCLUDES///////////////////////////////////////
#include <avr/io.h>

//////////////////////////////////INITIALIZATIONS////////////////////////////////////
#define F_CPU 2000000       // ATxmega runs at 2MHz

/////////////////////////////////////FUNCTIONs///////////////////////////////////////

///////////////////////////////////MAIN FUNCTION/////////////////////////////////////
int main(void)
{
    PORTQ_DIRSET = 0x0F;    // set pins connected to on-board LEDS to be outputs
	
    while(1)
    {
        if(PORTF_IN == 0x80)    // check to see if bit 7 on PortF is high
        {
            PORTQ_OUT = 0x00;   // Turn on on-board LEDS
        }
        else
        {
            PORTQ_OUT = 0x0F;   // Turn off on-board LEDS
        }
    }
}
