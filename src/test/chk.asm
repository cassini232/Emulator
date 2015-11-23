	TITLE	'Test program for Z80-Disassembler'

;==========================================================================
;	Test program for Z80-Disassembler
;==========================================================================

	LD	SP,STACK	; initialize stack for simulator
	LD	HL,LOOP		; start address for disassembler
	LD	(2000H),HL
LOOP:
	HALT			; stop simulation

PRTSTR:				; print 0 terminated string
	LD	A,(HL)		; next char -> A
	OR	A		; 0 ?
	RET	Z		; yes, done
	OUT	(0),A		; no, print it
	INC	HL		; increase pointer to string
	JP	PRTSTR		; process next char

	DEFS	100H
STACK:
	END
