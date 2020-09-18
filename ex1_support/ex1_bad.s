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

	LDR R7, =0x2
	STR R7, [GPIO, #GPIO_CTRL]

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

	LDR R7, =0xFFFFFFFF
	STR R7, [GPIO_O, #GPIO_DOUT]
	
	// R0 used for last input from buttons
	MOV R0, 0x0
	B main

.thumb_func
main:
	// R7: Newest input, R0: Old input, R1: Current output, R2: Contains 1 if new input has changed to 1, R3: contains 1 if button is pressed
	LDR R7, [GPIO_I, #GPIO_DIN]
<<<<<<< HEAD
	AND R3, R7, R0
	ORR R2, R7, R0
	CBNZ R3, skip
	MVN R0, R7
skip:
	MVN R2, R2
	CMP R2, 0x0

	BEQ main
=======
	AND R2, R7, R0
>>>>>>> 6dccc0b53ab50b2353ec8e8b20acf70ee6710b0a

	CBZ R7, ret_to_main
	B gpio_handler
	
.thumb_func
ret_to_main:
	B main
	
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
	ROR R1, 0x1F

turn_on_led:
	AND R3, R2, 0x2	//Check SW2 (up on left keypad) is pressed
	CBZ R3, rotate_right

	LDR R8, =0xFEFEFEFE
		
	AND R1, R1, R8
	
rotate_right:
	AND R3, R2, 0x4	//Check SW3 (right on left keypad) is pressed
	CBZ R3, turn_off_led
	
	//rotate_right()
	ROR R1, 0x1
	
turn_off_led:
	AND R3, R2, 0x8	//Check SW4 (down on left keypad) is pressed
	CBZ R3, invert

	LDR R8, =0x01010101

	ORR R1, R1, R8
	
invert:
	AND R3, R2, 0x10	//Check SW5 (left on right keypad) is pressed
	CBZ R3, turn_on_all_led
	
	MVN R1, R1
	
turn_on_all_led:
	AND R3, R2, 0x20	//Check SW6 (up on right keypad) is pressed
	CBZ R3, turn_off_all_led
	
	LDR R1, =0x00000000

turn_off_all_led:
	//Check SW8 (down on right keypad) is pressed
	AND R3, R2, 0x80	
	// If not, check next
	CBZ R3, toggle_drive_strength
	
	LDR R1, =0xFFFFFFFF

.thumb_func
toggle_drive_strength:
	// Check SW7 (Right on right keypad) is pressed
	AND R3, R2, 0x40 	
	// If not, check next
	CBZ R3, gpio_handler_write
	
	// Check current drive strength
	LDR R8, [GPIO_O, GPIO_CTRL]	
	AND R3, R8, 0x2
	
	// If low, set high, else - fall through
	CBZ R3, set_highest_drivestrength


.thumb_func
set_lowest_drivestrength:
	MOV R3, 0x1
	STR R3, [GPIO_O, #GPIO_CTRL]
	B gpio_handler_write

.thumb_func
set_highest_drivestrength:
	MOV R3, 0x2
	STR R3, [GPIO_O, #GPIO_CTRL]
	
gpio_handler_write:	
	MVN R0, R7	

	STR R1,[GPIO_O, #GPIO_DOUT]
	
	B main


	/////////////////////////////////////////////////////////////////////////////
        .thumb_func
dummy_handler:  
        b .  // do nothing
