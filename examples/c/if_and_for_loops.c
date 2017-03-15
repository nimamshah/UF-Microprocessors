/*//////////////////////////////////////////////////////////////////////////////////
 * if_and_for_loops.c
 *
 *  Modified: 28 October 2015
 *  Author:  Dr. Schwartz, Khaled Hassan, and Josh Weaver
 *  Purpose: To provide an example using GPIO port and demonstrate "if" and "for" 
 *           loops.  Read from switches connected to Port D.  If bit 7 is high, 
 *           continuously complement lower 4 bits of Port D and output on Port Q 
 *           immediately,  If bit 7 is low,  display a shifting pattern on Port Q,
 *           with a 1 second delay between each.
 *
 *           NOTE: This code is meant to work with the uTinkerer, **NOT** the uPAD.
 *			       PortQ's LEDs are active low.
 *///////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////INCLUDES///////////////////////////////////////
#include <avr/io.h>

//////////////////////////////////INITIALIZATIONS////////////////////////////////////
#define F_CPU 2000000       // ATxmega runs at 2MHz

/////////////////////////////////////FUNCTIONs///////////////////////////////////////
/************************************************************************************
* Name:     RoughDelay1sec
* Purpose:  Simple function to create a 1 second delay using a for structure
* Inputs:
* Output:
************************************************************************************/
void RoughDelay1sec(void)
{
    volatile uint32_t ticks;            //Volatile prevents compiler optimization
    for(ticks=0;ticks<=F_CPU;ticks++);  //increment 2e6 times -> ~ 1 sec
}

///////////////////////////////////MAIN FUNCTION/////////////////////////////////////
int main(void)
{
    //Initialize GPIO PORTs
    PORTD_DIRCLR = 0x8F;        // set most significant one and lowest four bits of Port D as input
    PORTQ_DIR = 0x0F;           // set up LEDs on Port Q as output
    
    uint8_t input;              //8bit variable to hold input from Port D
    
    while(1)    //infinite loop
    {
        input = PORTD_IN;   //read from switches on Port D
     
        // (1<<7) is 0b1000 0000.
        // Bitwise AND it with the input, and test if it is equal to zero
        // If not equal, bit 7 is high  
        if( (input & (1<<7) ) != 0 ){ 
            // Complement the input, and mask off all but the lowest four bits
            // Recall that the LEDs on Port Q are active-low,
            // so the LEDs will appear to mirror the switches
            PORTQ_OUT = (~input) & 0x0F;      
            
        // Else, bit 7 was low          
        } else {        
            // Iterate from 0 to 3 to turn out debug leds from right to left.
            for(int i = 0; i < 4; i++){
                PORTQ_OUT = (1 << i);
                RoughDelay1sec();
            }
        }
    }
    return 0;
}
