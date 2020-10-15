#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"
#include "gpio.h"
#include "music.h"

/**
 * function to set up GPIO mode and interrupts
 */
void setupGPIO()
{
	/**
	 * TODO set input and output pins for the joystick 
	 */

	/**
	 * Example of HW access from C code: turn on joystick LEDs D4-D8 check 
	 * efm32gg.h for other useful register definitions 
	 */
	
	*CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_GPIO;	/* enable GPIO clock */
	*GPIO_PA_CTRL = 2;							/* set high drive strength */
	*GPIO_PA_MODEH = 0x55555555;				/* set pins A8-15 as output */
	//*GPIO_PA_DOUT = 0x0700;       			/* turn on LEDs D4-D8 (LEDs are active* low) */

	*GPIO_PC_MODEL = 0x33333333;
	*GPIO_PC_DOUT = 0xFF;
}


void enableGPIOInterrupt()
{
	*GPIO_IEN = 0x00FF;
	*GPIO_EXTIFALL = 0xFF;		// Only generate interrupt for falling edge
	*GPIO_EXTIPSELL = 0x22222222;	// Port C chosen for Interrupt pin selection
}

void handle_gpio(uint16_t button_bitmask)
{	
	switch(button_bitmask) 
	{
		case SW1:
			start_song(1);
			*GPIO_PA_DOUT = ~(button_bitmask<<8);			
			break;
		
		case SW2:
			start_song(2);
			*GPIO_PA_DOUT = ~(button_bitmask<<8);
			break;
		
		case SW3:
			*GPIO_PA_DOUT = ~(button_bitmask<<8);
			break;
		
		case SW4:
			*GPIO_PA_DOUT = ~(button_bitmask<<8);
			break;
		
		case SW5:
			*GPIO_PA_DOUT = ~(button_bitmask<<8);
			break;
		
		case SW6:
			*GPIO_PA_DOUT = ~(button_bitmask<<8);
			break;

		case SW7:
			*GPIO_PA_DOUT = ~(button_bitmask<<8);
			break;

		case SW8:
			*GPIO_PA_DOUT = ~(button_bitmask<<8);
			break;
	}
	
}