/*//////////////////////////////////////////////////////////////////////////////////
 * switch_statements.c
 *
 *  Created: 28 October 2015
 *  Author:  Khaled Hassan and Josh Weaver
 *  Purpose: To provide an example using GPIO port and demonstrate "switch"
 *           statements (case structures).  Read from switches connected to Port D.
 *           Light up LEDs on Port Q in a certain fashion based on Port D. Of course,
 *           switch statements can be used to run any code, not just simple GPIO
 *           actions.
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
    //Initialize GPIO PORTs
    PORTD_DIRCLR = 0x0F;        //set lowest four bits of Port D as input
    PORTQ_DIR = 0x0F;           //set up LEDs on Port Q as output
	
    uint8_t input;  //8bit variable to hold input from Port D
	
    while(1)
    {
        input = PORTD_IN & 0x0F;    //read lowest 4 bits on Port D

        switch(input){
        case 8:
            PORTQ_OUT = ~(0x0C);    // C = 1100
            break;
            // If we don't break out now, the case for 8 will
            // "fall through" to the case for 4, and we will see
            // 0011 on the LEDs, not 1100. Try it!
        case 4:
            PORTQ_OUT = ~(0x03);    // 3 = 0011
            break;
        case 2:
            PORTQ_OUT = ~(0x05);    // 5 = 0101
            break;
        case 1:
            PORTQ_OUT = ~(0x0A);    // A = 1010
            break;
        default:    // what happens if we have more than one switch on? or none at all?
            PORTQ_OUT = ~(0x0F);    // let's display an "error" code in this case
        	
            // Note: I invert each of these values to account for the LEDs being active-low
    	}
    }
}
