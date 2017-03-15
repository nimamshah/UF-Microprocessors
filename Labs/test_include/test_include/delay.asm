/*
 * delay.asm
 * 
 * Lab 2 Part C
 * Name: Nicholas Imamshah
 * Section: 6957
 * TA Name: Daniel Gonzalez
 * Description: The purpose of this program is to create a delay.
 */

.equ cycles = 111
.def count = r17

DELAY:
	ldi count, cycles	;load the count(r17) with the number of cycles we need to loop

DELAY_LOOP:
	dec count			;decrement our count
	brne DELAY_LOOP		;if count != 0, repeat
	
	ret