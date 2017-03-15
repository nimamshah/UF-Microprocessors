;
; test_include.asm
;
; Created: 9/14/2016 2:59:27 AM
; Author : nicki
;


; Replace with your application code

.nolist
.include "ATxmega128A1Udef.inc"
.list

.org 0x0000
	rjmp MAIN

.org 0x0100
MAIN:
	rcall DELAY
	rjmp DONE

DONE:
	rjmp DONE

.include "delay.asm"
