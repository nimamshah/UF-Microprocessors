;
; hw6-23.asm
;
; Author : Nicholas Imamshah
; Section: 6957
; Description: Purpose of this program is to find the smallest of 
;	32 8-bit unsigned numbers in 32 successive RAM memory locations (BUF)

.nolist
.include "ATxmega128A1Udef.inc"
.list

.equ Table_Size = 32

.org 0x0000
	rjmp MAIN

.org 0x0100
Table: .db 34, 52, 98, 88, 44, 48, 10, 49, 60, 75, 55, 15, 33, 34, 31, 41, 86, 99, 12, 71, 96, 95, 25, 32, 37, 21, 70, 99, 85, 54, 19, 23
;min is 10

.dseg
.org 0x2032
Mini: .byte 1 ;reserve 1 byte for minimum

.cseg
.org 0x0200
MAIN:
	ldi r16, Table_Size	;load table size into register to be used as counter
	ldi ZL, low(Table << 1)
	ldi ZH, high(Table << 1)
	ldi r18, 0xFF	;load highest representable value into r18

LOOP:
	cpi r16, 0		;check if we've finished the table
	breq OUTPUT
	dec r16			;decrement counter
	lpm r17, Z+		;load value from table and Post-Incr Z
	cp r18, r17
	brlo LOOP
	mov r18, r17	;put the smaller value into r18
	rjmp LOOP

OUTPUT:
	sts Mini, r18
	rjmp DONE

DONE:
	rjmp DONE