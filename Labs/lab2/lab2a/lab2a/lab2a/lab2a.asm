/*
 * lab2a.asm
 * 
 * Lab 2 Part A
 * Name: Nicholas Imamshah
 * Section: 6957
 * TA Name: Daniel Gonzalez
 * Description: The purpose of this program is to test the LED circuits of the uPAD.
 */

.nolist	; This works, but the below file can't be removed for lss file.
.include "ATxmega128A1Udef.inc"
.list 

.org 0x0000
	rjmp MAIN

.org 0x0100
MAIN:
	ldi r16, 0xFF			;load 0xFF to r16
	sts PORTE_DIRSET, r16	;set all of PortE to outputs
	sts PORTE_OUT, r16		;set all bits of PortE (Turn on all LEDs)

LOOP:
	rjmp LOOP