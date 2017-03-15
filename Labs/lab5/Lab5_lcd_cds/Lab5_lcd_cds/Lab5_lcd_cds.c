/* .c
 * 
 *  C
 * Name: Nicholas Imamshah
 * Section: 6957
 * TA Name: Daniel Gonzalez
 * Description: The purpose of this program is to 
 */
//////////////////////////////////////INCLUDES///////////////////////////////////////
#include <avr/io.h>
#include "ebi_driver.h"
#include "ebi_init.h"
#include "adc_init.h"
#include "lcd_init.h"
#include "adc_pot.h"

//////////////////////////////////INITIALIZATIONS////////////////////////////////////
#define F_CPU 2000000

/////////////////////////////////////PROTOTYPES//////////////////////////////////////
uint16_t cds(void);
void rough_delay(void);

///////////////////////////////////MAIN FUNCTION/////////////////////////////////////
int main(void)
{
	ebi_init();
	adc_init();
	lcd_init();
	while (1)
	{
		lcd_voltage(cds());
		rough_delay();
	}
}

/////////////////////////////////////FUNCTIONS///////////////////////////////////////
uint16_t cds(void)
{
	while (!ADCB.CH1.INTFLAGS);
	ADCB.CH1.INTFLAGS = 0x01;
	if (ADCB.CH1.RES < 256)
	{
		__far_mem_write(CS0_Start, 0x01);
	} else
	{
		__far_mem_write(CS0_Start, 0x00);
	}
	return ADCB.CH1.RES;
}

void rough_delay(void)
{
	for (int i = 0; i < 15000; i++)
	{
		asm volatile ("nop");
	}
}