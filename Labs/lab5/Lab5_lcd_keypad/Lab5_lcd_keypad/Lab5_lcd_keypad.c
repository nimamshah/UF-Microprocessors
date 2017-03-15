/* Lab5_lcd_keypad.c
 * 
 * Lab 5 LCD Function using a Keypad
 * Name: Nicholas Imamshah
 * Section: 6957
 * TA Name: Daniel Gonzalez
 * Description: The purpose of this program is to control the LCD's function with a keypad 
 */
//////////////////////////////////////INCLUDES///////////////////////////////////////
#include <avr/io.h>
#include "ebi_driver.h"
#include "ebi_init.h"
#include "lcd_init.h"
#include "adc_init.h"
#include "adc_pot.h"
#include "adc_cds.h"
#include "keypad.h"
#include <time.h>
#include <stdlib.h>

//////////////////////////////////INITIALIZATIONS////////////////////////////////////
#define F_CPU 2000000

/////////////////////////////////////PROTOTYPES//////////////////////////////////////
void rough_delay(void);
void long_delay(void);
void fun_func(void);
void gen_seq(uint8_t diff, char *says);
void wait_key(void);
uint8_t simon_says(uint8_t diff, uint8_t speed, char *says);

char *prompt = "Simon Says!";
char *start = "Press any key";
char *quit = "Press * to quit.";

///////////////////////////////////MAIN FUNCTION/////////////////////////////////////
int main(void)
{
	ebi_init();
	lcd_init();
	adc_init();
	keypad_init();
	srand(time(NULL));
	
	
	uint8_t key, loop_key = 0xFF;
	while (1)
	{
		if (loop_key == 0xFF)					// If no loop key, scan for key
		{
			do 
			{
				key = keyscan();
			} while (key == 0xFF);
		}
		else
		{										// Else a loop key has been read, use it
			key = loop_key;
			loop_key = 0xFF;
		}
		
		if (key <= 0x01)						// Toggle LCD On/Off
		{
			lcd_toggle();
		} 
		else if (key <= 0x03)					// Display ADC Voltage reading from Potentiometer
		{
			do 
			{
				lcd_voltage(adc_pot());
				rough_delay();
				loop_key = keyscan();
			} while (loop_key == 0xFF);
		}
		else if (key <= 0x05)					// Send "Nick Imamshah" to LCD Screen
		{
			OUT_STRING("Nick Imamshah");
		}
		else if (key <= 0x07)					// Send phrase to LCD Screen on both lines
		{
			OUT_STRING("May the Schwartz");
			OUT_COMMAND(0xC0);
			OUT_STRING("be with you!");
		}
		else if (key >= 0x0E && key < 0xFF)		// Clear the LCD Screen and control LED with CdS ADC reading
		{
			CLEAR_SCREEN();
			uint16_t cds_volt = cds();
			if (cds_volt < 256)
			{
				__far_mem_write(CS0_Start, 0x01);
			} else
			{
				__far_mem_write(CS0_Start, 0x00);
			}
		}
		else
		{										// Perform a custom operation
			fun_func();
		}
		
		keyhold();
	}
}

/////////////////////////////////////FUNCTIONS///////////////////////////////////////
void rough_delay(void)
{
	for (int i = 0; i < 15000; i++)
	{
		asm volatile ("nop");
	}
}

void long_delay(void)
{
	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 5000; j++)
		{
			asm volatile ("nop");
		}
	}
}

