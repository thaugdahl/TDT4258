        .syntax unified
	
	      .include "efm32gg.s"

	/////////////////////////////////////////////////////////////////////////////
	//
  // Exception vector table
  // This table contains addresses for all exception handlers
	//
	/////////////////////////////////////////////////////////////////////////////
	
        .section .vectors
	
	      .long   stack_top               /* Top of Stack                 */
	      .long   _reset                  /* Reset Handler                */
	      .long   dummy_handler           /* NMI Handler                  */
	      .long   dummy_handler           /* Hard Fault Handler           */
	      .long   dummy_handler           /* MPU Fault Handler            */
	      .long   dummy_handler           /* Bus Fault Handler            */
	      .long   dummy_handler           /* Usage Fault Handler          */
	      .long   dummy_handler           /* Reserved                     */
	      .long   dummy_handler           /* Reserved                     */
	      .long   dummy_handler           /* Reserved                     */
	      .long   dummy_handler           /* Reserved                     */
	      .long   dummy_handler           /* SVCall Handler               */
	      .long   dummy_handler           /* Debug Monitor Handler        */
	      .long   dummy_handler           /* Reserved                     */
	      .long   dummy_handler           /* PendSV Handler               */
	      .long   dummy_handler           /* SysTick Handler              */

	      /* External Interrupts */
	      .long   dummy_handler
	      .long   gpio_handler            /* GPIO even handler */
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   gpio_handler            /* GPIO odd handler */
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler

	      .section .text

	/////////////////////////////////////////////////////////////////////////////
	//
	// Reset handler
  // The CPU will start executing here after a reset
	//
	/////////////////////////////////////////////////////////////////////////////

	      .globl  _reset
	      .type   _reset, %function
        .thumb_func
_reset: 
	// Set up aliases for constant-valued registers
	GPIO_O .req R4
	GPIO_I .req R5
	GPIO .req R6

	// Keep the base addresses in three dedicated registers.
	// Port A is used to drive the LEDs
	LDR GPIO_O, =GPIO_PA_BASE

	// Port C is used to read the button inputs
	LDR GPIO_I, =GPIO_PC_BASE

	// GPIO Settings and BASE
	LDR GPIO, =GPIO_BASE

	// Enable Clock
	LDR R9, =CMU_BASE
	LDR R7, [R9, #CMU_HFPERCLKEN0]
	MOV R8, #1
	LSL R8, R8, #CMU_HFPERCLKEN0_GPIO
	ORR R7, R7, R8
	STR R7, [R9, #CMU_HFPERCLKEN0]

	// Low drive strength
	LDR R7, =0x55555555
	STR R7, [GPIO_O, #GPIO_MODEH]

	LDR R7, =0x5555
	STR R7, [GPIO_O, #GPIO_DOUTSET]

	// Set port C 0-7 to input
	LDR R7, =0x33333333
	STR R7, [GPIO_I, #GPIO_MODEL]

	// Set internal pullup for input pins
	LDR R7, =0xFF
	STR R7, [GPIO_I, #GPIO_DOUT]

	LDR R7, =0x22222222
	STR R7, [GPIO, #GPIO_EXTIPSELL]

	LDR R7, =0xFF
	STR R7, [GPIO, #GPIO_EXTIRISE]
	STR R7, [GPIO, #GPIO_EXTIFALL]
	STR R7, [GPIO, #GPIO_IEN]

	LDR R7, =0x802
	LDR R8, =ISER0
	STR R7, [R8]	
	

	B main

	.thumb_func
main:
	LDR R7, =0x6
	LDR R8, =SCR
	STR R7, [R8]
	WFI


	
	/////////////////////////////////////////////////////////////////////////////
	//
  // GPIO handler
  // The CPU will jump here when there is a GPIO interrupt
	//
	/////////////////////////////////////////////////////////////////////////////
	
        .thumb_func
gpio_handler:  
	LDR R7, [GPIO, GPIO_IF]
	STR R7, [GPIO, GPIO_IFC]

	LDR R7, [GPIO_I, #GPIO_DIN]
	LSL R7, R7, #8
	STR R7,[GPIO_O, #GPIO_DOUT]
	BX LR
	
	/////////////////////////////////////////////////////////////////////////////
        .thumb_func
dummy_handler:  
        b .  // do nothing
