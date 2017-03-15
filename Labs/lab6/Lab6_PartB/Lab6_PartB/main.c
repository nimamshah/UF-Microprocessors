/* Lab6_PartB.c
 * 
 * Lab 6 Part B
 * Name: Nicholas Imamshah
 * Section: 6957
 * TA Name: Daniel Gonzalez
 * Description: The purpose of this program is to provide a keypad interface mapped to various sounds.
 */
//////////////////////////////////////INCLUDES///////////////////////////////////////
#include <avr/io.h>
#include <avr/interrupt.h>
#include "ebi_driver.h"
#include "ebi_init.h"
#include "lcd_init.h"
#include "keypad.h"

//////////////////////////////////INITIALIZATIONS////////////////////////////////////
#define F_CPU 2000000
#define SCALE 128000

typedef struct note {
	char *name;
	char *ascii_freq;
	uint16_t freq;
} note;

typedef struct beat {
	note n;
	uint16_t d;
} beat;

#define w 4000
#define h 2000
#define q 1000
#define e 500

#define k0 { "A6", "1760.00 Hz", 1760.00 }
#define k1 { "C6", "1046.50 Hz", 1046.50 }
#define k2 { "C#6/Db6", "1108.73 Hz", 1108.73 }
#define k3 { "D6", "1174.66 Hz", 1174.66 }
#define k4 { "D#6/Eb6", "1244.51 Hz", 1244.51 }
#define k5 { "E6", "1318.51 Hz", 1318.51 }
#define k6 { "F6", "1396.91 Hz", 1396.91 }
#define k7 { "F#6/Gb6", "1479.98 Hz", 1479.98 }
#define k8 { "G6", "1567.98 Hz", 1567.98 }
#define k9 { "G#6/Ab6", "1661.22 Hz", 1661.22 }
#define ka { "A#6/Bb6", "1864.66 Hz", 1864.66 }
#define kb { "B6", "1975.53 Hz", 1975.53 }
#define kc { "C7", "2093.00 Hz", 2093.00 }
#define kd { "C#7/Db7", "2217.46 Hz", 2217.46 }
#define knull { "0", "0", 0 }
#define e7 { "E7", "2637.02 Hz", 2637.02 }
#define d7 { "D7", "2349.32 Hz", 2349.32 }
#define wa { "W", "0", 1 }

note notes[] = { k0, k1, k2, k3, k4 , k5, k6, k7, k8, k9, ka, kb, kc, kd, knull };

beat arp_beats[] =
{
	{ k1, q },
	{ k5, q },
	{ k8, q },
	{ kc, q },
	{ k8, q },
	{ k5, q },
	{ k1, q },
	{ knull, w }
};

beat lavender[] = 
{
	{ k1, q },
	{ k8, q },
	{ kb, q },
	{ k7, q },
	{ k1, q },
	{ k8, q },
	{ kb, q },
	{ k7, q },
	{ k1, q },
	{ k8, q },
	{ kb, q },
	{ k7, q }, 
	{ knull, w }
};

beat green_hill[] =
{
	{ kc, e },
	{ k0, q },
	{ kc, e },
	{ kb, q },
	{ kc, e },
	{ kb, e },
	{ kb, e },
	{ k8, q },
	{ k8, e },
	{ k5, e },
	{ k8, e },
	{ e7, e },
	{ d7, q },
	{ kc, e },
	{ kb, q },
	{ kc, e },
	{ kb, e },
	{ kb, e },
	{ k8, q },
	{ k8, e },
	{ kc, e },
	{ k0, q },
	{ kc, e },
	{ kb, q },
	{ kc, e },
	{ kb, e },
	{ kb, e },
	{ k8, q },
	{ k8, e },
	{ k0, e },
	{ k0, e },
	{ k6, q },
	{ k0, e },
	{ k8, q },
	{ k0, e },
	{ k8, e },
	{ k8, e },
	{ k1, q },
	{ k1, e },
	{ knull, w }
};


/////////////////////////////////////PROTOTYPES//////////////////////////////////////
void play(note n);
void play_note(note n, uint16_t d);
void play_sequence(char *name_top, char *name_bottom, beat *b);
void tc_init(void);
uint16_t calc_ffrq(uint16_t freq);
void calc_per(uint16_t period);

