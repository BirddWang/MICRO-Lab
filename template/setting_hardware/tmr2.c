#include <xc.h>
#include <pic18f4520.h> 

// TMR2
// 8-bits | prescale: 01, 04, 16 | selected val
// 4 Clocks --> 1 Timer
void TMR2_Initialize(void) {
    PIE1bits.TMR2IE = 1;
    PIR1bits.TMR2IF = 0;
    IPR1bits.TMR2IP = 0;
    
    //Prescaler
    T2CONbits.T2CKPS = 0b01; //1:4
    // 00[1:1] | 01[1:4] | 1x[1:16];
    
    //Postscaler
    T2CONbits.T2OUTPS = 0b1111; //1:16
    // 1:n (n = 0bxxxx) 01~16
    
    TMR2 = 0x00;
    PR2 = 0xff; //count number
}

void TMR2_Start(unsigned char t) {
    TMR2 = 0x00;
    PR2 = t;
    T2CONbits.TMR2ON = 1;
}