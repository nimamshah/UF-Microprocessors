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
.include "keypad.inc"
MAIN:
	ldi YL, low(Keys)
	ldi YH, high(Keys)

	ldi r17, 0xFF				;set PortE as all OUTPUT
	sts PORTE_DIRSET, r17
	rcall KEYPAD				;jump to LOOP to begin repeated scanning 