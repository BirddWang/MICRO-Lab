#include <xc.h>

void CCP1_Compare_Initialize() {
    TRISCbits.TRISC2=0;	// RC2 pin is output.
    CCP1CON=9;		// Compare mode, initialize CCP1 pin high, clear output on compare match
    PIR1bits.CCP1IF=0;
    IPR1bits.CCP1IP = 1;
}

void CCP1_PWM_Initialize() {
    TRISCbits.TRISC2 = 0;
    PIR1bits.CCP1IF = 0;
    IPR1bits.CCP1IP = 1;
    CCP1CONbits.CCP1M = 0b1100;
    
    // Period
    // 1MHz / 4 = 250kHz --> 4us
    // PR2 = 0xff --> 255, prescaler = 1:4
    // 500 ~ 2400(max)
    // 2500 / 4 / 4 = 156.25
    // duty = 155
    // 2400us = 2.4ms --> 1/2.4ms ~ 400Hz
    CCPR1L = 155;
    CCP1CONbits.P1M = 0b01;
    
}

void CCP1_PWM_Set(unsigned int duty) {
    if(duty < 10) {
        CCPR1L = 0;
        CCP1CONbits.P1M = 0;
    }
    else {
        CCPR1L = duty >> 2;
        CCP1CONbits.P1M = duty & 0b11;
    }
}