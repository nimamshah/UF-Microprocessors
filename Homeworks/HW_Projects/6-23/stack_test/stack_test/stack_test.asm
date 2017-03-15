;
; stack_test.asm
;
; Author: Nicholas Imamshah
; Section: 6957
; Description: Pedagogical program for understanding stack pointer operations.

.nolist
.include "ATxmega128A1Udef.inc"
.list

.org 0x0000
	rjmp MAIN

.org 0x0100
MAIN:
	ldi r16, 0x37
	push r16
	ldi r16, 0xAB
	push r16
	ldi r16, 0x12
	push r16
	ldi r16, 0xEF
	push r16

	rcall SUBR

	rjmp DONE

SUBR:
	ldi r16, 0x1C
	push r16
	ret

DONE:
	rjmp DONE