#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"
#include "timer.h"


/**
* function to setup the timer
* @param period 
* 
*/
void setupSamplingTimer(uint16_t period, uint8_t prescaler)
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
	
	setupSamplingTimer(top, prescaler);

}


/**
* function to start the timer
* 
*/
void startSamplingTimer()
{
	// start timer 1, which enables pushing data to DAC
	*TIMER1_CMD = 1;
}

/**
* function to stop the timer
* 
*/
void stopSamplingTimer()
{
	// stop timer 1, which disables pushing data to DAC
	*TIMER1_CMD = 2;
}



void setupSemiquiverTimer(uint16_t period, uint8_t prescaler)
{
	// enable timer 0 clock
	*CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_TIMER0;
	// set timer 0 top
	*TIMER0_TOP = period;
	*TIMER0_CTRL |= (prescaler << 24);
}

/**
* function to start the timer
* 
*/
void startSemiquiverTimer()
{
	// start timer 0, which enables playing a sound
	*TIMER0_CMD = 1;
}

/**
* function to stop the timer
* 
*/
void stopSemiquiverTimer()
{
	// stop timer 0, which disables playing a sound
	*TIMER0_CMD = 2;
}



void enableTimerInterrupts()
{
	// enable timer 1 interrupt
	*TIMER1_IEN |= 1;
	*TIMER0_IEN |= 1;
}
