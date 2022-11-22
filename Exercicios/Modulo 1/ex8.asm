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

; FIB16
MAIN:		CLR		R5
			INC		R5			; inicializa os 2 primeiros numeros da serie(0 e 1)
			MOV		#1, R7
			MOV 	#2, R6

FIB16:		MOV 	R6, R5		; R7 = R6 + R5
			ADD 	R7, R5
			JC 		FIM			; se houver carry chegou no limite

			MOV 	R6, R7		; atualiza os novos números da sequencia Fibonacci
			MOV 	R5, R6
			JMP 	FIB16

FIM:
			MOV		R5, R10		; guarda em R10 o maior valor possivel em 16 bits
			ret

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
