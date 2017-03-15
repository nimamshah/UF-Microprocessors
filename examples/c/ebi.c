/*//////////////////////////////////////////////////////////////////////////////////
 * ebi.c
 *
 *  Modified: 10 March 2014
 *  Author:  Dr. Schwartz, Ivan Bukreyev and Josh Weaver
 *  Purpose: To provide an example for the EBI in C. Program configures both CS0
 *           and CS1 to control two external address ranges.  The example also 
 *           shows how pointers	work for reading and writing to I/O.  Finally, the
 *           example shows how an inline Assembly command (include file) may be
 *           used to access memory locations using 3 bytes.
 *///////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////INCLUDES///////////////////////////////////////
#include <avr/io.h>
#include "ebi_driver.h"

//////////////////////////////////INITIALIZATIONS////////////////////////////////////
#define F_CPU 2000000       // ATxmega runs at 2MHz on Reset.
#define CS0_Start 0x4000
#define CS0_End 0x4FFF
#define CS1_Start 0x370000

/////////////////////////////////////FUNCTIONS///////////////////////////////////////
/************************************************************************************
* Name:     RoughDelay1sec
* Purpose:  Simple function to create a 1 second delay using a for structure
* Inputs:   		 
* Output:   
************************************************************************************/
void RoughDelay1sec(void)
{
    volatile uint32_t ticks;            //Volatile prevents compiler optimization
    for(ticks=0;ticks<=F_CPU;ticks++);	//increment 2e6 times -> ~ 1 sec
}

/************************************************************************************
* Name:     EBI_init
* Purpose:  Function to initialize the desired EBI Ports.  Configures to run IO Port, 
*           SRAM, and LCD.  All CSs and other control signals generate appropriate
*           enables inside CPLD.
* Inputs:
* Output:
************************************************************************************/
void EBI_init()
{
    PORTH.DIR = 0x37;       // Enable RE, WE, CS0, CS1, ALE1
    PORTH.OUT = 0x33;
    PORTK.DIR = 0xFF;       // Enable Address 7:0 (outputs)
    // Do not need to set PortJ to outputs
    
    EBI.CTRL = EBI_SRMODE_ALE1_gc | EBI_IFMODE_3PORT_gc;            // ALE1 multiplexing, 3 port configuration

    EBI.CS0.BASEADDRH = (uint8_t) (CS0_Start>>16) & 0xFF;
    EBI.CS0.BASEADDRL = (uint8_t) (CS0_Start>>8) & 0xFF;            // Set CS0 range to 0x004000 - 0x004FFF
    EBI.CS0.CTRLA = EBI_CS_MODE_SRAM_gc | EBI_CS_ASPACE_4KB_gc;	    // SRAM mode, 4k address space	

    // BASEADDR is 16 bit (word) register. C interface allows you to set low and high parts with 1
    // instruction instead of the previous two
    EBI.CS1.BASEADDR = (uint16_t) (CS1_Start>>8) & 0xFFFF;          // Set CS1 range to 0x370000 - 0x37FFFF
    EBI.CS1.CTRLA = EBI_CS_MODE_SRAM_gc | EBI_CS_ASPACE_64KB_gc;
}

///////////////////////////////////MAIN FUNCTION/////////////////////////////////////
int main(void)
{
    uint8_t volatile *ptr_8 = (uint16_t)CS0_End;        // This is 8 bit pointer and will point to one 8-bit location
    uint16_t volatile *ptr_16 = (uint16_t)CS0_Start;    // This is 16 bit pointer and will point to two 8-bit locations (16-bit)	
	
    //Declare read variable. Volatile prevents compiler optimization of variables
    volatile uint8_t read_8;
    volatile uint16_t read_16;

    EBI_init();	        //Call init EBI function

    read_8 = *ptr_8;    //read 8-bit value into read_8
    read_16 = *ptr_8;   //read 8-bit value into read_16; upper byte of read_16 will be padded with 0x00
    read_16 = *ptr_16;  //read 16-bit value into read_16
	
    *ptr_8 = read_8;    //write 8 bit value (read_8) to one 8-bit memory location
    *ptr_16 = read_16;  //write 16 bit value (read_16) to two 8-bit memory locations
    *ptr_16 = read_8;   //write 8 bit value (read_8) to two 8-bit memory locations, upper byte will be padded with 0x00
	

    ptr_8 = CS0_Start;  //re-initialize pointer point to CS0_Start
    // Write 0xAA to each value pointed to by all points in CS0
    while(ptr_8 <= CS0_End) // Loop until the end of CS0 range is met
    {
        *ptr_8++ = 0xAA;    // Write 0xAA and increment pointer address
    }
	
    ptr_8 = CS0_Start;  //re-initialize pointer point to CS0_Start
    // Read and Write to/from all IO port locations on CS0
    while(ptr_8 <= CS0_End) // Loop until the end of CS0 range is met
    {
        read_8 = *ptr_8;
        asm("nop");
        *ptr_8++ = read_8;
    }
	
    // Show a simple test for CS1 showing the special far memory write and read function
    __far_mem_write(0x370000, 0x55);
    uint8_t mem_value = __far_mem_read(0x370000);
	
    // To test and see if __far_mem_write worked
    if (mem_value == 0x55) {
        asm("nop");
        asm("nop");
    }
	
    while(1);	//infinite loop
}
