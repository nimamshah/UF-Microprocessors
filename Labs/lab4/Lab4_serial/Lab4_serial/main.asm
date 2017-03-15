/* Lab4_serial.asm
 * 
 * Lab 4 XMEGA USART System
 * Name: Nicholas Imamshah
 * Section: 6957
 * TA Name: Daniel Gonzalez
 * Description: The purpose of this program is to configure the XMEGA USART for communication with a terminal program.
 */

.nolist
.include "ATxmega128A1Udef.inc"
.list

.equ BSel = 289
.equ BScale = -7

.equ Prompt = '?'

.equ StringLength = 10
.dseg
.org 0x2000						;where inputs will be stored, outputs written to
STRING:	.byte StringLength

.cseg
.org 0x0000
	rjmp MAIN

.org 0x0200
MAIN:
	rcall GPIO_INIT
	rcall USART_INIT

REPEAT:
	ldi r16, Prompt				;load Prompt character
	ldi r17, StringLength
	ldi ZL, low(STRING)
	ldi ZH, high(STRING)

LOOP:
	rcall OUT_CHAR				;echo character
	
	rcall IN_CHAR

	st Z+, r16					;store input char
	dec r17
	brne LOOP
	rcall OUT_CHAR				;echo last entered char

	ldi r16, '!'
	rcall OUT_CHAR				;signify end of string
	rcall OUT_STRING

	rjmp REPEAT
/************************************************************************************
* Name:     USART_INIT
* Purpose:  Subroutine to initialize the XMEGA USART system
* Inputs:   None			 
* Outputs:  None
* Affected: r16, USARTD0_CTRLB, USARTD0_CTRLC, USARTD0_BAUDCTRLA, USARTD0_BAUDCTRLB
 ***********************************************************************************/
 USART_INIT:
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
* Purpose:  Subroutine to initialize the XMEGA GPIO for use with the USART System
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
* Name:     OUT_STRING
* Purpose:  Subroutine to output character strings stored in memory
* Inputs:   None			 
* Outputs:  Transmits data via USART
* Affected: r16
 ***********************************************************************************/
OUT_STRING:
	ldi ZL, low(STRING)
	ldi ZH, high(STRING)
PRINT_CHAR:
	ld r16, Z+					;load char pointed to by Z, POST-increment
	cpi r16, 0x00				;check if char is null
	breq PRINT_DONE				;	if null -> DONE printing string
	rcall OUT_CHAR				;	else OUTPUT that char
	rjmp PRINT_CHAR				;repeat

PRINT_DONE:
	ret
/************************************************************************************
* Name:     IN_CHAR
* Purpose:  Subroutine to receive a single character from receiver pin of the USART
* Inputs:   None			 
* Outputs:  r16 loaded with input from SCI
* Affected: r16, USARTD0_STATUS, USARTD0_DATA
 ***********************************************************************************/
IN_CHAR:

RX_POLL:
	lds r16, USARTD0_STATUS		;load the status register
	sbrs r16, 7					;proceed to reading in a char if
								;	the RXB8 flag is set
	rjmp RX_POLL				;else continue polling
	lds r16, USARTD0_DATA		;read the character into r16

	ret