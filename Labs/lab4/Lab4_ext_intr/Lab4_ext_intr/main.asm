/* Lab4_ext_intr.asm
 * 
 * Lab 4 External Interrupt
 * Name: Nicholas Imamshah
 * Section: 6957
 * TA Name: Daniel Gonzalez
 * Description: The purpose of this program is to configure an interrupt on the XMEGA that will watch for a falling edge.
 */

.nolist
.include "ATxmega128A1Udef.inc"
.list

.org 0x0000
	rjmp MAIN

.org PORTE_INT0_VECT		;place code at the interrupt vector for the PORTE_INT0 interrupt
	jmp ISR_LED_COUNT			;jump to our interrupt routine

;must org program at 0x0200 so it doesn't conflict with interrupt vectors that are at 0x0000-0x00FE
.org 0x0200
MAIN:
	rcall CONFIG_EBI
	nop
	rcall INIT_INTERRUPT
	nop

	ldi r18, 0				;initialize count

LOOP:
	rjmp LOOP
/************************************************************************************
* Name:     INIT_INTERRUPT
* Purpose:  Subroutine to initialize the PortE external pin interrupt PE0 using INT0
* Inputs:   None			 
* Outputs:  None
* Affected: r16, PMIC_CTRL, PORTE: _INT0MASK_OUT, _DIRCLR, _INTCTRL, _PIN0CTRL
 ***********************************************************************************/
INIT_INTERRUPT:
	ldi r16, 0x01			;set output to default to '1'.
	sts PORTE_OUT, r16
	sts PORTE_DIRCLR, r16	;Set external interrupt pin (PE0) as an input

	ldi r16, 0x01			;select the external interrupt as a low-level priority
	sts PORTE_INTCTRL, r16

	ldi r16, 0x01			;select PORTE_PIN0 as the interrupt source
	sts PORTE_INT0MASK, r16

	ldi r16, 0x02			;select falling edge for external interrupt 
	sts PORTE_PIN0CTRL, r16

	ldi r16, 0x01			;enable low-level interrupts
	sts PMIC_CTRL, r16

	sei						;set global interrupt flag LAST!
	ret
/************************************************************************************
* Name:     ISR_LED_COUNT
* Purpose:  Interrupt service routine to increment the count of executions on LEDs
* Inputs:   None
* Outputs:  None
* Affected: r17, r18
 ***********************************************************************************/
ISR_LED_COUNT:
	ldi r16, 255
	rcall DELAY
	nop

	lds r16, PORTE_IN
	nop
	sbrc r16, 0				;if bit0 is cleared, then continue
	reti					;else, we got a rising edge

	inc r18					;inc each time ISR runs
	st X, r18				;store to LEDs
	ldi r17, 0x01
	sts PORTE_INTFLAGS, r17	;clear the PORTE_INTFLAGS
	reti					;return from the interrupt service routine

DELAY:
	ldi r17, 50
	rcall DELAY_INNER
	dec r16
	nop
	brne DELAY
	ret
DELAY_INNER:
	dec r17
	nop
	brne DELAY_INNER
	ret
/************************************************************************************
* Name:     CONFIG_EBI
* Purpose:  Subroutine to initialize Ports H,J,K/EBI for additional Input/Output ports
* Inputs:   None			 
* Outputs:  None
* Affected: R16, X, Z, EBI_CTRL, PORTH,J,K, 
 ***********************************************************************************/
.set PORT = 0x288000
.set PORT_END = 0x289FFF

CONFIG_EBI:
	ldi r16, 0x17			;Configure PORTH bits 4, 2, 1, and 0 as outputs.
	sts PORTH_DIRSET, r16	;	These are the CS0(L), ALE1(H), RE(L), and WE(L) outputs.
							;	(CS0 is bit 4; ALE1 is bit 2; RE is bit 1; WE is bit 0)

	ldi r16, 0x13			;Default CS0(L), RE(L), and WE(L) to H = false.
	sts PORTH_OUTSET, r16	;	ALE defaults to 0 = L = false.

	ldi r16, 0xFF 			;Set all PORTK pins (A15-A0) to be outputs.
	sts PORTK_DIRSET, r16

	ldi r16, 0xFF 			;Set all PORTJ pins (D7-D0) to be outputs.
	sts PORTJ_DIRSET, r16

	ldi r16, 0x01			;Store 0x41 in EBI_CTRL reg to select 3 port EBI(H,J,K)
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

	ret