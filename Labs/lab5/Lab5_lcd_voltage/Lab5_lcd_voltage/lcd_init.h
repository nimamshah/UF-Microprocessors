/* lcd_init.h
 * 
 * LCD Initialization Header
 * Name: Nicholas Imamshah
 * Section: 6957
 * TA Name: Daniel Gonzalez
 * Description: The purpose of this program is to initialize the LCD.
 */
//////////////////////////////////////INCLUDES///////////////////////////////////////
#include <avr/io.h>

//////////////////////////////////INITIALIZATIONS////////////////////////////////////
#define F_CPU 2000000
#define LCD_BASEADDR 0x395000

/////////////////////////////////////PROTOTYPES//////////////////////////////////////
void lcd_init();
void wait_busy();
void OUT_CHAR(char character);
void OUT_STRING(char *string);
void OUT_COMMAND(char command);
void CLEAR_SCREEN(void);

/////////////////////////////////////FUNCTIONS///////////////////////////////////////
void lcd_init()
{
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
	} while (result & 0x80);				// Poll the BF of the LCD
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
	while (*string != '\0')					// Loop until null characer is encountered
	{
		OUT_CHAR(*string);
		string++;
	}
}

void OUT_COMMAND(char command)
{
	__far_mem_write(LCD_BASEADDR, command);
	asm volatile ("nop");
	asm volatile ("nop");
	wait_busy();
}

void CLEAR_SCREEN(void)
{
	__far_mem_write(LCD_BASEADDR, 0x01);
	asm volatile ("nop");
	asm volatile ("nop");
	wait_busy();
}