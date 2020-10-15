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
	 * 1. Enable clock to timer by setting bit 6 in CMU_HFPERCLKEN0 :)
	 * 2. Write the period to register TIMER1_TOP :)
	 * 3. Enable timer interrupt generation by writing 1 to TIMER1_IEN :)
	 * 4. Start the timer by writing 1 to TIMER1_CMD :)
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
	uint8_t prescaler_found;

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

void samplingTimer_setTop(uint16_t top){
	*TIMER1_TOPB = top;
}

/**
* function to start the timer
* 
*/
void startSamplingTimer()
{
	// start timer 1, which enables pushing data to DAC
	*TIMER1_CMD = TIMER1_CMD_START;
}

/**
* function to stop the timer
* 
*/
void stopSamplingTimer()
{
	// stop timer 1, which disables pushing data to DAC
	*TIMER1_CMD = TIMER1_CMD_STOP;
}



void setupSemiquaverTimer()
{
	// enable timer 2 clock
	*CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_TIMER2;
	// set timer 2 top
	setSemiquaverFreq(14);

	*TIMER2_CTRL |= (8 << 24); //prescaler = 8
}

/**
* function to set frequency. If the timer is running, use updateSemiquaverFreq() instead
* @param freq is the desired frequency (default at 8)
*/
void setSemiquaverFreq(uint16_t freq)
{	
	uint16_t period = 54680 / freq;		// TOP = 54680 gives 1 Hz
	*TIMER2_TOP = period;
}
/**
* function to update frequency. If the timer is NOT running, use setSemiquaverFreq() instead
* @param freq is the desired frequency (default at 8)
*/
void updateSemiquaverFreq(uint16_t freq)
{
	uint16_t period = 54680 / freq;		// TOP = 54680 gives 1 Hz
	*TIMER2_TOPB = period; 
}


/**
* function to start the timer
* 
*/
void startSemiquaverTimer()
{
	// start timer 0, which enables playing a sound
	//*LETIMER0_CMD |= LETIMER0_CMD_START;
	*TIMER2_CMD |= TIMER2_CMD_START;
}

/**
* function to stop the timer
* 
*/
void stopSemiquaverTimer()
{
	// stop timer 0, which disables playing a sound
	//*LETIMER0_CMD |= LETIMER0_CMD_STOP;
	*TIMER2_CMD |= TIMER2_CMD_STOP;// 0x6; 
}



void enableTimerInterrupts()
{
	// enable timer 1 interrupt
	*TIMER1_IEN 	|= 1;
	*TIMER2_IEN     |= 1;
	//*LETIMER0_IEN 	|= 1;
}
