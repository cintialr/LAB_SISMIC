#include <msp430.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "lcd.h"
#include "uart.h"

// Definições do Modulo RTC DS1302
#define RST  BIT2     // P3.2     RESET
#define CLK  BIT4     // P3.4     Serial Clock
#define DAT  BIT3     // P3.3     Data Input/Output

#define TERM_CLR "\033[2J\r"
#define TERM_HOME "\033[0;0H"

// Definições de alarme e hora
unsigned char hour, minute, second;
unsigned char alarm_hour = 0, alarm_min = 0, alarm_sec = 0;
unsigned char alarm_active = 0;

// Definições de data
unsigned char dayOfWeek, day, month, year; // Exemplo Sab, 15/02/25

volatile uint8_t index= 0;
volatile char rxBuffer[12];
volatile uint8_t lcdBlinkState = 0;

void RTC_Init();
void sendByte(uint8_t data);
void RTC_SetTime(unsigned char h, unsigned char m, unsigned char s);
void RTC_SetDay(unsigned char Day, unsigned char d, unsigned char m, unsigned char y);
unsigned char decToBCD(unsigned char val);
void RTC_ReadTime();
void displayDateTime();
void checkAlarm();
void setupTimer();
void processUART();
void blinkLCD();
char *getDayOfWeek(unsigned char day);

void main(void) {
    WDTCTL = WDTPW | WDTHOLD; // Parar o watchdog timer
    // Configuração do timer
    TA0CTL  = TASSEL__ACLK |                // Escolhe o ACLK como ref de tempo
              MC__UP       |                // Modo UP: Conta de 0 até CCR0
              TACLR;                        // Começa em 0

    TA0CCR0  = 32768/2 - 1;                 // Conta 0,5 segundo
    TA0CCTL0 = CCIE;                        // CCIE - Habilitador local da INTpa

    // configuração RTC

    P3DIR |= RST + CLK + DAT;
    P3OUT &= ~(RST + CLK + DAT);

    __enable_interrupt(); // Habilita interrupções globais
    i2cConfig();
    lcdInit();
    RTC_Init();
    uartOpen(1); // Inicializa a UART
    // Dia da semana {0:"Dom"; 1:"Seg"; 2:"Ter"; 3:"Qua"; 4:"Qui"; 5:"Sex"; 6:"Sab"}
    RTC_SetDay(3, 31, 12, 25);  //Dia da semana, Dia, mes, Ano
    RTC_SetTime(23, 59, 0);

    uartPrint("Digite hh:mm para configurar o alarme.\n\r");
//    uartPrint(TERM_HOME);

    while (1) {
        processUART();
        displayDateTime();
        if (alarm_active) {
            checkAlarm();
            blinkLCD();
        }
        // estava desligando o LCD
        __low_power_mode_1(); //Economiza energia sem desligar o LCD.
    }
}

void RTC_Init() {
    P3DIR |= RST + CLK; // RST e CLK como saída
    P3DIR &= ~DAT;      // DAT como entrada
    P3OUT &= ~(CLK + RST); // Começa com sinais em 0
}
void sendByte(uint8_t data) {
    int i;
    for (i = 0; i < 8; i++) {
        P3OUT &= ~CLK;  // Abaixa o clock
        if (data & 0x01)
            P3OUT |= DAT;  // Envia bit 1
        else
            P3OUT &= ~DAT;  // Envia bit 0
        P3OUT |= CLK;  // Sobe o clock
        data >>= 1;
    }
}
void RTC_SetTime(unsigned char h, unsigned char m, unsigned char s) {
    P3OUT |= RST;  // Ativa RTC
    sendByte(0x80);  // Comando de escrita para horas
    sendByte(decToBCD(h));  // Envia hora
    sendByte(decToBCD(m));  // Envia minuto
    sendByte(decToBCD(s));  // Envia segundo

    P3OUT &= ~RST;  // Desativa RTC
    hour = h;
    minute = m;
    second = s;
}
void RTC_SetDay(unsigned char Day, unsigned char d, unsigned char m, unsigned char y) {
    P3OUT |= RST;  // Ativa o RTC DS1302
    sendByte(0x86);  // Comando de escrita para o dia da semana
    sendByte(decToBCD(Day));  // Envia o dia da semana
    sendByte(decToBCD(d));  // Envia o dia do mês
    sendByte(decToBCD(m));  // Envia o mês
    sendByte(decToBCD(y));  // Envia o ano
    P3OUT &= ~RST;  // Desativa o RTC

    // Atualiza as variáveis globais (não afeta o RTC, apenas mantém o código consistente)
    dayOfWeek = Day;
    day = d;
    month = m;
    year = y;
}
unsigned char decToBCD(unsigned char val) {
    return ((val / 10) << 4) | (val % 10);
}
void RTC_ReadTime() {
        static const unsigned char daysInMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

        second = (second + 1) % 60;
        if (second == 0) {
            minute = (minute + 1) % 60;
            if (minute == 0) {
                hour = (hour + 1) % 24;
                if (hour == 0) { // Se chegou à meia-noite, mudar o dia
                    day++;

                    // Verificar se o ano é bissexto para ajustar fevereiro
                    unsigned char maxDays = daysInMonth[month - 1]; // O array começa do índice 0 (janeiro)
                    if (month == 2 && ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))) {
                        maxDays = 29; // Ano bissexto
                    }

                    if (day > maxDays) { // Avançar o mês
                        day = 1;
                        month++;
                        if (month > 12) { // Avançar o ano
                            month = 1;
                            year++;
                        }
                    }

                    dayOfWeek = (dayOfWeek + 1) % 7; // Avança o dia da semana
                }
            }
        }
}