///////////////////////////////////MAIN FUNCTION/////////////////////////////////////
int main(void)
{
	ebi_init();
	lcd_init();
	keypad_init();
	tc_init();
	
	uint8_t key;
	while (1)
	{
		do
		{
			key = keyscan();		// Get Key press
		} while (key == 0xFF);
		keyhold();
		TCE1.CTRLFSET = TC_CMD_RESTART_gc;
		
		if (key <= 0x0D)			// Keys 0-D
		{
			calc_per(567.8);
			note n = notes[key];
			play(n);
		}
		else if (key == 0x0E)		// Key *
		{
			play_sequence("Sonic", "Green Hill Zone", green_hill);
		}
		else						// Key #
		{
			play_sequence("Pokemon", "Lavender Town", lavender);
		}
	}
}

/////////////////////////////////////FUNCTIONS///////////////////////////////////////
void play(note n)
{
	CLEAR_SCREEN();
	
	OUT_STRING(n.name);					// Print note and frequency
	OUT_COMMAND(0xC0);
	OUT_STRING(n.ascii_freq);
	
	TCE0.CCA = calc_ffrq(n.freq);		// Calculate CCA for given note frequency
	TCE0.CTRLB |= TC0_CCBEN_bm;			// Enable TCs
	TCE1.CTRLB |= TC1_CCAEN_bm;
}

void play_note(note n, uint16_t d)
{
	calc_per(d);						// Set PER register for note duration
	if (n.freq > 1)						// If freq < 1, do not play (used for waits)
	{
		TCE0.CCA = calc_ffrq(n.freq);	
		TCE0.CTRLB |= TC0_CCBEN_bm;
	}
	TCE1.CTRLB |= TC1_CCAEN_bm;
}

void play_sequence(char *name_top, char *name_bottom, beat *b)
{
	CLEAR_SCREEN();
	
	OUT_STRING(name_top);				// Print name of song selection
	OUT_COMMAND(0xC0);
	OUT_STRING(name_bottom);
	
	int i = 0;
	while (b[i].n.freq != 0)			// Loop until end of sequence reached (knull)
	{
		TCE1.CTRLFSET = TC_CMD_RESTART_gc;
		play_note(b[i].n, b[i].d);
		while(TCE1.CTRLB & TC1_CCAEN_bm);
		i++;
	}
}

void tc_init(void)
{
	PORTE.DIRSET = 0x12;
	
	TCE0.CTRLA = TC_CLKSEL_DIV1_gc;			// Prescaler: CLK
	TCE0.CTRLB = TC_WGMODE_FRQ_gc;			// FRQ Mode
	TCE0.CTRLE = TC_BYTEM_NORMAL_gc;
	
	TCE1.CTRLA = TC_CLKSEL_DIV64_gc;		// Prescaler: CLK/64
	TCE1.CTRLB = TC_WGMODE_NORMAL_gc;		// Normal Mode
	TCE1.CTRLE = TC_BYTEM_NORMAL_gc;
	
	TCE1.INTCTRLA = TC_OVFINTLVL_LO_gc;		// Enable low-level interrupts on overflow
	calc_per(567.8);
	
	PMIC.CTRL |= PMIC_LOLVLEN_bm;
	
	sei();
}

uint16_t calc_ffrq(uint16_t freq)
{
	return (F_CPU / (freq * 2)) - 1;		// Formula from Doc 8331.
}

void calc_per(uint16_t period)
{
	TCE1.PER = ((period * F_CPU) / SCALE) - 1;	// Formula for FRQ from Doc 8331,
												//	rearranged to determine PER. 
}


////////////////////////////////////////ISRs/////////////////////////////////////////
ISR(TCE1_OVF_vect)
{
	TCE1.INTFLAGS = TC1_OVFIF_bm;			// Clear interrupt flag
	TCE1.CTRLB = TC_WGMODE_NORMAL_gc;		// Disable CCs
	TCE0.CTRLB = TC_WGMODE_FRQ_gc; 
}