;	ORG	0200H

	LD	SP,STACK	; initialize stack for simulator

	XOR	A
	LD	HL,BLKBLK	; zero out test block
	LD	B,teststr - BLKBLK
ZLOP:	LD	(HL),A
	INC	HL
	DJNZ	ZLOP
	NOP

	LD	HL,teststr
	CALL	PRTSTR
	NOP
	HALT			; stop simulation

TESTS:
	EXX
	NOP
	EXX
	XOR	A
	NOP
t1:	inc	a
	inc	hl
	jp	t1
	NOP
PRTSTR:				; print 0 terminated string
	LD	A,(HL)		; next char -> A
	OR	A		; 0 ?
	RET	Z		; yes, done
	OUT	(0),A		; no, print it
	INC	HL		; increase pointer to string
	JP	PRTSTR		; process next char

BLKBLK:
	DEFS	3
teststr:
	defm	'Test string'
	defb	0AH
	defb	0
	DEFS	100H
STACK:
	END