void fun_func(void)
{
	OUT_STRING(prompt);
	wait_key();
	
	CLEAR_SCREEN();
	
	uint8_t key = 0xFF, diff = 5;
	
	OUT_STRING(quit);
	OUT_COMMAND(0xC0);
	OUT_STRING("Press any key");
	do 
	{
		key = keyscan();
	} while (key == 0xFF);
	//wait_key();
	key == 0xFF;
	
	while (key != 0x0E)
	{	
		CLEAR_SCREEN();
		
		uint8_t total_score = 0;			// Initialize the total score
		char *says = 0;
		
		for (int i = 0; i < 3; i++)			// Play 3 rounds
		{
			OUT_STRING("Round ");
			OUT_CHAR((char) (i+1) + '0');
			gen_seq(diff + i*2, says);
			wait_key();
			CLEAR_SCREEN();
			total_score += simon_says(diff + i*2, 14-i*3, says);	// Play game & sum scores
			keyhold();						// Wait for key press to continue to next round
			CLEAR_SCREEN();
		}
		
		OUT_STRING("Total Score: ");		// Output total score
		if (total_score >= 20)				// Handles various ranges of numbers
		{
			uint8_t d1 = total_score/10;
			uint8_t d2 = total_score - 20;
			OUT_CHAR(conv_nibble(d1));
			OUT_CHAR(conv_nibble(d2));
		}
		else if (total_score > 9 && total_score < 20)
		{
			uint8_t d1 = total_score/10;
			uint8_t d2 = total_score - 10;
			OUT_CHAR(conv_nibble(d1));
			OUT_CHAR(conv_nibble(d2));
		} else
		{
			OUT_CHAR((char) total_score + '0');
		}
		long_delay();						// Wait before moving on to options
		
		CLEAR_SCREEN();
		OUT_STRING(quit);
		wait_key();
		
		do 
		{									// Scan for keys
			key = keyscan();
		} while (key == 0xFF);
	}
	CLEAR_SCREEN();
}

void gen_seq(uint8_t diff, char *says)
{
	for (int i = 0; i < diff; i++)
	{
		says[i] = (rand() % 4) + 'A';
	}
	int nulls = rand() % diff;
	for (int i = 0; i < nulls; i++)
	{
		says[rand() % diff] = '0';
	}
	asm volatile ("nop");
}

void wait_key(void)
{
	rough_delay();
	OUT_COMMAND(0xC0);			// Break line first
	OUT_STRING(start);			// Output 'start' string
	uint8_t key;			
	do
	{							// Wait for a key press
		key = keyscan();
	} while (key == 0xFF);
	keyhold();
}

uint8_t simon_says(uint8_t diff, uint8_t speed, char *says)
{
	uint8_t key = 0xFF, score = 0;		// Initialize variables
	for (uint8_t i = 0; i < diff; i++)	// 'diff'iculty determines length of Round
	{
		if (says[i] >= 'A' && says[i] <= 'D')					// Simon said
		{
			OUT_STRING("Simon says: ");
			OUT_CHAR(says[i]);
			keyhold();
			for (int i = 0; i < speed; i++)			// Speed determines how long player
			{										//	has to enter a key
				for (int j = 0; j < 1000; j++)
				{
					key = keyscan();
					if (key != 0xFF) break;
				}
			}
			if (conv_nibble(key & 0x0F) == says[i])	// Check if correct action
			{
				OUT_COMMAND(0xC0);
				OUT_STRING("CORRECT! :-)");
				score += 1;
			}
			else
			{
				OUT_COMMAND(0xC0);
				OUT_STRING("WRONG! :-(");
			}
			long_delay();							// Wait before moving to next letter
			CLEAR_SCREEN();
		}
		else
		{											// Simon DID NOT say
			char c = (rand() % 4) + 'A';
			OUT_STRING("Press this: ");
			OUT_CHAR(c);
			keyhold();
			for (int i = 0; i < speed; i++)			// Speed determines how long player
			{										//	has to enter a key
				for (int j = 0; j < 1000; j++)
				{
					key = keyscan();
					if (key != 0xFF) break;
				}
			}
			if (key == 0xFF)						// Check if correct action
			{
				OUT_COMMAND(0xC0);
				OUT_STRING("Good Job!");
				score += 1;
			}
			else
			{
				OUT_COMMAND(0xC0);
				OUT_STRING("Simon didn't say");
			}
			long_delay();							// Wait before moving to next letter
			CLEAR_SCREEN();
		}
	}
	OUT_STRING("Score: ");							// Output Round score
	OUT_CHAR((char) score + '0');
	OUT_COMMAND(0xC0);
	wait_key();										// Wait for player input before continuing
	return score;
}