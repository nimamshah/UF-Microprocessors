/* adc_init.h
 * 
 * ADC Initialization Header
 * Name: Nicholas Imamshah
 * Section: 6957
 * TA Name: Daniel Gonzalez
 * Description: The purpose of this program is to configure the ADC for converting analog input.
 */
//////////////////////////////////////INCLUDES///////////////////////////////////////
#include <avr/io.h>

//////////////////////////////////INITIALIZATIONS////////////////////////////////////
#define F_CPU 2000000

/////////////////////////////////////PROTOTYPES//////////////////////////////////////
void adc_init();

/////////////////////////////////////FUNCTIONS///////////////////////////////////////
void adc_init()
{
	// General ADC Configuration
	ADCB.CTRLB = ADC_CONMODE_bm | ADC_FREERUN_bm;		// High Z, No Limit, Signed, Free Run, 12 Bit
	ADCB.REFCTRL = ADC_REFSEL_AREFB_gc;					// Ext. Ref. from AREFB
	ADCB.PRESCALER = ADC_PRESCALER_DIV4_gc;
	ADCB.EVCTRL = ADC_SWEEP0_bm | ADC_EVACT1_bm;
	
	// ADC Channel Configuration
	ADCB.CH0.MUXCTRL = ADC_CH_MUXPOS_PIN4_gc;			// Pin 4
	ADCB.CH0.INTCTRL = ADC_CH_INTLVL_LO_gc;				// Enable low-level interrupts
	ADCB.CH0.CTRL = ADC_CH_INPUTMODE_SINGLEENDED_gc;	// Single-ended
	
	ADCB.CH1.MUXCTRL = ADC_CH_MUXPOS_PIN5_gc;			// Pin 5
	ADCB.CH1.INTCTRL = ADC_CH_INTLVL_LO_gc;				// Enable low-level interrupts
	ADCB.CH1.CTRL = ADC_CH_INPUTMODE_SINGLEENDED_gc;	// Single-ended
	
	// Begin Conversions
	ADCB.CTRLA = ADC_CH0START_bm | ADC_CH1START_bm | ADC_ENABLE_bm;		// Start Conversion on channel 0 and 1, Enable ADC
	
	PORTB.DIRCLR = 0x33;
}