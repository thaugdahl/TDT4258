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
 */

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
	advance_music();
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
	uint32_t button_bitmask = *GPIO_IF;

	*GPIO_IFC = 0x55;
	handle_gpio(button_bitmask);

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
	uint32_t button_bitmask = *GPIO_IF;
	handle_gpio(button_bitmask);

	*GPIO_IFC = 0xAA;

}
