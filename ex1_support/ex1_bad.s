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

	// Enable High Frequency Peripheral Clock
	LDR R9, =CMU_BASE
	LDR R7, [R9, #CMU_HFPERCLKEN0]
	MOV R8, #1
	LSL R8, R8, #CMU_HFPERCLKEN0_GPIO
	ORR R7, R7, R8
	STR R7, [R9, #CMU_HFPERCLKEN0]

	// Low drive strength
	LDR R7, =0x55555555
	STR R7, [GPIO_O, #GPIO_MODEH]

	//BL led_test
	LDR R7, =0x5555
	STR R7, [GPIO_O, #GPIO_DOUTSET]

	// Set port C 0-7 to input
	LDR R7, =0x33333333
	STR R7, [GPIO_I, #GPIO_MODEL]

	// Set internal pullup for input pins
	LDR R7, =0xFF
	STR R7, [GPIO_I, #GPIO_DOUT]
	
	// R0 used for last input from buttons
	MOV R0, =0xFFFF
	B gpio_handler

	.thumb_func
main:
	// R7: Newest input, R0: Inverted old input, R1: Current output, R2: Contains 1 if new input has changed to 1, R3: contains 1 if button is pressed
	LDR R7, [GPIO_I, #GPIO_DIN]
	AND R2, R7, R0
	CBZ R2, main
	B gpio_handler
	
	
	/////////////////////////////////////////////////////////////////////////////
	//
	// GPIO handler
	// The CPU will jump here when there is a GPIO interrupt
	//
	/////////////////////////////////////////////////////////////////////////////
	
        .thumb_func
gpio_handler:  
	
	AND R3, R2, 0x1	//Check SW1 (left on left keypad) is pressed
	CBZ R3, turn_on_led

	//rotate_left()
	ROR R1, 0xF

turn_on_led:
	AND R3, R2, 0x2	//Check SW2 (up on left keypad) is pressed
	CBZ R3, rotate_right
	
	ORR R1, R1, 0x01010101
	
rotate_right:
	AND R3, R2, 0x4	//Check SW3 (right on left keypad) is pressed
	CBZ R3, turn_off_led
	
	//rotate_right()
	ROR R1, 0x1
	
turn_off_led:
	AND R3, R2, 0x8	//Check SW4 (down on left keypad) is pressed
	CBZ R3, invert
	
	AND R1, R1, 0xFEFEFEFE
	
invert:
	AND R3, R2, 0x10	//Check SW5 (left on right keypad) is pressed
	CBZ R3, turn_on_all_led
	
	NOT R1, R1
	
turn_on_all_led:
	AND R3, R2, 0x20	//Check SW6 (up on right keypad) is pressed
	CBZ R3, turn_off_all_led
	
	MOV R1, 0xFF

turn_off_all_led:
	AND R3, R2, 0x80	//Check SW8 (down on right keypad) is pressed
	CBZ R3, gpio_handler_write
	
	MOV R1, 0x00
	
gpio_handler_write:	
	NOT R0, R7	

	STR R1,[GPIO_O, #GPIO_DOUT]
	
	B main


	/////////////////////////////////////////////////////////////////////////////
        .thumb_func
dummy_handler:  
        b .  // do nothing
