#include <stdint.h>
#include <stdbool.h>


#include "efm32gg.h"
#include "gpio.h"
#include "timer.h"
#include "dac.h"
#include "interrupt_handlers.h"
#include "music.h"
    
// Allows this Preprocessor Definition to be made in the compiler flags as well
#ifndef INTERRUPT_DISABLED
#define INTERRUPT_ENABLED true
#else
#define INTERRUPT_ENABLED false
#endif	
    
/**
 * TODO calculate the appropriate sample period for the sound wave(s) you 
 * want to generate. The core clock (which the timer clock is derived
 * from) runs at 14 MHz by default. Also remember that the timer counter
 * registers are 16 bits. 
 */ 
    
/**
 * The period between sound samples, in clock cycles 
 * Sampling frequency can be calculated as following:
 * F_s = F_clk/ (2^(SAMPLE_PRESCALER)x(SAMPLE_PERIOD+1)) 
 */ 
    
#define  SAMPLE_PERIOD		493	
#define  SAMPLE_PRESCALER	0
    
/**
 * Declaration of peripheral setup functions 
 */ 

void setupNVIC();

 
/**
 * Your code will start executing here 
 */ 

int main(void) 
{
	//Call the peripheral setup functions   
	setupGPIO();	
	setupDAC();	
	setupSamplingTimer(SAMPLE_PERIOD, SAMPLE_PRESCALER);
	setupNoteTimer();
 
#if INTERRUPT_ENABLED
	//Enable interrupt handling 
	setupNVIC();
#else
	uint16_t button_bitmask = *GPIO_PC_DIN & 0xff;	
#endif	
	    	
	start_song(0);
	while (1) 
	{
#if INTERRUPT_ENABLED
		__asm__ ("wfi");
#else	
		if (*TIMER1_IF && TIMER1_IF_OF)	
 		{
			advance_sine();
			*TIMER1_IFC |= TIMER1_IFC_OF;	
		}

		if (*TIMER2_IF && TIMER2_IF_OF)	
 		{						
			advance_music();	
			*TIMER2_IFC |= 0x1;	
		}
		
		if (button_bitmask != (~(*GPIO_PC_DIN)))
		{			
			button_bitmask = (~(*GPIO_PC_DIN));			
			handle_gpio(button_bitmask);
		}
#endif	
	}
return 0;
}

/**
 * Enables interrupts
 * Enables interrupts and sets the correct bits in the ISER register 
 * bit 1 (GPIO_EVEN), 11 (GPIO_ODD), 12(TIMER1) and 13(TIMER2) 
 */
void setupNVIC() 
{
	enableTimerInterrupts();
	enableGPIOInterrupt();

	*ISER0 |= ISER0_GPIO_EVEN;
	*ISER0 |= ISER0_GPIO_ODD;
	*ISER0 |= ISER0_TIMER1;
	*ISER0 |= ISER0_TIMER2;
} 