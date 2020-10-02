#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"
#include "dac.h"

/**
 * TIMER1 interrupt handler 
 */

void __attribute__ ((interrupt)) TIMER1_IRQHandler()
{
	/**
	 * TODO feed new samples to the DAC 
	 * Remember to clear the pending interrupt,
	 * by writing 1 to TIMER1_IFC 
	 */
	
	advance_sine();

    *GPIO_PA_DOUT ^= *GPIO_PA_DOUT;
    //*GPIO_PA_DOUT = 0x0F;
    //*(GPIO_PA_DOUT) = 0x55;

	*TIMER1_IFC |= 1;
}

/**
 * GPIO even pin interrupt handler 
 */
void __attribute__ ((interrupt)) GPIO_EVEN_IRQHandler()
{
	/**
	 * TODO handle button pressed event, remember to clear pending
	 * interrupt 
	 */

    *GPIO_IFC |= 1;

    *GPIO_PA_DOUT = *GPIO_PC_DIN << 8;
}

/**
 * GPIO odd pin interrupt handler 
 */
void __attribute__ ((interrupt)) GPIO_ODD_IRQHandler()
{
	/**
	 * TODO handle button pressed event, remember to clear pending
	 * interrupt 
	 */
}
