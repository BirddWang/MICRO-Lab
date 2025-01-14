#include <xc.h>
#include <pic18f4520.h> 

// 16-bits | prescale: 01~08 | non-selected(using overflow method)
// 4 Clocks --> 1 Timer
void TMR3_Initialize(void) {
    PIE2bits.TMR3IE = 1; //Enable
    PIR2bits.TMR3IF = 0; //IF Flag
    IPR2bits.TMR3IP = 0; //Priority
    TMR3H = 0; //init
    TMR3L = 0; //init
    
    // Prescalar:
    T3CONbits.T3CKPS = 0b01;
    // 0b11: 1:8 | 0b10: 1:4
    // 0b01: 1:2 | 0b00: 1:1
    
}

void TMR3_START(unsigned int T) {
    TMR1H = T / 256; //init
    TMR1L = T & 255; //init
    T3CONbits.TMR3ON = 1;
}