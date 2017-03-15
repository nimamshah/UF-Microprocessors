/* Lab5_lcd_name.c
 * 
 * Lab 5 LCD Name in C
 * Name: Nicholas Imamshah
 * Section: 6957
 * TA Name: Daniel Gonzalez
 * Description: The purpose of this program is to send out my name to the LCD.
 */
//////////////////////////////////////INCLUDES///////////////////////////////////////
#include <avr/io.h>
#include "ebi_driver.h"

//////////////////////////////////INITIALIZATIONS////////////////////////////////////
#define F_CPU 2000000
#define CS0_Start 0x288000
#define CS0_End	0x289FFF
#define CS1_Start 0x394000
#define CS1_End 0x397FFF
#define LCD_BASEADDR 0x395000

/////////////////////////////////////PROTOTYPES//////////////////////////////////////
void ebi_init();
void lcd_init();
void wait_busy();
void OUT_CHAR(char character);
void OUT_STRING(char *string);

///////////////////////////////////MAIN FUNCTION/////////////////////////////////////
int main(void)
{
	ebi_init();
	lcd_init();
	OUT_STRING("Nick Imamshah");
}

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

void lcd_init()
{
	__far_mem_write(0x288000, 0x00);
	wait_busy();
	__far_mem_write(LCD_BASEADDR, 0x38);	// Two lines
	asm volatile ("nop");
	asm volatile ("nop");
	wait_busy();
	
	__far_mem_write(LCD_BASEADDR, 0x0F);	// Display on; Cursor on; Blink on
	asm volatile ("nop");
	asm volatile ("nop");
	wait_busy();
	
	__far_mem_write(LCD_BASEADDR, 0x01);	// Clear screen; Cursor home
	asm volatile ("nop");
	asm volatile ("nop");
	wait_busy();
}

void wait_busy()
{
	uint8_t result = 0;
	do 
	{
		result = __far_mem_read(LCD_BASEADDR);
	} while (result & 0x80);
}

void OUT_CHAR(char character)
{
	__far_mem_write(LCD_BASEADDR+1, character);
	asm volatile ("nop");
	asm volatile ("nop");
	wait_busy();
}

void OUT_STRING(char *string)
{
	while (*string != '\0')
	{
		OUT_CHAR(*string);
		string++;
	}
}