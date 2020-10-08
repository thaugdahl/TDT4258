#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"
#include "timer.h"


/**
* function to setup the timer
* @param period 
* 
*/
void setupTimer(uint16_t period, uint8_t prescaler)
{
	/**
	 * TODO enable and set up the timer
	 * 
	 * 1. Enable clock to timer by setting bit 6 in CMU_HFPERCLKEN0 
	 * 2. Write the period to register TIMER1_TOP 
	 * 3. Enable timer interrupt generation by writing 1 to TIMER1_IEN 
	 * 4. Start the timer by writing 1 to TIMER1_CMD
	 * 
	 * This will cause a timer interrupt to be generated every (period)
	 * cycles. Remember to configure the NVIC as well, otherwise the
	 * interrupt handler will not be invoked. 
	 */

	// enable timer 1 clock
	*CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_TIMER1;
	// set timer 1 top
	*TIMER1_TOP = period;

	*TIMER1_CTRL |= (prescaler << 24);
}

void setSamplingFrequency(uint32_t frequency)
{
	// NOT CURRENTLY WORKING! Fix it or don't use it. Not necessary as of now, and should probably be deleted 
	uint32_t top;
	uint32_t prescaler = 0;
	bool prescaler_found;

	while(prescaler_found == 0)
	{
		top = 14000000 / (frequency * 2^(prescaler)	);
		if(top <= 0xFFFF)
		{
			prescaler_found = 1;
		}
		else
		{
			prescaler_found = 0;
			prescaler += 1;
		}
	}
	
	setupTimer(top, prescaler);

}

void enableTimerInterrupt()
{
	// enable timer 1 interrupt
	*TIMER1_IEN |= 1;
	
}

void startTimer()
{
	// start timer 1
	*TIMER1_CMD = 1;
}

void stopTimer()
{
	// stop timer 1
	*TIMER1_CMD = 2;
}
