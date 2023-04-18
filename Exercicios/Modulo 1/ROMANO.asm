;-------------------------------------------------------------------------------
; MSP430 Assembler Code Template for use with TI Code Composer Studio
;
;
;-------------------------------------------------------------------------------
            .cdecls C,LIST,"msp430.h"       ; Include device header file
            
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;-------------------------------------------------------------------------------
            .text                           ; Assemble into program memory.
            .retain                         ; Override ELF conditional linking
                                            ; and retain current section.
            .retainrefs                     ; And retain any sections that have
                                            ; references to current section.

;-------------------------------------------------------------------------------
RESET       mov.w   #__STACK_END,SP         ; Initialize stackpointer
StopWDT     mov.w   #WDTPW|WDTHOLD,&WDTCTL  ; Stop watchdog timer


;-------------------------------------------------------------------------------
; Main loop here
;-------------------------------------------------------------------------------
NUM 	.equ 	2019						;Indicar número a ser convertido

		CLR 	R5
		CLR		R6
	 	MOV 	#NUM, R5 				; R5 = número a ser convertido
 		MOV 	#RESP, R6 				; R6 = ponteiro para escrever a resposta
 		CALL 	#ALG_ROM 				; chamar subrotina
 		JMP		$ 						; travar execução
 		nop 							; exigido pelo montador

ALG_ROM:								; assumindo que vamos de 1 até 3999, iniciamos tratando dos milhares
		CMP		#0x0BB8, R5				; comparo NUM com 3000
		JHS		ESCREVE_MMM				; NUM >= 3000

		CMP		#0x07D0, R5				; comparo NUM com 2000
		JHS		ESCREVE_MM				; NUM >= 2000

		CMP		#0x03E8, R5				; compara NUM com 1000
		JHS		ESCREVE_M				; NUM >= 1000

		JMP		CENTENAS				; começamos a tratar as centenas
		RET

ESCREVE_MMM:
		SUB		#0x0BB8, R5				; NUM - 3000
		MOV		#0x004D, 0(R6)			; escrevemos M
		MOV		#0x004D, 2(R6)			; escrevemos M
		MOV		#0x004D, 4(R6)			; escrevemos M
		ADD		#6, R6
		JMP		CENTENAS				; começamos a tratar as centenas

ESCREVE_MM:
		SUB		#0x07D0, R5				; NUM - 2000
		MOV		#0x004D, 0(R6)			; escrevemos M
		MOV		#0x004D, 2(R6)			; escrevemos M
		ADD		#4, R6
		JMP		CENTENAS				; começamos a tratar as centenas

ESCREVE_M:
		SUB		#0x03E8, R5				; NUM - 1000
		MOV		#0x004D, 0(R6)			; escrevemos M
		ADD		#2, R6
		JMP		CENTENAS				; começamos a tratar as centenas

CENTENAS:
		CMP		#0x0384, R5				; compara com 900
		JHS		ESCREVE_CM				; NUM >= 900

		CMP		#0x0320, R5				; compara com 800
		JHS		ESCREVE_DCCC			; NUM >= 800

		CMP		#0x02BC, R5				; compara com 700
		JHS		ESCREVE_DCC				; NUM >= 700

		CMP		#0x0258, R5				; compara com 600
		JHS		ESCREVE_DC				; NUM >= 600

		CMP		#0x01F4, R5				; compara com 500
		JHS		ESCREVE_D				; NUM = 500

		CMP 	#0x0190, R5				; compara com 400
		JHS		ESCREVE_CD				; NUM >= 400

		CMP		#0x012C, R5				; compara com 300
		JHS		ESCREVE_CCC				; NUM >= 300

		CMP		#0x00C8, R5				; compara com 200
		JHS		ESCREVE_CC				; NUM >= 200

		CMP 	#0x0064, R5				; compara com 100
		JHS		ESCREVE_C				; NUM >= 100

		JMP		DEZENAS					; começa o tratamento das dezenas

ESCREVE_C:
		SUB		#0x0064, R5				; NUM - 100
		MOV		#0x0043, 0(R6)			; escreve C
		ADD		#2, R6
		JMP 	DEZENAS					; começa o tratamento das dezenas

ESCREVE_CC:
		SUB		#0x00C8, R5				; NUM - 200
		MOV		#0x0043, 0(R6)			; escreve C
		MOV		#0x0043, 2(R6)			; escreve C
		ADD		#4, R6
		JMP 	DEZENAS					; começa o tratamento das dezenas

