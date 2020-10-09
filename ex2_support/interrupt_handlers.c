#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"
#include "dac.h"
#include "timer.h"
#include "interrupt_handlers.h"
#include "music.h"

/**
 * TIMER1 interrupt handler 
 */

static int counter = 0;

void __attribute__ ((interrupt)) TIMER1_IRQHandler()
{
	/**
	 * TODO feed new samples to the DAC 
	 * Remember to clear the pending interrupt,
	 * by writing 1 to TIMER1_IFC 
	 */
	*TIMER1_IFC |= 1;
	advance_sine();

}

void __attribute__ ((interrupt)) TIMER2_IRQHandler()
{
	/**
	 * TODO feed new samples to the DAC 
	 * Remember to clear the pending interrupt,
	 * by writing 1 to TIMER2_IFC 
	 */

	*TIMER2_IFC |= 0x1;
	//advance_music();
	counter++;
	*GPIO_PA_DOUT ^= counter;
}

void __attribute__ ((interrupt)) LETIMER0_IRQHandler()
{
	/**
	 * TODO feed new samples to the DAC 
	 * Remember to clear the pending interrupt,
	 * by writing 1 to LETIMER0_IFC 
	 */
	*LETIMER0_IFC |= 0x1;

	advance_music();
	//*GPIO_PA_DOUT ^= 0xAA;
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

	*GPIO_IFC = 0x55;
	//startSampling
	start_song(0);

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

	*GPIO_IFC = 0xAA;
	//stopTimer
	stop_song();
	//Timer();
}
