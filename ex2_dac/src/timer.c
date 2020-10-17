#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"
#include "timer.h"


/**
* function to setup the timer
* @param period 	Period in number of clock cycles
* @param prescaler 	
* 
*/
void setupSamplingTimer(uint16_t period, uint8_t prescaler)
{
	// enable timer 1 clock
	*CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_TIMER1;

	// set timer 1 top
	*TIMER1_TOP = period;

	// set timer 1 prescaler
	*TIMER1_CTRL |= (prescaler << 24);
}

/**
* function to set the timer 1 top
* @param top value written to timer 1 top
* Since TOPB (top buffer) is used, this value can be used while running the timer
*/
void samplingTimer_setTop(uint16_t top)
{
	*TIMER1_TOPB = top;
}

/**
* function to start the timer 1
* Starts the timer that enabled pushing data to DAC
*/
void startSamplingTimer()
{
	*TIMER1_CMD = TIMER1_CMD_START;
}

/**
* function to stop timer 1
* This will disable pushing data to DAC
* 
*/
void stopSamplingTimer()
{
	*TIMER1_CMD = TIMER1_CMD_STOP;
}


/**
* Sets up timer 2
* By default uses 10 Hz
*/
void setupNoteTimer()
{
	*CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_TIMER2;
	setNoteFreq(10);
	*TIMER2_CTRL |= (8 << 24);
}

/**
* function to set frequency for timer 2. 
* @param freq is the desired frequency (default at 8)
* If the timer is running, use updateNoteFreq() instead
*/
void setNoteFreq(uint16_t freq)
{	
	uint16_t period = 54680 / freq;		// TOP = 54680 gives 1 Hz
	*TIMER2_TOP = period;
}
/**
* function to update frequency for timer 2. 
* @param freq is the desired frequency (default at 8)
* If the timer is NOT running, use setNoteFreq() instead
*/
void updateNoteFreq(uint16_t freq)
{
	uint16_t period = 54680 / freq;		// TOP = 54680 gives 1 Hz
	*TIMER2_TOPB = period; 
}


/**
* function to start timer 2
* This will enable playing songs
*/
void startNoteTimer()
{
	*TIMER2_CMD |= TIMER2_CMD_START;
}

/**
* function to stop timer 2
* This will disable playing songs
*/
void stopNoteTimer()
{
	*TIMER2_CMD |= TIMER2_CMD_STOP;
}

/**
* enables timer interrupts
* Enables interrupts for timer 1 and 2
*/
void enableTimerInterrupts()
{
	*TIMER1_IEN 	|= 1;
	*TIMER2_IEN     |= 1;
}