void displayDateTime() {
    char linha1[20];
    char linha2[20];

    sprintf(linha1, "%s, %02u/%02u/%02u\n", getDayOfWeek(dayOfWeek), day, month, year);
    sprintf(linha2, "  %02u:%02u:%02u\n", hour, minute, second);

    lcdPrint((uint8_t *)linha1);
    lcdPrint((uint8_t *)linha2);
    __delay_cycles(1000000); // 1seg

}

void checkAlarm() {
    static unsigned int start_time = 0;  // Marca o tempo de ativação do alarme

    // Verifica se o alarme está ativo e se o horário corresponde
    if (alarm_active && hour == alarm_hour && minute == alarm_min) {
        if (start_time == 0) {  // O alarme acabou de ativar
            start_time = second;  // Salva o tempo inicial
        }

        if ((second - start_time) < 60) {  // Pisca por até 60 segundos
            blinkLCD();
        } else {  // Depois de 1 minuto, desliga o alarme
            alarm_active = 0;
            start_time = 0;  // Reseta o contador do alarme
        }

        // 🔹 Verifica se algum botão foi pressionado para desligar o alarme
        if (!(P1IN & BIT1) || !(P2IN & BIT1)) {  // Se P1.1 ou P2.1 forem pressionados
            alarm_active = 0;  // Desliga o alarme imediatamente
            start_time = 0;  // Reseta o contador
            uartPrint("Alarme desligado pelo botão.\n");
        }
    } else {
        start_time = 0;  // Reseta caso a hora não coincida mais
    }
}


void blinkLCD() {                   // faz o LCD piscar com o alarme
    static unsigned int counter = 0;
    if (++counter >= 50000) {
        counter = 0;
        lcdBlinkState = !lcdBlinkState;
        if (lcdBlinkState) {
            lcdWriteByte(0x08, 0); // Desliga o LCD
        } else {
            lcdWriteByte(0x0F, 0); // Liga o LCD
        }
    }
}

void processUART() {
    int h, m;
    char buffer[6];

    if (uartAvailable() > 0){  // Aguarda pelo menos 5 caracteres (hh:mm)
        uartRead(buffer);         // Lê os dados do UART
        buffer[5] = '\0';         // Garante terminação da string

        if (sscanf(buffer, "%02d:%02d", &h, &m) == 2) {  // Valida entrada
            if (h >= 0 && h < 24 && m >= 0 && m < 60) {
                alarm_hour = h;
                alarm_min = m;
                alarm_active = 1;  // Ativa o alarme
                uartPrint("Alarme definido para: ");
                uartPrint(buffer);
                uartPrint("\n");
            } else {
                uartPrint("Erro: Formato inválido!\n");
            }
        }
    }
}

char* getDayOfWeek(unsigned char day) {
    switch(day) {
        case 0: return "Dom";
        case 1: return "Seg";
        case 2: return "Ter";
        case 3: return "Qua";
        case 4: return "Qui";
        case 5: return "Sex";
        case 6: return "Sab";
        default: return "Day";
    }
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A_ISR(void) {
    RTC_ReadTime();
    displayDateTime();     // Atualiza o display com a hora e data
    checkAlarm();          // Verifica se o alarme deve tocar
}

