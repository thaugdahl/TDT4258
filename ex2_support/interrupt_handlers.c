#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"
#include "dac.h"
#include "timer.h"
#include "interrupt_handlers.h"
#include "music.h"
#include "gpio.h"
    
/**
 * TIMER1 interrupt handler 
 * Advances to the next value in the sine array and writes to DAC
 */ 
void __attribute__ ((interrupt)) TIMER1_IRQHandler() 
{ 
	*TIMER1_IFC |= 1;
	advance_sine();
} 

/**
 * TIMER2 interrupt handler 
 * Updates TIMER1 to play next note in song
 */  
void __attribute__ ((interrupt)) TIMER2_IRQHandler() 
{	    
	*TIMER2_IFC |= 0x1;
	advance_music();
} 
 

/**
 * GPIO even pin interrupt handler 
 * handle_gpio() used to handle correct behavior according to button press
 */ 
void __attribute__ ((interrupt)) GPIO_EVEN_IRQHandler() 
{
	uint32_t button_bitmask = *GPIO_IF;
	*GPIO_IFC = 0x55;	
	handle_gpio(button_bitmask);
} 
 

/**
 * GPIO odd pin interrupt handler 
 * handle_gpio() used to handle correct behavior according to button press
 */ 
void __attribute__ ((interrupt)) GPIO_ODD_IRQHandler() 
{ 
	uint32_t button_bitmask = *GPIO_IF;
	*GPIO_IFC = 0xAA;
	handle_gpio(button_bitmask);
} 
