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
    T2CONbits.T2CKPS = 0b11; //1:4
    // 00[1:1] | 01[1:4] | 1x[1:16];
    
    //Postscaler
    T2CONbits.T2OUTPS = 0b1111; //1:16
    // 1:n (n = 0bxxxx) 01~16
    
    // SG-90 needs 50Hz:20ms
    // 125000Hz -> 8us
    // 8us * 4 * 4 * (PR2+1) = 20ms
    // PR2 = 20000/128-1 = 155.25
    TMR2 = 0x00;
    PR2 = 155; //count number
}

void TMR2_Start(void) {
    TMR2 = 0x00;
    T2CONbits.TMR2ON = 1;
}