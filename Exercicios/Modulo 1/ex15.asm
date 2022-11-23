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

;--------------------------------------------------------------------------------
RESET       mov.w   #__STACK_END,SP         ; Initialize stackpointer
StopWDT     mov.w   #WDTPW|WDTHOLD,&WDTCTL  ; Stop watchdog timer


;-------------------------------------------------------------------------------
; Main loop here
;-------------------------------------------------------------------------------

CONFIGURA_PINOS:
;  P2.1 - S1
	BIC.B   #BIT1, &P2DIR  ; P2.1 = entrada
	BIS.B   #BIT1, &P2REN  ; Habilita resistor
	BIS.B   #BIT1, &P2OUT  ; PULLUP
;  P1.1 - S2
	BIC.B   #BIT1, &P1DIR  ; P1.1 = entrada
	BIS.B   #BIT1, &P1REN  ; Habilita resistor
	BIS.B   #BIT1, &P1OUT  ; PULLUP

;  P1.0 - LED1 VERMELHO
	BIS.B   #BIT0, &P1DIR  ; P1.0 = saida
	BIC.B   #BIT0, &P1OUT  ; LED apagado
;  P4.7 - LED2 VERDE
	BIS.B   #BIT7, &P4DIR  ; P4.7 = saida
	BIC.B   #BIT7, &P4OUT  ; LED apagado

SW_LEDS:
	BIT.B   #BIT1, &P2IN  ; TESTA S1
	JZ      LED1     ; SE S1 ESTIVER ACIONADO
	BIC.B   #BIT0, &P1OUT ;  APAGA LED VERMELHO

SW_LEDS1:
	BIT.B   #BIT1, &P1IN  ; TESTA S2
	JZ      LED2     ; SE S2 ESTIVER ACIONADO
	BIC.B   #BIT7, &P4OUT ;  APAGA LED VERDE
	JMP    SW_LEDS

LED1:
	BIS.B   #BIT0, &P1OUT ; ACENDE LED VERMELHO
	JMP    SW_LEDS1

LED2:
	BIS.B   #BIT7, &P4OUT ; ACENDE LED VERDE
	JMP    SW_LEDS


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

