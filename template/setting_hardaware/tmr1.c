#include <xc.h>
#include <pic18f4520.h> 

// 16-bits | prescale: 01~08 | non-selected(using overflow method)
// 4 Clocks --> 1 Timer
void TMR1_Initialize(void) {
    PIE1bits.TMR1IE = 1; //Enable
    PIR1bits.TMR1IF = 0; //IF Flag
    IPR1bits.TMR1IP = 0; //Priority
    TMR1H = 0; //init
    TMR1L = 0; //init
    
    // Prescalar:
    T1CONbits.T1CKPS = 0b11;
    // 0b11: 1:8 | 0b10: 1:4
    // 0b01: 1:2 | 0b00: 1:1
    
}

void TMR1_START(unsigned char T) {
    TMR1H = T / 256; //init
    TMR1L = T & 255; //init
    PIR1bits.TMR1IF = 0;
    T1CONbits.TMR1ON = 1;
}