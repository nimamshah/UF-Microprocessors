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

.org 0x0100
.include "keypad.inc"
MAIN:
	ldi r17, 0xFF			;set PortE as all OUTPUT
	sts PORTE_DIRSET, r17
	rcall KEYPAD				;jump to LOOP to begin repeated scanning 