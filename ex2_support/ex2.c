#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"
#include "gpio.h"
#include "timer.h"
#include "dac.h"
#include "interrupt_handlers.h"

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

#define  SAMPLE_PERIOD		106	// Combined with a prescaler of 1, will result in ~32768 kHz
#define  SAMPLE_PRESCALER	2

//#define  SAMPLE_PERIOD		54686	// Combined with a prescaler of 8, will result in ~1 Hz
//#define  SAMPLE_PRESCALER	8

/**
 * Declaration of peripheral setup functions 
 */

void setupNVIC();

/**
 * Your code will start executing here 
 */
int main(void)
{
	/**
	 * Call the peripheral setup functions 
	 */
	setupGPIO();
	setupDAC();
	setupTimer(SAMPLE_PERIOD, SAMPLE_PRESCALER);

	/**
	 * Enable interrupt handling 
	 */
	setupNVIC();

	//startTimer();	// Moved to GPIO interrupt_handler, so we can shut off that fucking sound

	/**
	 * TODO for higher energy efficiency, sleep while waiting for
	 * interrupts instead of infinite loop for busy-waiting 
	 */
	while (1) {
//		*GPIO_PA_DOUT = (*GPIO_PC_DIN << 8);
		
	}

	return 0;
}

void setupNVIC()
{
	/**
	 * TODO use the NVIC ISERx registers to enable handling of
	 * interrupt(s) remember two things are necessary for interrupt
	 * handling: - the peripheral must generate an interrupt signal - the
	 * NVIC must be configured to make the CPU handle the signal You will
	 * need TIMER1, GPIO odd and GPIO even interrupt handling for this
	 * assignment. 
	 */

	enableTimerInterrupt();
	enableGPIOInterrupt();

	*ISER0 |= 0x1802;	// Bits 2, 11 and 12 for their corresponding IRQ# channels
}

/**
 * if other interrupt handlers are needed, use the following names:
 * NMI_Handler HardFault_Handler MemManage_Handler BusFault_Handler
 * UsageFault_Handler Reserved7_Handler Reserved8_Handler
 * Reserved9_Handler Reserved10_Handler SVC_Handler DebugMon_Handler
 * Reserved13_Handler PendSV_Handler SysTick_Handler DMA_IRQHandler
 * GPIO_EVEN_IRQHandler TIMER0_IRQHandler USART0_RX_IRQHandler
 * USART0_TX_IRQHandler USB_IRQHandler ACMP0_IRQHandler ADC0_IRQHandler
 * DAC0_IRQHandler I2C0_IRQHandler I2C1_IRQHandler GPIO_ODD_IRQHandler
 * TIMER1_IRQHandler TIMER2_IRQHandler TIMER3_IRQHandler
 * USART1_RX_IRQHandler USART1_TX_IRQHandler LESENSE_IRQHandler
 * USART2_RX_IRQHandler USART2_TX_IRQHandler UART0_RX_IRQHandler
 * UART0_TX_IRQHandler UART1_RX_IRQHandler UART1_TX_IRQHandler
 * LEUART0_IRQHandler LEUART1_IRQHandler LETIMER0_IRQHandler
 * PCNT0_IRQHandler PCNT1_IRQHandler PCNT2_IRQHandler RTC_IRQHandler
 * BURTC_IRQHandler CMU_IRQHandler VCMP_IRQHandler LCD_IRQHandler
 * MSC_IRQHandler AES_IRQHandler EBI_IRQHandler EMU_IRQHandler 
 */
