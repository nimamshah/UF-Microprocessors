/*
 * lab2d.asm
 * 
 * Lab 2 Part D
 * Name: Nicholas Imamshah
 * Section: 6957
 * TA Name: Daniel Gonzalez
 * Description: The purpose of this program is to use the keypad interface developed previously.
 */

.nolist
.include "ATxmega128A1Udef.inc"
.list

.org 0x0000
	rjmp MAIN

.dseg
.org 0x2000
Keys: .byte 5

.cseg
.org 0x0100
;.include "keypad.inc" originally used include, but need one file for submission
MAIN:
	ldi YL, low(Keys)
	ldi YH, high(Keys)

	ldi r17, 0xFF				;set PortE as all OUTPUT
	sts PORTE_DIRSET, r17
	rcall KEYPAD				;jump to LOOP to begin repeated scanning 

/*
 * lab2c.asm
 * 
 * Lab 2 Part C
 * Name: Nicholas Imamshah
 * Section: 6957
 * TA Name: Daniel Gonzalez
 * Description: The purpose of this program is to interface with a keypad.
 */

KEYPAD:
	rcall KEYSCAN			;call the Keypad Scanning subroutine
	st Y+, r16		;store key pressed to Keys table
	cpi YL, 5
	breq CHECK0

	rjmp KEYPRESSED
	rjmp KEYPAD

CHECK0:
	ldi YL, low(Keys)
	ldi YH, high(Keys)
	ld r17, Y+
	cpi r17, 0x03
	brne WRONG

CHECK1:
	ld r17, Y+
	cpi r17, 0x07
	brne WRONG

CHECK2:
	ld r17, Y+
	cpi r17, 0x04
	brne WRONG
	
CHECK3:
	ld r17, Y+
	cpi r17, 0x04
	brne WRONG

CHECK4:
	ld r17, Y+
	cpi r17, 0x0F
	brne WRONG

	ldi r17, 0x80

LOAD_R:
	sts PORTE_OUT, r17
	rcall DELAY
	lsr r17
	cpi r17, 0x01
	brne LOAD_R
LOAD_L:
	sts PORTE_OUT, r17
	rcall DELAY
	lsl r17
	brne LOAD_L
	lsl r17
	sts PORTE_OUT, r17
WRONG:
	ldi YL, low(Keys)
	ldi YH, high(Keys)
	rjmp KEYPAD
	
DELAY:
	ldi r18, 255

DELAY_LOOP:
	dec r18
	
	rcall DELAY_INNER
	cpi r18, 0x00
	brne DELAY_LOOP
	ret

DELAY_INNER:
	ldi r19, 255
DELAY_INNER_LOOP:
	dec r19
	cpi r19, 0x00
	brne DELAY_INNER_LOOP
	ret

KEYPRESSED:
	lds r16, PORTF_IN		;check PortF's input again
	cpi r16, 0xF0			;if it is < 0xF0, then one of the keys are pressed
	brlo KEYPRESSED			;loop until this is not the case
	
	rjmp KEYPAD

KEYSCAN:
	ldi r19, 0x18			;Need OPC set to PULLUP for all Keypad pins
	sts PORTF_PIN7CTRL, r19
	sts PORTF_PIN6CTRL, r19
	sts PORTF_PIN5CTRL, r19
	sts PORTF_PIN4CTRL, r19
	sts PORTF_PIN3CTRL, r19
	sts PORTF_PIN2CTRL, r19
	sts PORTF_PIN1CTRL, r19
	sts PORTF_PIN0CTRL, r19

	ldi r16, 0x0F			;set the LSNibble of PORTF to output
	sts PORTF_DIRSET, r16
	rcall COL1				;call to the first column to scan

	ret

INIT:
	sts PORTF_OUT, r16		;initiates the bits for each columns scan
	nop
	lds r17, PORTF_IN		;get the input bits from PortF
	ori r17, 0x0F			;bit mask the input to simplify code

	ret

COL1:
	ldi r16, 0x0E			;column 1 is 0b1101
	rcall INIT				;check for pressed key

	cpi r17, 0xEF			;check if row 1
	breq PRESS_1
	cpi r17, 0xDF			;check if row 2
	breq PRESS_4
	cpi r17, 0xBF			;check if row 3
	breq PRESS_7
	cpi r17, 0x7F			;check if row 4
	breq PRESS_ST

	rjmp COL2				;move on to column 2

	PRESS_1:				;load value corresponding to key pressed
		ldi r16, 0x01
		ret
	PRESS_4:
		ldi r16, 0x04
		ret
	PRESS_7:
		ldi r16, 0x07
		ret
	PRESS_ST:
		ldi r16, 0x0E
		ret

COL2:
	ldi r16, 0x0D			;column 2 is 0b1011
	rcall INIT				;check for pressed key

	cpi r17, 0xEF			;check if row 1
	breq PRESS_2
	cpi r17, 0xDF			;check if row 2
	breq PRESS_5
	cpi r17, 0xBF			;check if row 3
	breq PRESS_8
	cpi r17, 0x7F			;check if row 4
	breq PRESS_0

	rjmp COL3				;move on to column 3

	PRESS_2:				;load value corresponding to key pressed
		ldi r16, 0x02
		ret
	PRESS_5:
		ldi r16, 0x05
		ret
	PRESS_8:
		ldi r16, 0x08
		ret
	PRESS_0:
		ldi r16, 0x00
		ret

COL3:
	ldi r16, 0x0B			;column 3 is 0b0111
	rcall INIT				;check for pressed key

	cpi r17, 0xEF			;check if row 1
	breq PRESS_3
	cpi r17, 0xDF			;check if row 2
	breq PRESS_6
	cpi r17, 0xBF			;check if row 3
	breq PRESS_9
	cpi r17, 0x7F			;check if row 4
	breq PRESS_NUM

	rjmp COL4				;move on to column 4

	PRESS_3:				;load value corresponding to key pressed
		ldi r16, 0x03
		ret
	PRESS_6:
		ldi r16, 0x06
		ret
	PRESS_9:
		ldi r16, 0x09
		ret
	PRESS_NUM:
		ldi r16, 0x0F
		ret

COL4:
	ldi r16, 0x07			;column 4 is 0b1110
	rcall INIT				;check for pressed key

	cpi r17, 0xEF			;check if row 1
	breq PRESS_A
	cpi r17, 0xDF			;check if row 2
	breq PRESS_B
	cpi r17, 0xBF			;check if row 3
	breq PRESS_C
	cpi r17, 0x7F			;check if row 4
	breq PRESS_D

	rjmp COL1

	PRESS_A:				;load value corresponding to key pressed
		ldi r16, 0x0A
		ret
	PRESS_B:
		ldi r16, 0x0B
		ret
	PRESS_C:
		ldi r16, 0x0C
		ret
	PRESS_D:
		ldi r16, 0x0D
		ret