ESCREVE_CCC:
		SUB		#0x012C, R5				; NUM - 300
		MOV		#0x0043, 0(R6)			; escreve C
		MOV		#0x0043, 2(R6)			; escreve C
		MOV		#0x0043, 4(R6)			; escreve C
		ADD		#6, R6
		JMP 	DEZENAS					; começa o tratamento das dezenas

ESCREVE_CD:
		SUB		#0x0190, R5				; NUM - 400
		MOV		#0x0043, 0(R6)			; escreve C
		MOV		#0x0044, 2(R6)			; escreve D
		ADD		#4, R6
		JMP 	DEZENAS					; começa o tratamento das dezenas

ESCREVE_D:
		SUB		#0x01F4, R5				; NUM - 500
		MOV		#0x0044, 0(R6)			; escreve D
		ADD		#2, R6
		JMP 	DEZENAS					; começa o tratamento das dezenas

ESCREVE_DC:
		SUB		#0x0258, R5				; NUM - 600
		MOV		#0x0044, 0(R6)			; escreve D
		MOV		#0x0043, 2(R6)			; escreve C
		ADD		#4, R6
		JMP 	DEZENAS					; começa o tratamento das dezenas

ESCREVE_DCC:
		SUB		#0x02BC, R5				; NUM - 700
		MOV		#0x0044, 0(R6)			; escreve D
		MOV		#0x0043, 2(R6)			; escreve C
		MOV		#0x0043, 4(R6)			; escreve C
		ADD		#6, R6
		JMP 	DEZENAS					; começa o tratamento das dezenas

ESCREVE_DCCC:
		SUB		#0x0320, R5				; NUM - 800
		MOV		#0x0044, 0(R6)			; escreve D
		MOV		#0x0043, 2(R6)			; escreve C
		MOV		#0x0043, 4(R6)			; escreve C
		MOV		#0x0043, 6(R6)			; escreve C
		ADD		#8, R6
		JMP 	DEZENAS					; começa o tratamento das dezenas

ESCREVE_CM:
		SUB		#0x0384, R5				; NUM - 900
		MOV		#0x0043, 0(R6)			; escreve C
		MOV		#0x004D, 2(R6)			; escreve M
		ADD		#4, R6
		JMP 	DEZENAS					; começa o tratamento das dezenas

DEZENAS:
		CMP		#0x005A, R5				; compara com 90
		JHS		ESCREVE_XC				; NUM >= 90

		CMP		#0x0050, R5				; compara com 80
		JHS		ESCREVE_LXXX			; NUM >= 80

		CMP		#0x0046, R5				; compara com 70
		JHS		ESCREVE_LXX				; NUM >= 70

		CMP		#0x003C, R5				; compara com 60
		JHS		ESCREVE_LX				; NUM >= 60

		CMP		#0x0032, R5				; compara com 50
		JHS		ESCREVE_L				; NUM >= 50

		CMP 	#0x0028, R5				; compara com 40
		JHS		ESCREVE_XL				; NUM >= 40

		CMP		#0x001E, R5				; compara com 30
		JHS		ESCREVE_XXX				; NUM >= 30

		CMP		#0x0014, R5				; compara com 20
		JHS		ESCREVE_XX				; NUM >= 20

		CMP 	#0x000A, R5				; compara com 10
		JHS		ESCREVE_X				; NUM >= 10

		JMP 	UNIDADE					; começa o tratamento das unidades;

ESCREVE_X:
		SUB		#0x000A, R5				; NUM - 10
		MOV		#0x0058, 0(R6)			; escrevemos X
		ADD		#2, R6
		JMP		UNIDADE

ESCREVE_XX:
		SUB		#0x0014, R5				; NUM - 20
		MOV		#0x0058, 0(R6)			; escrevemos X
		MOV		#0x0058, 2(R6)			; escrevemos X
		ADD		#4, R6
		JMP		UNIDADE

ESCREVE_XXX:
		SUB		#0x001E, R5				; NUM - 30
		MOV		#0x0058, 0(R6)			; escrevemos X
		MOV		#0x0058, 2(R6)			; escrevemos X
		MOV		#0x0058, 4(R6)			; escrevemos X
		ADD		#6, R6
		JMP		UNIDADE

ESCREVE_XL:
		SUB		#0x0028, R5				; NUM - 40
		MOV		#0x0058, 0(R6)			; escrevemos X
		MOV		#0x004C, 2(R6)			; escrevemos L
		ADD		#4, R6
		JMP		UNIDADE

ESCREVE_L:
		SUB		#0x0032, R5				; NUM - 50
		MOV		#0x004C, 0(R6)			; escrevemos L
		ADD		#2, R6
		JMP		UNIDADE

