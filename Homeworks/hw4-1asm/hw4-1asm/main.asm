/* hw4-1asm.asm
 * 
 * HW 4-1 ASM
 * Name: Nicholas Imamshah
 * Section: 6957
 * TA Name: Daniel Gonzalez
 * Description: The purpose of this program is to compute the average of two 8-bit integers
 */

.nolist
.include "ATxmega128A1Udef.inc"
.list

.org 0x0000
	rjmp MAIN

.org 0x200
MAIN:
	ldi r16, 0x02		;load num1
	ldi r17, 0x04		;load num2
	rcall AVERAGE		;compute average
	nop					;dummy instr for breakpoint

AVERAGE:
	add r16, r17			;add the numbers
	asr r16				;arithmetic shift right to divide by 2
	ret