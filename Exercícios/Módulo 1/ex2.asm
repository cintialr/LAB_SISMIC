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
MAIN:
			PUSH   R5  ; ENDEREÇO DE INICIO DE UM VETOR DE BYTES
			PUSH   R6  ; MENOR ELEMENTO DO VETOR
			PUSH   R7  ; FREQUENCIA
			PUSH   R8  ; TAM DO VETOR
			CLR    R5
			CLR    R6
			CLR    R7
			CLR    R8

			MOV		#vetor, R5
			MOV		@R5+, R8		; PEGA O TAM DO VETOR E INCREMENTA A POSICAO DO VETOR

MAIOR16:
			CMP		@R5+,R6
			JLO		MAIOR
			JEQ		IGUAL
			DEC		R8				; NÃO É MAIOR NEM IGUAL, ENTÃO NÃO NOS IMPORTA
			JNZ		MAIOR16			; VOLTA PARA VERIFICAR O PROXIMO
			JMP		FIM

MAIOR:
			MOV		-1(R5), R6		; MOV PARA O R6 E DECREMENTA R5
			MOV 	#1, R7			; ENCONTROU UM NOVO MAIOR, R7 = 1
			DEC		R8				; TAM DO VETOR - 1
			JNZ		MAIOR16			; VOLTA PARA VERIFICAR O PROXIMO
			JMP		FIM

IGUAL:
			INC		R7				; IGUAL APENAS INCREMENTA
			DEC		R8				; TAM DO VETOR - 1
			JNZ		MAIOR16			; VOLTA PARA VERIFICAR O PROXIMO
			JMP		FIM

FIM:
			POP    R8  ; TAM DO VETOR
			POP    R7  ; FREQUENCIA
			POP    R6  ; MENOR ELEMENTO DO VETOR
			POP    R5  ; ENDEREÇO DE INICIO DE UM VETOR DE BYTES
			JMp $
			NOP

			.data
vetor:		.byte	6,0,"JOAQUIMJOSE",0
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
            
