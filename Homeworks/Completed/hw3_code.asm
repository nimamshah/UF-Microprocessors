6.1)
	a) IF P >= Q
	cp P, Q
	brsh ELSE

	b) IF Q > P
	cp P, Q
	brlo ELSE

	c) iF P = Q
	cp P, Q
	breq ELSE

6.2)
	a) IF P >= Q
	cp P, Q
	brge ELSE

	b) IF Q > P
	cp P, Q
	brlt ELSE

	c) IF P = Q
	cp P, Q
	breq ELSE

6.11)
	Using a space in memory is a waste since we know the constant before compile time.

6.12) FOR Loop
	ldi r16, 0
LOOP:	
	cpi r16, 10
	breq DONE
	inc r16
	rjmp LOOP
DONE:	
	rjmp DONE

6.15)
	lds r16, K1
	lds r17, K2
	lds r18, K3
WHILE:	
	cp r16, r17
	brlt DONE
	cp r17, r18
	brlt THEN
	mov r17, r18
	inc r16
	rjmp WHILE

THEN:	
	mov r17, r16
	rjmp WHILE

6.16)
iter	K1	K2	K3
------------------
0		1	3	-2
1		2	-2	-2
------------------
1 pass	2	-2	-2

