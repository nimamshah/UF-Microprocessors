/* Lab4_serial_menu.asm
 * 
 * Lab 4 Serial Menu
 * Name: Nicholas Imamshah
 * Section: 6957
 * TA Name: Daniel Gonzalez
 * Description: The purpose of this program is to provide a menu with responses by using the XMEGA USART System.
 */

.nolist
.include "ATxmega128A1Udef.inc"
.list

.equ BSel = 289
.equ BScale = -7

.equ Prompt = '?'
.equ CR = 0x0D
.equ LF = 0x0A
.equ ESC = 0x1B
.equ TAB = 0x09
.equ NUL = 0x00

.org 0x100
MENU:	.db "Nick's favorite:", CR, LF
		.db "1.", TAB, "OS/Computer (Mac or PC)", CR, LF
		.db "2.", TAB, "EE/CE Course ", CR, LF
		.db "3.", TAB, "Hobby", CR, LF
		.db "4.", TAB, "Quote", CR, LF
		.db "5.", TAB, "Movie", CR, LF
		.db "6.", TAB, "Re-display menu", CR, LF
		.db "ESC: exit", CR, LF, CR, LF, NUL
FAV_OS:	.db	"Windows, unless Danny is looking, then Linux ", CR, LF, CR, LF, NUL
FAV_CE:	.db	"Design Patterns", CR, LF, CR, LF, NUL
FAV_HO:	.db "Gaming with friends", CR, LF, CR, LF, NUL
FAV_QT:	.db "From an archery instructor: 'The reason you are missing, is because you are focusing on the target and not on your actions.' ", CR, LF, CR, LF, NUL
FAV_MV:	.db	"Kung Fury", CR, LF, CR, LF, NUL
PR_ESC:	.db	"Done!", CR, LF, CR, LF, NUL
.org 0x0000
	rjmp MAIN

.org 0x0200
MAIN:
	rcall GPIO_INIT
	rcall USART_INIT

REPEAT:
	ldi r16, Prompt				;load Prompt character
	ldi ZL, low(MENU << 1)
	ldi ZH, high(MENU << 1)
	rcall OUT_STRING

	rcall IN_CHAR				;read input
	rcall OUT_CHAR				;echo to console
	push r16					;push to stack
	ldi r16, CR 				;CRLF
	rcall OUT_CHAR
	ldi r16, LF
	rcall OUT_CHAR
	pop r16						;pop input off stack

	cpi r16, '1'
	breq OPT_1					;option 1 chosen
	
	cpi r16, '2'
	breq OPT_2					;option 2 chosen
	
	cpi r16, '3'
	breq OPT_3					;option 3 chosen
	
	cpi r16, '4'
	breq OPT_4					;option 4 chosen
	
	cpi r16, '5'
	breq OPT_5					;option 5 chosen
	
	cpi r16, '6'
	breq REPEAT					;option 6 chosen

	cpi r16, ESC
	breq OPT_E					;option ESC chosen

	rjmp REPEAT

OUTPUT:
	rcall OUT_STRING
	rjmp REPEAT

OPT_1:
	ldi ZL, low(FAV_OS << 1)
	ldi ZH, high(FAV_OS << 1)
	rjmp OUTPUT
OPT_2:
	ldi ZL, low(FAV_CE << 1)
	ldi ZH, high(FAV_CE << 1)
	rjmp OUTPUT
OPT_3:
	ldi ZL, low(FAV_HO << 1)
	ldi ZH, high(FAV_HO << 1)
	rjmp OUTPUT
OPT_4:
	ldi ZL, low(FAV_QT << 1)
	ldi ZH, high(FAV_QT << 1)
	rjmp OUTPUT
OPT_5:
	ldi ZL, low(FAV_MV << 1)
	ldi ZH, high(FAV_MV << 1)
	rjmp OUTPUT
OPT_E:
	ldi ZL, low(PR_ESC << 1)
	ldi ZH, high(PR_ESC << 1)
	rcall OUT_STRING
	rjmp DONE

DONE:
	rjmp DONE
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
* Inputs:   Z pointing to desired output string			 
* Outputs:  Transmits data via USART
* Affected: r16
 ***********************************************************************************/
OUT_STRING:
	;push r16
PRINT_CHAR:
	lpm r16, Z+					;load char pointed to by Z, POST-increment
	cpi r16, NUL				;check if char is null
	breq PRINT_DONE				;	if null -> DONE printing string
	rcall OUT_CHAR				;	else OUTPUT that char
	rjmp PRINT_CHAR				;repeat

	;pop r16
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
								;	the RXCIF flag is set
	rjmp RX_POLL				;else continue polling
	lds r16, USARTD0_DATA		;read the character into r16

	ret