/*
 * lab2c.asm
 * 
 * Lab 2 Part B
 * Name: Nicholas Imamshah
 * Section: 6957
 * TA Name: Daniel Gonzalez
 * Description: The purpose of this program is to alternate the nibbles of the LED array.
 */

.nolist	; This works, but the below file can't be removed for lss file.
.include "ATxmega128A1Udef.inc"
.list 

.def lower = r18
.def upper = r19

.org 0x0000
	rjmp MAIN

.org 0x0100
MAIN:
	ldi r16, 0xFF			;set all bits of PortE to outputs
	sts PORTE_DIRSET, r16
	ldi lower, 0x0F 		;load named registers with corresponding bits
	ldi upper, 0xF0
	
MSB:
	sts PORTE_OUT, upper	;set MSBits and delay
	rcall DELAY

LSB:
	sts PORTE_OUT, lower	;set LSBits and delay
	rcall DELAY
	rjmp MSB

;Contents of delay.inc
DELAY:
	ldi r16, 108		;load the count(r16) with the number of cycles we need to loop

DELAY_LOOP:
	dec r16				;decrement our count
	brne DELAY_LOOP		;if count != 0, repeat
	
	ret
;.include "delay.inc"
