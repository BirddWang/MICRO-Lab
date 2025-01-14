#include <xc.h>
#include <pic18f4520.h>

void CCP1_Compare_Initialize() {
    TRISCbits.TRISC2=0;	// RC2 pin is output.
    CCP1CONbits.CCP1M = 0b0010;	// Compare mode, initialize CCP1 pin high, clear output on compare match
    PIE1bits.CCP1IE = 1;
    PIR1bits.CCP1IF=0;
    IPR1bits.CCP1IP = 1;
}

void CCP1_PWM_Initialize() {
    TRISCbits.TRISC2 = 0;
    PIR1bits.CCP1IF = 0;
    IPR1bits.CCP1IP = 1;
    CCP1CONbits.CCP1M = 0b1100;
    CCPR1L = 0;
    PR2 = 155;
}

void CCP1_PWM_Set(unsigned int duty) {
    CCPR1L = duty>>2;
    CCP1CONbits.DC1B = duty&2;
    if(duty < 10) {
        CCPR1L = 0;
        CCP1CONbits.DC1B = 0;
    }
}