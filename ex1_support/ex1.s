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

	MOV R7, 0x1
	STR R7, [GPIO_O, #GPIO_CTRL]	

	// Low drive strength
	LDR R7, =0x55555555
	STR R7, [GPIO_O, #GPIO_MODEH]

	// Set port C 0-7 to input
	LDR R7, =0x33333333
	STR R7, [GPIO_I, #GPIO_MODEL]

	// Set internal pullup for input pins
	LDR R7, =0xFF
	STR R7, [GPIO_I, #GPIO_DOUT]

	LDR R1, =0x00000000
	STR R1, [GPIO_O, #GPIO_DOUTSET]

	LDR R7, =0x22222222
	STR R7, [GPIO, #GPIO_EXTIPSELL]

	LDR R7, =0xFF
	//STR R7, [GPIO, #GPIO_EXTIRISE]
	STR R7, [GPIO, #GPIO_EXTIFALL]
	STR R7, [GPIO, #GPIO_IEN]

	LDR R7, =0x802
	LDR R8, =ISER0
	STR R7, [R8]	

	LDR R7, =0x6
	LDR R8, =SCR
	STR R7, [R8]


	B main

.thumb_func
main:
	WFI
	B main


	
	/////////////////////////////////////////////////////////////////////////////
	//
	// GPIO handler
	// The CPU will jump here when there is a GPIO interrupt
	//
	/////////////////////////////////////////////////////////////////////////////
	
.thumb_func
gpio_handler:  
	// Entrypoint for GPIO Interrupts


	// Store Interrupt Flags so they can be referenced and cleared at the end of the handler
	LDR R2, [GPIO, #GPIO_IF]

	// GPIO Handler
	// ------------------------------------------------------------
	// Button Mappings:
	// SW1: Rotate Lights Right
	// SW2: Turn Leftmost Light On
	// SW3: Rotate Lights Left
	// SW4: Turn Leftmost Light Off
	// SW5: Invert Lights
	// SW6: Turn All Lights On
	// SW7: Toggle Drive Strength
	// SW8: Turn All Lights Off
	// ------------------------------------------------------------

	//Check SW1 (left on left keypad) is pressed
	AND R3, R2, 0x1		
	CBZ R3, turn_on_led

	//rotate_left()
	ROR R1, 0x1

.thumb_func
turn_on_led:
	//Check SW2 (up on left keypad) is pressed
	AND R3, R2, 0x2	
	CBZ R3, rotate_right

	LDR R8, =0xFEFEFEFE
	AND R1, R1, R8
	
.thumb_func
rotate_right:
	//Check SW3 (right on left keypad) is pressed
	AND R3, R2, 0x4	
	CBZ R3, turn_off_led
	
	//rotate_right()
	ROR R1, 0x1F
	
.thumb_func
turn_off_led:
	//Check SW4 (down on left keypad) is pressed
	AND R3, R2, 0x8	
	CBZ R3, invert

	LDR R8, =0x01010101

	ORR R1, R1, R8
	
.thumb_func
invert:
	//Check SW5 (left on right keypad) is pressed
	AND R3, R2, 0x10	
	// If not, check next
	CBZ R3, turn_on_all_led
	
	MVN R1, R1
	
.thumb_func
turn_on_all_led:
	//Check SW6 (up on right keypad) is pressed
	AND R3, R2, 0x20	
	// If not, check next
	CBZ R3, turn_off_all_led
	
	LDR R1, =0x00000000

.thumb_func
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
	LDR R7, [GPIO_O, GPIO_CTRL]	
	AND R3, R7, 0x2
	
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
	


.thumb_func
gpio_handler_write:	
	MVN R0, R7

	STR R1,[GPIO_O, #GPIO_DOUT]

	// Clear interrupt flags
	STR R2, [GPIO, #GPIO_IFC]
	
	BX LR


/////////////////////////////////////////////////////////////////////////////
.thumb_func
dummy_handler:  
        b .  // do nothing
