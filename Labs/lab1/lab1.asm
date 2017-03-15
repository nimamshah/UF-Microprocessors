/*
 * lab1.asm
 * 
 * Lab 1 Part B
 * Name: Nicholas Imamshah
 * Section: 6957
 * TA Name: Daniel Gonzalez
 * Description: The purpose of this program is to filter data from an array in memory.
 */

.nolist	; This works, but the below file can't be removed for lss file.
.include "ATxmega128A1Udef.inc"
.list 

.equ u_bnd = 0x80
.equ l_bnd = 0x16
.equ NUL = 0x00

.org 0x0000
	rjmp MAIN

.org 0x1BA2
Table: .db 0xA4, 0x70, 0x81, 0x58, 0x58, 0x53, 0x96, 0x17, 0x5D, 0xEE, 0x58, 0xF1, 0x83, 0xDB, 0x55, 0x99, 0x16, 0xC2, 0xD7, 0xF5, 0x00

.dseg
.org 0x2016
OutTable: .byte 256

.cseg
.org 0x200
MAIN:
	ldi ZL, low(Table << 1)		;load the low byte of the Table address into ZL register
	ldi ZH, high(Table << 1)	;load the high byte of the Table address into ZH register

	ldi YL, low(OutTable)		;load the low byte of the OutTable address into YL register
	ldi YH, high(OutTable)		;load the high byte of the OutTable address into YH register

	ldi r17, 0x37				;load 0x37 for XORing later

LOOP:
	lpm r16, Z+		;load value from table and Post-Increment Z

	cpi r16, NUL	;check if we've hit the NUL value (i.e., end of table marker)
	breq DONE		;if we've hit NUL, we're DONE

	cpi r16, l_bnd	;compare value with the lower bound
	brsh CHECK_H	;if >=l_bnd (unsigned), check upper bound

CHECK_H:	
	cpi r16, u_bnd	;compare value with the upper bound
	brsh LOOP		;if >=u_bnd (unsigned), conditions not met, return to LOOP

	eor r16, r17	;XOR value meeting conditions with 0x37
	st Y+, r16		;store value meeting conditions to OutTable
	rjmp LOOP		;continue LOOP

DONE:
	rjmp DONE	;infinite loop because program has completed.
