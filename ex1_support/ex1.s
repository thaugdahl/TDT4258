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

	LDR GPIO_O, =GPIO_PA_BASE
	LDR GPIO_I, =GPIO_PC_BASE
	LDR GPIO, =GPIO_BASE

	// Enable Clock
	LDR R9, =CMU_BASE
	LDR R7, [R9, #CMU_HFPERCLKEN0]
	MOV R8, #1
	LSL R8, R8, #CMU_HFPERCLKEN0_GPIO
	ORR R7, R7, R8
	STR R7, [R9, #CMU_HFPERCLKEN0]

	// Low drive strength
	MOV R7, =0x55555555
	STR R7, [GPIO_O, #GPIO_MODEH]

	BL led_test
	LDR R7, =0xFF00
	STR R7, [GPIO_O, #GPIO_DOUT]

	// Set port C 0-7 to input
	LDR R7, =0x33333333
	STR R7, [GPIO_I, #GPIO_MODEL]
	LDR R7, =0xFF
	STR R7, [GPIO_I, #GPIO_DOUT]


	.thumb_func
main:
	BL .

	.thumb_func
sleeper:
	LDR R3, =0x30000
sleeper_loop:
	SUBS R3, #1
	BNE sleeper_loop
	BX lr


    b .  // do nothing
	
	/////////////////////////////////////////////////////////////////////////////
	//
  // GPIO handler
  // The CPU will jump here when there is a GPIO interrupt
	//
	/////////////////////////////////////////////////////////////////////////////
	

		.thumb_func
led_test:
	PUSH {LR}
	BL led_loop


	.thumb_func
led_loop:
	PUSH {LR}
	STR R7, [GPIO_O, #GPIO_DOUT]
	BL wait
	ROR R7, R8
	SUBS R9, #1
	BNE led_loop
	POP{PC}


        .thumb_func
gpio_handler:  

	      b .  // do nothing
	
	/////////////////////////////////////////////////////////////////////////////
        .thumb_func
dummy_handler:  
        b .  // do nothing
