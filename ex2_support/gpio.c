#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"
#include "gpio.h"
#include "music.h"
    
/**
 * function to set up GPIO modes
 */ 
void setupGPIO() 
{
	*CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_GPIO;	/* enable GPIO clock */
	
	// Set up output pins
	*GPIO_PA_CTRL = 2;							/* set high drive strength */
	*GPIO_PA_MODEH = 0x55555555;				/* set pins A8-15 as output */
	*GPIO_PA_DOUT = 0xFFFF;						/* Ensure all LED-light starts turned off */


	// Set up input pins
	*GPIO_PC_MODEL = 0x33333333;
	*GPIO_PC_DOUT = 0xFF;
} 	
 
 
/**
 * function to enable GPIO interrupts
 */
void enableGPIOInterrupt() 
{
	*GPIO_IEN = 0x00FF;
	*GPIO_EXTIFALL = 0xFF;			// Only generate interrupt for falling edge
	*GPIO_EXTIPSELL = 0x22222222;	// Port C chosen for Interrupt pin selection
} 

/**
 * function to handle button pressed
 * @param button_bitmask holds the bitmask for what putton has been pressed (1 if pressed)
 * Note that buttons are 1-indexed and inputs are 0-indexed!
 */ 
void handle_gpio(uint16_t button_bitmask) 
{
	switch ((button_bitmask) & 0xff)
	{
		case SW1:
			start_song(1);
			break;
		case SW2:
			start_song(2);
			break;
		case SW3:
			start_song(3);
			break;
		case SW4:
			start_song(4);
			break;
		case SW5:
			break;
		case SW6:
			break;
		case SW7:
			break;
		case SW8:
			stop_song();
			break;
		default:
			break;
	}
}
