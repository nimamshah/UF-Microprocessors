/* lab3port.asm
 * 
 * Lab 3
 * Name: Nicholas Imamshah
 * Section: 6957
 * TA Name: Daniel Gonzalez
 * Description: The purpose of this program is to configure the EBI.
 */

.nolist
.include "ATxmega128A1Udef.inc"
.list

.set PORT = 0x288000
.set PORT_END = 0x289FFF

.org 0x0000
	rjmp MAIN

.org 0x200 
MAIN:
	ldi r16, 0x17			;Configure PORTH bits 4, 2, 1, and 0 as outputs.
	sts PORTH_DIRSET, r16	;	These are the CS0(L), ALE1(H), RE(L), and WE(L) outputs.
							;	(CS0 is bit 4; ALE1 is bit 2; RE is bit 1; WE is bit 0)

	ldi r16, 0x13			;Default CS0(L), RE(L), and WE(L) to H = false.
	sts PORTH_OUTSET, r16	;	ALE defaults to 0 = L = false.

	ldi r16, 0xFF 			;Set all PORTK pins (A15-A0) to be outputs.
	sts PORTK_DIRSET, r16

	ldi r16, 0xFF 			;Set all PORTJ pins (D7-D0) to be outputs.
	sts PORTJ_DIRSET, r16

	ldi r16, 0x41			;Store 0x41 in EBI_CTRL reg to select 3 port EBI(H,J,K)
	sts EBI_CTRL, r16		;	mode and SRAM ALE1 mode

;Initialize the Z pointer to point to the base address for CS0 in memory
	ldi ZH, high(EBI_CS0_BASEADDR)
	ldi ZL, low(EBI_CS0_BASEADDR)

;Load the middle byte (A15:8) of the three byte addr into a reg and store it as the
;	LOW byte of the Base Address, BASEADDRL.
	ldi r16, byte2(PORT)
	st Z+, r16

;Load the highest byte (A23:16) of the three byte addr into a reg and store it as the
;	HIGH byte of the Base Address, BASEADDRH.
	ldi r16, byte3(PORT)
	st Z, r16

	ldi r16, 0x15			;Set to 8KB CS space and turn on SRAM mode, 0x288000 - 0x289FFF
	sts EBI_CS0_CTRLA, r16

;Steps for using the port expansion
	ldi r16, byte3(PORT)	;initialize a pointer to point to the base addr of the PORT
	sts CPU_RAMPX, r16		;use the CPU_RAMPX reg to set the third byte of the pointer

	ldi XH, high(PORT)		;set the middle (XH) and low (XL) bytes of the pointer as usual
	ldi XL, low(PORT)

TEST:
	ld r16, X 				;read the input port into r16
	nop
	nop
	st X, r16				;write the input stored in r16 to the outport
	nop
	rjmp TEST				;put a breakpoint on me and check r16!