ESCREVE_LX:
		SUB		#0x003C, R5				; NUM - 60
		MOV		#0x004C, 0(R6)			; escrevemos L
		MOV		#0x0058, 2(R6)			; escrevemos X
		ADD		#4, R6
		JMP		UNIDADE

ESCREVE_LXX:
		SUB		#0x0046, R5				; NUM - 70
		MOV		#0x004C, 0(R6)			; escrevemos L
		MOV		#0x0058, 2(R6)			; escrevemos X
		MOV		#0x0058, 4(R6)			; escrevemos X
		ADD		#6, R6
		JMP		UNIDADE

ESCREVE_LXXX:
		SUB		#0x0050, R5				; NUM - 80
		MOV		#0x004C, 0(R6)			; escrevemos L
		MOV		#0x0058, 2(R6)			; escrevemos X
		MOV		#0x0058, 4(R6)			; escrevemos X
		MOV		#0x0058, 6(R6)			; escrevemos X
		ADD		#8, R6
		JMP		UNIDADE

ESCREVE_XC:
		SUB		#0x003C, R5				; NUM - 90
		MOV		#0x0058, 0(R6)			; escrevemos X
		MOV		#0x0043, 2(R6)			; escrevemos C
		ADD		#4, R6
		JMP		UNIDADE

UNIDADE:						; verifica as unidades
		CMP		#9, R5
		JEQ		ESCREVE_IX

		CMP		#8, R5
		JEQ		ESCREVE_VIII

		CMP		#7, R5
		JEQ		ESCREVE_VII

		CMP		#6, R5
		JEQ		ESCREVE_VI

		CMP		#5, R5
		JEQ		ESCREVE_V

		CMP		#4, R5
		JEQ		ESCREVE_IV

		CMP		#3, R5
		JEQ		ESCREVE_III

		CMP		#2, R5
		JEQ		ESCREVE_II

		CMP		#1, R5
		JEQ		ESCREVE_I

		CMP		#0, R5	; no caso do 0, apenas retornamos
		RET

ESCREVE_IX:
		SUB		#9, R5				; NUM - 9
		MOV		#0x0049, 0(R6)		; escrevemos I
		MOV		#0x0058, 2(R6)		; escrevemos X
		ADD		#4, R6
		RET

ESCREVE_VIII:
		SUB		#8, R5				; NUM - 8
		MOV		#0x0056, 0(R6)		; escrevemos V
		MOV		#0x0049, 2(R6)		; escrevemos I
		MOV		#0x0049, 4(R6)		; escrevemos I
		MOV		#0x0049, 6(R6)		; escrevemos I
		ADD		#8, R6
		RET

ESCREVE_VII:
		SUB		#7, R5				; NUM - 7
		MOV		#0x0056, 0(R6)		; escrevemos V
		MOV		#0x0049, 2(R6)		; escrevemos I
		MOV		#0x0049, 4(R6)		; escrevemos I
		ADD		#6, R6
		RET

ESCREVE_VI:
		SUB		#6, R5				; NUM - 6
		MOV		#0x0056, 0(R6)		; escrevemos V
		MOV		#0x0049, 2(R6)		; escrevemos I
		ADD		#4, R6
		RET

ESCREVE_V:
		SUB		#5, R5				; NUM - 5
		MOV		#0x0056, 0(R6)		; escrevemos V
		ADD		#2, R6
		RET

ESCREVE_IV:
		SUB		#4, R5				; NUM - 4
		MOV		#0x0049, 0(R6)		; escrevemos I
		MOV		#0x0056, 2(R6)		; escrevemos V
		ADD		#4, R6
		RET

ESCREVE_III:
		SUB		#3, R5				; NUM - 3
		MOV		#0x0049, 0(R6)		; escrevemos I
		MOV		#0x0049, 2(R6)		; escrevemos I
		MOV		#0x0049, 4(R6)		; escrevemos I
		ADD		#6, R6
		RET

ESCREVE_II:
		SUB		#2, R5				; NUM - 2
		MOV		#0x0049, 0(R6)		; escrevemos I
		MOV		#0x0049, 2(R6)		; escrevemos I
		ADD		#4, R6
		RET

ESCREVE_I:
		SUB		#1, R5				; NUM - 1
		MOV		#0x0049, 0(R6)		; escrevemos I
		ADD		#2, R6
		RET


;********************** AREA DE DADOS **********************;
	.data

; Local para armazenar a resposta (RESP = 0x2400)
RESP: .byte "RRRRRRRRRRRRRRRRRR",0

;-------------------------------------------------------------------------------
; Stack Pointer definition
;-------------------------------------------------------------------------------
            .global __STACK_END
            .sect   .stack
            
;-------------------------------------------------------------------------------
; Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET
            
