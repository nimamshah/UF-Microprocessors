/* Lab5_lcd_voltage.c
 * 
 * Lab 5 LCD Voltage in C
 * Name: Nicholas Imamshah
 * Section: 6957
 * TA Name: Daniel Gonzalez
 * Description: The purpose of this program is to 
 */
//////////////////////////////////////INCLUDES///////////////////////////////////////
#include <avr/io.h>
#ifndef LCD_BASEADDR
#include "lcd_init.h"
#endif

//////////////////////////////////INITIALIZATIONS////////////////////////////////////
#define F_CPU 2000000
#define ADC_RATIO 0.001221

/////////////////////////////////////PROTOTYPES//////////////////////////////////////
uint16_t adc_pot(void);
void lcd_voltage(uint16_t volt);
uint8_t conv_nibble(uint8_t nib);

/////////////////////////////////////FUNCTIONS///////////////////////////////////////
uint16_t adc_pot(void) 
{
	while (!ADCB.CH0.INTFLAGS);
	ADCB.CH0.INTFLAGS = 0x01;
	return ADCB.CH0.RES;
}

void lcd_voltage(uint16_t volt)
{
	CLEAR_SCREEN();
	
	// Convert ADC value to Decimal Voltage
	volt = volt & 0x07FF;					// We can assume positive, so ignore sign bit.
	volt *= 2;								// Multiply by 2 to account for ADC /2
	float dec_volt = ADC_RATIO*volt;		// Apply formula. 
	
	uint8_t int1, int2, int3, h1, h2, h3;
	char d1, d2, d3;
	float volt2, volt3;
	
	int1 = (uint8_t) dec_volt;				// Determine Decimal representation
	d1 = (char) (int1 + '0');
	volt2 = 10*(dec_volt - int1);
	int2 = (uint8_t) volt2;
	d2 = (char) (int2 + '0');
	volt3 = 10*(volt2 - int2);
	int3 = (uint8_t) volt3;
	d3 = (char) (int3 + '0');
	
	h1 = conv_nibble(volt>>8);				// Obtain ASCII for Hex representation
	h2 = conv_nibble(volt>>4 & 0x0F);
	h3 = conv_nibble(volt & 0x0F);
	
	char string[] = {d1, '.', d2, d3, ' ', 'V', ' ', '(', '0', 'x', h1, h2, h3, ')', '\0'};
	OUT_STRING(string);						// Output Voltmeter reading
}

uint8_t conv_nibble(uint8_t nib)
{
	if (nib < 0xA)
	{
		nib += '0';							// Offset by ASCII '0'
	} else
	{
		nib += 'A' - 0xA;					// Subtract out 0xA so that 0xA => 0, 0xB => 1, etc., then offset by ASCII 'A'
	}
	return nib;
}