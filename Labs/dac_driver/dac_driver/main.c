/* dac_driver.c
 * 
 * A driver for the DAC
 * Name: Nicholas Imamshah
 * Section: 6957
 * TA Name: Daniel Gonzalez
 * Description: The purpose of this program is to provide useful functions for working with the XMEGA's DAC.
 */
//////////////////////////////////////INCLUDES///////////////////////////////////////
#include <avr/io.h>

//////////////////////////////////INITIALIZATIONS////////////////////////////////////
#define F_CPU 2000000

/////////////////////////////////////PROTOTYPES//////////////////////////////////////
void dac_init(void);
void dac_write(uint16_t data);
int dac_data_empty(void);

///////////////////////////////////MAIN FUNCTION/////////////////////////////////////
int main(void)
{
	dac_init();
	while (1)
	{
		while (!dac_data_empty());
		dac_write(0xFFF);
		while (!dac_data_empty());
		dac_write(0x800);
		while (!dac_data_empty());
		dac_write(0x800);
		while (!dac_data_empty());
		dac_write(0x800);
		while (!dac_data_empty());
		dac_write(0x800);
		while (!dac_data_empty());
		dac_write(0x800);
		while (!dac_data_empty());
		dac_write(0x800);
		while (!dac_data_empty());
		dac_write(0x800);
		while (!dac_data_empty());
		dac_write(0x800);
		while(!dac_data_empty());
		dac_write(0);	
	}
}

/////////////////////////////////////FUNCTIONS///////////////////////////////////////
void dac_init(void) 
{
	// Configure DAC for single channel 0 w/ reference AREFB and then enable channel and DAC.
	DACB.CTRLB = DAC_CHSEL_SINGLE_gc;
	DACB.CTRLC = DAC_REFSEL_AREFB_gc;
	DACB.CTRLA = DAC_CH0EN_bm | DAC_ENABLE_bm; 
}

void dac_write(uint16_t data) 
{
	DACB.CH0DATA = data;
}

int dac_data_empty(void) 
{
	return (DACB.STATUS & DAC_CH0DRE_bm);
}