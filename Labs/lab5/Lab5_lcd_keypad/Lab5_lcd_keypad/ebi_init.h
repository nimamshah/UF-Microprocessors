/* ebi_init.h
 * 
 * EBI Initialization Header
 * Name: Nicholas Imamshah
 * Section: 6957
 * TA Name: Daniel Gonzalez
 * Description: The purpose of this program is to configure the EBI for I/O Ports and LCD.
 */
//////////////////////////////////////INCLUDES///////////////////////////////////////
#include <avr/io.h>

//////////////////////////////////INITIALIZATIONS////////////////////////////////////
#define F_CPU 2000000
#define CS0_Start 0x288000
#define CS0_End	0x289FFF
#define CS1_Start 0x394000
#define CS1_End 0x397FFF

/////////////////////////////////////PROTOTYPES//////////////////////////////////////
void ebi_init();

/////////////////////////////////////FUNCTIONS///////////////////////////////////////
void ebi_init()
{
	PORTH.DIR = 0x37;
	PORTH.OUT = 0x33;
	PORTK.DIR = 0xFF;
	
	EBI.CTRL = EBI_SRMODE_ALE1_gc | EBI_IFMODE_3PORT_gc;
	
	EBI.CS0.BASEADDRH = (uint8_t) (CS0_Start>>16) & 0xFF;
	EBI.CS0.BASEADDRL = (uint8_t) (CS0_Start>>8) & 0xFF;
	EBI.CS0.CTRLA = EBI_CS_MODE_SRAM_gc | EBI_CS_ASPACE_8KB_gc;
	
	EBI.CS1.BASEADDR = (uint16_t) (CS1_Start>>8) & 0xFFFF;
	EBI.CS1.CTRLA = EBI_CS_MODE_SRAM_gc | EBI_CS_ASPACE_16KB_gc;
}