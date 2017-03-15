/*
 * delay.asm
 * 
 * Lab 2 Part C
 * Name: Nicholas Imamshah
 * Section: 6957
 * TA Name: Daniel Gonzalez
 * Description: The purpose of this program is to create a delay.
 */

.nolist
.include "ATxmega128A1Udef.inc"
.list

.equ cycles = 111
.def count = r17

.org 0x0000
	rjmp MAIN

.org 0x100
MAIN:
	ldi count, cycles	;load the count(r17) with the number of cycles we need to loop

LOOP:
	dec count			;decrement our count
	brne LOOP			;if count != 0, repeat
	
DONE:
	rjmp DONE			;infinite loop