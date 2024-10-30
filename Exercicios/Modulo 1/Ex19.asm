
; R12  um número sem sinal de 16 bits
; R13  o endereço do vetor de saída

main:
push R12
push R13

W16_ASC:
MOV #NUM, R12
MOV @R12+, R5
CMP R15, Ox41
JHS LETRA
JLO NUMERO
JMP $

LETRA:
ADD 0x30, R5
CALL W16_ASC

NUMERO: 
ADD 0x37, R5
CALL W16_ASC






.data
num: .word 0x89AB
