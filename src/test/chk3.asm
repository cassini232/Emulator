	TITLE	'Test program for Z80-Disassembler'

;==========================================================================
;	Test program for Z80-Disassembler
;==========================================================================

	LD	SP,STACK	; initialize stack for simulator
	LD	BC,1234H	; useless instruction
	LD	DE,5678H	; "
	LD	HL,HELO
	LD	(2000H),HL
	call	prtstr
	halt

PRTSTR:				; print 0 terminated string
	LD	A,(HL)		; next char -> A
	OR	A		; 0 ?
	RET	Z		; yes, done
	OUT	(0),A		; no, print it
	INC	HL		; increase pointer to string
	JP	PRTSTR		; process next char

	defs	10h
HELO:	defm	'fredx'
	defb	00
	DEFS	100H
STACK:
	END
