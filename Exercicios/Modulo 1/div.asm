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

; prog que faça a divisão e retorne o resto da divisão
; realiza a divisão entre um numerador e um denominador
; entrada R12, R13 -> saida R12(resto), R13(quociente)

main:
		PUSH R4
		CLR R4
		MOV #15, R12
		MOV #5, R13
		CALL #DIVISAO
		NOP
DIVISAO:
		MOV R13, R4  ;DENOMINADOR EM R4
		CLR R13			; ZERA R13

DIV_LOOP:
		CMP R4, R12 	; VERIFICA SE O RESTO R12 JA É MENOR Q O DENOMINADOR R4
		JLO DIV_RET

		SUB R4, R12 ; numerador - denominador = resto (R4 -= R12)
		INC R13		; quociente++

		JMP DIV_LOOP

DIV_RET:
		POP R4
		RET


                                            

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
            
