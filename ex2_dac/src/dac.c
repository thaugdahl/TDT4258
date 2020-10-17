#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"
#include "music.h"
#include "utils.h"
#include "dac.h"
#include "sinewave.h"

static int freq = 1;
static int i = 0;
static int volume = 4;

/**
 * Set up and start DAC
 * 
 * Set prescaler to 32, and OUTMODE to 1 (DAC output to pin enabled. DAC output to ADC and ACMP disabled)
 */ 
void setupDAC() 
{    
	/**
	 * 5 - Prescale : 2^5 = 32 (Clock divided by 32 (base is 14 MHz), if sinemode is enabled frequency will be )
	 * 0 - reserved
	 * 0 - 2 MSB reserved, 2 LSB - REFSEL = 0 (Internal 1.25 V bandgap reference)
	 * 1 - OUTMODE = 1 (PIN - DAC output to pin enabled. DAC output to ADC and ACMP disabled)
	 * 0 - SINEMODE disabled
	*/ 
	    
	*CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_DAC0;
	*DAC0_CTRL = 0x50010;
	*DAC0_CH0CTRL = 0x1;
	*DAC0_CH1CTRL = 0x1;
} 
 
/**
 * Writes value to DAC from the next sine value
 * Sampling timer (TIMER1) interrupt will call this function
 */ 
void advance_sine() 
{
	i++;
	i %= SINEWAVE_LENGTH;	// Sets i back to 0 to avoid i increasing above the maximum length
	int index = i << freq % SINEWAVE_LENGTH;
	
	*DAC0_CH0DATA = sinewave[index] << volume;
	*DAC0_CH1DATA = sinewave[index] << volume;
} 
