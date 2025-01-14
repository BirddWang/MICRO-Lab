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
float curr_duty_per_2 = 0.2;
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
