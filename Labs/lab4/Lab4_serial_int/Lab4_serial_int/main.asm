/* Lab4_serial_int.asm
 * 
 * Lab 4 Serial Interrupt
 * Name: Nicholas Imamshah
 * Section: 6957
 * TA Name: Daniel Gonzalez
 * Description: This is an interrupt driven echo program.
 */

.nolist
.include "ATxmega128A1Udef.inc"
.list

.equ BSel = 289
.equ BScale = -7

.org 0x0000
	rjmp MAIN

.org USARTD0_RXC_VECT
	jmp ISR_ECHO

.org 0x0200
MAIN:
	rcall EBI_INIT
	rcall GPIO_INIT
	rcall USART_INIT
	rcall INTERRUPT_INIT

	ldi r16, 0x55
	st X, r16
	ldi r17, 0xFF				;for XORing to create a toggle

LOOP:
	EOR r16, r17				;toggle r16

	st X, r16					;output to LEDs

	ldi r20, 50
	rcall DELAY_X_MS

	rjmp LOOP
/************************************************************************************
* Name:     OUT_CHAR
* Purpose:  Subroutine to output a single character to the transmit pin of the USART
* Inputs:   None			 
* Outputs:  Transmits data via USART
* Affected: USARTD0_STATUS, USARTD0_DATA
 ***********************************************************************************/
OUT_CHAR:
	push r17

TX_POLL:
	lds r17, USARTD0_STATUS		;load status register
	sbrs r17, 5					;proceed to writing out the char if
								;	the DREIF flag is set
	rjmp TX_POLL				;else go back to polling
	sts USARTD0_DATA, r16		;send the character out over the USART
	
	pop r17

	ret
/************************************************************************************
* Name:     USART_INIT
* Purpose:  Subroutine to initialize the XMEGA USART system
* Inputs:   None			 
* Outputs:  None
* Affected: r16, USARTD0_CTRLB, USARTD0_CTRLC, USARTD0_BAUDCTRLA, USARTD0_BAUDCTRLB
 ***********************************************************************************/
 USART_INIT:
	ldi r16, 0x10
	sts USARTD0_CTRLA, r16		;enable low-level interrupts on USART receive

	ldi r16, 0x18
	sts USARTD0_CTRLB, r16		;turn on TXEN and RXEN lines

	ldi r16, 0x23
	sts USARTD0_CTRLC, r16		;Parity = even, 8 bit frame, 1 stop bit

	ldi r16, (BSel & 0xFF)		;select only lower 8 bits of BSel
	sts USARTD0_BAUDCTRLA, r16	;set BAUDCTRLA to lower 8 bits of BSel

	ldi r16, ((BScale << 4) & 0xF0) | ((BSel >> 8) & 0x0F)
	sts USARTD0_BAUDCTRLB, r16	;set BAUDCTRLB to BScale | Bsel.
								;	Lower 4 bits are upper 4 bits of BSel.
								;	Upper 4 bits are upper 4 bits of BScale.
	ret
/************************************************************************************
* Name:     GPIO_INIT
* Purpose:  Subroutine to initialize the XMEGA GPIO for the USART System
* Inputs:   None			 
* Outputs:  None
* Affected: r16, PORTD_DIR, PORTD_OUT, PORTQ_DIR, PORTQ_OUT
 ***********************************************************************************/
 GPIO_INIT:
	ldi r16, 0x08
	sts PORTD_DIRSET, r16		;set PORTD_PIN3 as output for TX pin of USARTD0
	sts PORTD_OUTSET, r16		;set the TX line to default to '1'

	ldi r16, 0x04
	sts PORTD_DIRCLR, r16		;set RX pin for input

	ldi r16, 0xA 
	sts PORTQ_DIRSET, r16		;set pins 1 and 3 of PORTQ to output
	sts PORTQ_OUTCLR, r16		;set USB_SW_EN = '0', USB_SW_SEL = '0'

	ret
/************************************************************************************
* Name:     EBI_INIT
* Purpose:  Subroutine to initialize Ports H,J,K/EBI for additional Input/Output ports
* Inputs:   None			 
* Outputs:  None
* Affected: R16, X, Z, EBI_CTRL, PORTH,J,K, 
 ***********************************************************************************/
.set PORT = 0x288000
.set PORT_END = 0x289FFF

EBI_INIT:
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
/************************************************************************************
* Name:     INTERRUPT_INIT
* Purpose:  Subroutine to initialize the PortE external pin interrupt PE0 using INT0
* Inputs:   None			 
* Outputs:  None
* Affected: r16, PMIC_CTRL, PORTE: _INT0MASK_OUT, _DIRCLR, _INTCTRL, _PIN0CTRL
 ***********************************************************************************/
INTERRUPT_INIT:
	ldi r16, 0x01			;enable low-level interrupts
	sts PMIC_CTRL, r16

	sei						;set global interrupt flag LAST!
	ret
/************************************************************************************
* Name:     ISR_ECHO
* Purpose:  Interrupt service routine to echo char on USART receive pin to transmit
* Inputs:   None
* Outputs:  None
* Affected: USARTD0_STATUS
 ***********************************************************************************/
ISR_ECHO:
	push r16				;push any registers used to ensure they can be used after

	lds r16, USARTD0_DATA	;read character into r16
	rcall OUT_CHAR			;echo character

	ldi r16, 0x80
	sts USARTD0_STATUS, r16	;ensure RXCIF is cleared

	pop r16					;restore registers used in ISR
	reti
/************************************************************************************
* Name:     DELAY_X_MS
* Purpose:  Subroutine to delay for X*10ms
* Inputs:   X stored in r20		 
* Outputs:  None
* Affected: r20
 ***********************************************************************************/
.equ jcycles = 20
.equ kcycles = 246

DELAY_X_MS:
	dec r20
	rcall JLOOP_INIT
	cpi r20, 0
	brne DELAY_X_MS
	ret

JLOOP_INIT:
	push r20
	ldi r20, jcycles
JLOOP:
	dec r20
	rcall KLOOP_INIT
	cpi r20, 0
	brne JLOOP
	pop r20
	ret

KLOOP_INIT:
	push r20
	ldi r20, kcycles
KLOOP:
	dec r20
	nop
	brne KLOOP
	pop r20
	ret