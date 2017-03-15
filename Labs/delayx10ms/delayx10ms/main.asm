/*
 * delayx10ms.asm
 * 
 * Name: Nicholas Imamshah
 * Section: 6957
 * TA Name: Daniel Gonzalez
 * Description: This is a generic delay program that can be parameterized with a desired multiplier.
 */

.nolist
.include "ATxmega128A1Udef.inc"
.list

.org 0x0000
	rjmp MAIN

.org 0x100
MAIN:
	ldi r16, 1
	rcall DELAY_X_MS
	rjmp DONE

DONE:
	rjmp DONE			;infinite loop

.include "delayx10ms.inc"
