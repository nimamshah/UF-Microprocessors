/*
 * delayx10ms.inc
 * 
 * Name: Nicholas Imamshah
 * Section: 6957
 * TA Name: Daniel Gonzalez
 * Description: This is a generic delay program that can be parameterized with a desired multiplier.
 */

.equ jcycles = 20
.equ kcycles = 246

DELAY_X_MS:
	dec r16
	rcall JLOOP_INIT
	cpi r16, 0
	brne DELAY_X_MS
	ret

JLOOP_INIT:
	push r16
	ldi r16, jcycles
JLOOP:
	dec r16
	rcall KLOOP_INIT
	cpi r16, 0
	brne JLOOP
	pop r16
	ret

KLOOP_INIT:
	push r16
	ldi r16, kcycles
KLOOP:
	dec r16
	nop
	brne KLOOP
	pop r16
	ret