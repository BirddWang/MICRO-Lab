#include <xc.h>
#include <pic18f4520.h>

void CCP2_Compare_Initialize() {
    TRISCbits.TRISC1=0;	// RC2 pin is output.
    CCP2CONbits.CCP2M = 0b0010;	// Compare mode, initialize CCP1 pin high, clear output on compare match
    PIE2bits.CCP2IE = 1;
    PIR2bits.CCP2IF=0;
    IPR2bits.CCP2IP = 1;
}