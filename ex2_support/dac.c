#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"
#include "sounds.h"

int freq = 1;
int i = 0;

void setupDAC()
{
	/*
	 * TODO enable and set up the Digital-Analog Converter
	 * 
	 * 1. Enable the DAC clock by setting bit 17 in CMU_HFPERCLKEN0
	 * 2. Prescale DAC clock by writing 0x50010 to DAC0_CTRL 
	 * 3. Enable leftand right audio channels by writing 1 to DAC0_CH0CTRL andDAC0_CH1CTRL 
	 * 4. Write a continuous stream of samples to the DAC data registers, DAC0_CH0DATA and DAC0_CH1DATA, for example from a timer interrupt 
	 */
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

/**TODO: finish comment
 * 
*/
int period;
void set_freq(int frequency){
	if ((frequency < 0) || (frequency > 8)){
		return -1;
	}
	freq = (1 << frequency);
}

/**TODO: finish comment
 * timer interrupt will call this function
*/
void advance_sine(){
	i += i*freq % 256;
	*DAC0_CH0DATA = sinewave[i*freq];
	*DAC0_CH1DATA = sinewave[i*freq];
}