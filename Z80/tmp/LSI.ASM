
;**************************************************
;* 						  *
;*  USER-MODIFYABLE ROUTINES AND CONSTANTS FOR    *
;*  HARDWARE-DEPENDENT TERMINAL CHARACTERISTICS   *
;*  AND FUNCTIONS USED BY WORD-MASTER.            *
;*                       RELEASE 1.07   2/21/80   *
;**************************************************
;

; *** SUBROUTINE TO CLEAR SCREEN AND HOME CURSOR ***
;
; THIS VERSION WORKS FOR TWO TYPES OF CRT TERMINALS,
; ADM3-A AND SOROC-IQ/120, AND ONE MEMORY-MAPPED VIDEO
; BOARD, THE IMSAI VIO.
;
	ORG	180H
OUTCHR	EQU	01EFH	;OUTPUT CHARACTER FOR RELEASE 1.07
MEMORY	EQU	29B8H	;USER PATCH AREA FOR RELEASE 1.07
CLRSCRN: NOP! NOP! NOP	;SPACE FOR LONGER PATCH /
	 NOP! NOP! NOP	; MAKE ADDRS MATCH OLD VERSION
    ;FIRST, SEND ESCAPE, *. THIS WORKS FOR SOROC
    ;AND WILL BE ERASED BY SUBSEQUENT ^Z FOR OTHERS.
	MVI A,1BH	;(1) GET ESCAPE CHARACTER
	CALL OUTCHR	;SEND IT TO TERMINAL
	MVI A,'*'	;(2) GET ASTERISK
	CALL OUTCHR	;SEND IT TO TERMINAL
    ;NOW SEND CTRL-Z. THIS WORKS FOR ADM-3A, IMSAI VIO.
	MVI A,1AH	;(3) GET CONTROL-Z CHARACTER
	CALL OUTCHR	;SEND IT AND RETURN TO CALLER
	RET
   ;TO CHANGE ABOVE ROUTINE TO SEND DIFFERENT
   ;CHARACTERS, PATCH IN DESIRED CHARACTER IN
   ;INSTRUCTION AT (1), (2), OR (3) ABOVE.
   ;TO SEND LESS THAN 3 CHARACTERS, PATCH IN A
   ;"RET" AFTER LAST DESIRED "CALL OUTCHR".

;
; *** SUBROUTINE TO POSITION CURSOR AT      ***
; ***     LINE L (0=TOP), COLUMN H (0=LEFT) ***
;
;ADM-3A / IMSAI VIO / SOROC VERSION:
; SENDS ESCAPE, =, Y+20H, X+20H
;
 TCURSOR: MVI A,1BH! CALL OUTCHR	;SEND ESCAPE
	MVI A,'='! CALL OUTCHR	;SEND EQUALS
	MVI A,20H		;ADD 20 HEX...
	ADD L			;.. TO LINE #...
	CALL OUTCHR		;.. AND SEND IT.
	MVI A,20H		;ADD 20 HEX...
	ADD H			;..TO COLUMN NUMBER
	CALL OUTCHR		;..AND GO SEND IT.
	RET

	DB 0,0,0,0,0	;ADDITIONAL PATCH SPACE
	DB 0,0,0,0,0	;...


	;NOTE: BACKSPACE ROUTINE THAT WAS IN
	;PRIOR RELEASES IS NO LONGER NEEDED.

; **** MODIFYABLE CONSTANTS *****

 ;PBEGMEM POINTS TO BEGINNING OF MEMORY TO USE
 ;FOR EDIT BUFFER AND SCRATCHPAD. IF SPACE IS NEEDED
 ;FOR PATCHES, PUT THEM WHERE THIS POINTS AND
 ;INCREASE THIS POINTER. REMEMBER TO USE A LARGE
 ;ENOUGH "SAVE" COMMAND!

PBEGMEM: DW MEMORY

 ;SCREEN SIZE: TAKEN FROM THE FOLLOWING,
 ;EXCEPT SET AUTOMATICALLY TO MATCH HARDWARE
 ;VALUES WHEN IMSAI VIO VIDEO DISPLAY IS IN USE
 ;(DETECTED BY PRESENCE OF THE VIO ROM AT PROPER
 ;ADDRESS, AND CON: IOBYTE FIELD = 2 OR 3).

HITE:	DB 24	;MUST BE EXACT SCREEN HEIGHT IN LINES
WID:	DB 80	;MUST BE <= EXACT SCREEN WIDTH

 ;EREOL CONTAINS THE CHARACTER(S) TO ERASE SCREEN
 ;TO END-OF-LINE WITHOUT MOVING CURSOR, IF SUCH A
 ;CHARACTER IS AVAILABLE IN THE TERMINAL HARDWARE.
 ;IF 0, WILL BE SIMULATED BY SOFTWARE.
 ;AUTOMATICALLY SET TO CTL-U WHEN VIO IS IN USE.

EREOL:	DB 0	;(FIRST) CHARACTER, OR 0 IF NONE
	DB 0	;SECOND CHARACTER IF TERMINAL
		;...REQUIRES 2-CHARACTER
		;...SEQUENCE, ELSE A 0.

 ;NOVIO: IF NON-0, WILL NOT LOOK FOR IMSAI VIO.
 ;PATCH NON-0 IF VIO PROVIOSIONS INTERFERE WITH
 ;YOUR TERMINAL.

NOVIO:	DB 0
	DB 0,0,0	;RESERVED FOR EXPANSION


;DELAYS EXECUTED AFTER VARIOUS TERMINAL FUNCTIONS,
;BEFORE NEXT CHARACTER IS SENT TO TERMINAL. THESE
;ALLOW TIME FOR TERMINAL TO RESPOND, AS REQUIRED
;BY SOME TERMINALS WHEN USED AT HIGH BAUD RATES.
;INCREASE IF YOU EXPERIENCE, FOR EXAMPLE, LOSS OF
;CHARACTERS AFTER CLEAR SCREEN.  EACH DELAY IS
;APPROX NUMBER OF MILLISECONDS ON 4MHZ PROCESSOR;
;DELAY IS TWICE AS LONG AS SHOWN FOR 2MHZ 8080.

DELCLR:	DB 25	;DELAY AFTER CLEAR SCREEN: 25+ MSEC.
DELCUS:	DB 10	;DELAY AFTER POSITION CURSOR: 10+MSEC.
DELERE:	DB 5	;DELAY AFTER ERASE TO EOL: 5+ MSEC.


	DB 0,0,0,0,0		;MORE 
	DB 0,0,0,0,0		;EXTRA
	DB 0,0,0,0,0		;PATCHING
	DB 0,0,0,0,0		;SPACE
                
	END
