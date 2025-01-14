#include "./setting_hardware/setting.h"
#include <stdlib.h>
#include <pic18f4520.h>
#include "stdio.h"
#include "string.h"
// setting Hz for __delay
#define _XTAL_FREQ 4000000
// using namespace std;

char str[20];
long long int constant_pwm = 2000;
float curr_duty_per = 0.2;
void main(void) 
{    
    SYSTEM_Initialize() ;
    T3CONbits.T3CCP1 = 1; // T1 for CCP1
    T3CONbits.T3CCP2 = 0; // T3 for CCP2
    T3CONbits.TMR3ON = 1;
    while(1);
}

void __interrupt(high_priority) Hi_ISR(void)
{
}

// void interrupt low_priority Lo_ISR(void)
void __interrupt(low_priority)  Lo_ISR(void)
{
    //Uart
    if(RCIF)
    {
        if(RCSTAbits.OERR)
        {
            CREN = 0;
            Nop();
            CREN = 1;
        }
        MyusartRead();
        
        RCIF = 0;
    }
    
    // TMR1
    if(CCP1IF) {
        // 2000 --> 65535-2000*k
        T1CONbits.TMR1ON = 0;
        long long int curr = (CCPR1H << 4) | CCPR1L;
        if(curr == 65535 - 2000*curr_duty_per) {
            curr = 65535 - 2000*(1-curr_duty_per);
        }
        else {
            curr = 65535 - 2000*curr_duty_per;
        }
        TMR1H = (curr>>4);
        TMR1L = curr & 0b1111;
        CCP1IF = 0;
    }
    if(CCP2IF) {
        T3CONbits.TMR3ON = 0;
        long long int curr = (CCPR2H << 4) | CCPR1L;
        if(curr == 65535 - 2000*curr_duty_per) {
            curr = 65535 - 2000*(1-curr_duty_per);
        }
        else {
            curr = 65535 - 2000*curr_duty_per;
        }
        TMR3H = (curr>>4);
        TMR3L = curr & 0b1111;
        CCP2IF = 0;
    }
    if(TMR1IF) {
        TMR1IF = 0;
    }
    // TMR2
    if(TMR2IF) {
        TMR2IF = 0;
    }
    // TMR3
    if(TMR3IF) {
        TMR3IF = 0;
    }
    return;
}
