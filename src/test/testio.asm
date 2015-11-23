	TITLE	'Test program for I/O'

	LD	A,'0'
	out	(0),A
	LD	A,'1'
	out	(1),A
;	LD	A,'X'
;	out	(100),A
	in	a,(0)
	out	(1),A
	HALT
