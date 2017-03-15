/*//////////////////////////////////////////////////////////////////////////////////
 * multiple_while_loops.c
 *
 *  Modified: 28 October 2015
 *  Author:  Dr. Schwartz, Daniel Frank and Josh Weaver
 *  Purpose: To provide a example of multiple while loops.  Use while loops and a
 *           counter to flash LEDs of PortQ.
 *           
 *           NOTE: This code is meant to work with the uTinkerer, **NOT** the uPAD.
 *			       PortQ's LEDs are active low.
 *///////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////INCLUDES///////////////////////////////////////
#include <avr/io.h>

//////////////////////////////////INITIALIZATIONS////////////////////////////////////
#define F_CPU 2000000       // ATxmega runs at 2MHz
int count = 0;  // initialize GLOBAL counter and declare it an integer

/////////////////////////////////////FUNCTIONs///////////////////////////////////////

///////////////////////////////////MAIN FUNCTION/////////////////////////////////////
int main(void)
{
//	PORTQ_DIRSET = 0x0F; //set pins connected to on-board LEDS to be outputs
	PORTQ.DIRSET = 0x0F;  //set pins connected to on-board LEDS to be outputs

    while(1)
    {
        // Loop from count = 0 to counter = 10000
        while(count <= 10000)
        {
            PORTQ_OUT = 0x00;   // Turn on on-board LEDS
            count = count+1;    // increment counter
        }
        
        // Loop from count = 10000 down to count = 1
        while(count >= 1)
        {
            PORTQ_OUT = 0x0F;   //Turn off on-board LEDs
            count = count-1;    //decrement counter
        }
    }
}
