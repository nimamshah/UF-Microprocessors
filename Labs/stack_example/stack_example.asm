/*
 * Stack1.asm
 *
 *  Modified: 29Jan15 PM
 *  Author: Dr. Schwartz
 *	Quick example of how the stack and stack pointer function
 *	Subroutine copies the elements in a vector
 *	
 *	Memory Maps:
 *	Data Memory: I/O Registers: 0x0000 - 0x00FF
 *	Data Memory: Internal SRAM: 0x2000 - 0x3FFF
 *	Program Memory: FLASH range: 0x000000 - 0x10FFF
  *	Program Memory: Application section (in FLASH): 0x0100 - 0xEFFF  
 *	Note: Programs can go anywhere in the application section of FLASH
 *        and results should be writen to SRAM. 
 */ 

.include "ATxmega128A1Udef.inc"

.EQU	stack_init = 0x2FFF	;initialize stack pointer (between 0x2000 & 0x3FFF)	
							;  Default is 0x3FFF
.EQU	vec_len	= 4			;test vector length

.ORG 0x100
VECTOR:	.DB 1,2,3,4

.DSEG
.ORG 0x2ABC					;where test data will be placed
DATA_TAB:		.BYTE vec_len

.CSEG 
.ORG 0x0000
	rjmp MAIN

.ORG 0x200
MAIN:
	ldi YL, low(stack_init)
	out CPU_SPL, YL			;initialize low byte of stack pointer 
; sts or out will work above, but out will ONLY work for addresses 
;   less than 63 = 0x3F; CPU_SPL = 61, see ATxmega128A1Udef.inc
	ldi YL, high(stack_init)
	out CPU_SPH, YL			;initialize high byte of stack pointer 
; sts or out will work above, but out will ONLY work for addresses 
;   less than 63 = 0x3F; CPU_SPH = 62, see ATxmega128A1Udef.inc
	ldi R16, vec_len
	push R16				;save something on stack
	pop  R17				;pop it back into a different register

	ldi XL, low(DATA_TAB)
	ldi XH, high(DATA_TAB)

	push XL					;save XL and XH onto the stack
	push XH
	pop  R17				;pop the two bytes into different registers
	pop  YL

	ldi ZL, low(VECTOR<<1)
	ldi ZH, high(VECTOR<<1)
		
	rcall COPY_TBL		;call to a subroutine (stores 22 bit = 3 byte PC on the stack)
		
HERE:	
	rjmp HERE

.org 0x300	;put this here only to know the address of subroutine
;*********************SUBROUTINES**************************************
; Subroutine Name: COPY_TBL
; Copies a table in program memory (flash) into data memory (SRAM).
; Inputs: Z (points to table in program memory)
;         X (points to table in data memory)
;		  R16 (size of table to copy)
; Ouputs: Table created at X with R16 elements
; Affected: None

COPY_TBL:
	push XL		;If you don't want registers trashed, then push
	push XH		;  at beginning of subroutine and pop in reverse
	push ZL		;  at the end of the subroutine.
	push ZH
	push R16

COPY:
	lpm R17, Z+	;load vector values from FLASH 
	st X+, R17	;copy (unsorted) vector values to SRAM
	dec R16
	brne COPY	;get vector start address

	pop R16		; Restore these registers to their initial values
	pop ZH		;   upon entering the subroutine
	pop ZL
	pop XH
	pop XL
	ret			;return from subroutine
