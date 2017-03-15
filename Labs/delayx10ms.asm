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