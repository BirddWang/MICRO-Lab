#include "setting_hardaware/setting.h"
#include <stdlib.h>
#include "stdio.h"
#include "string.h"
#define _XTAL_FREQ 4000000
// using namespace std;

char str[20];
void main(void) 
{    
    SYSTEM_Initialize() ;

    return;
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
    if(TMR1IF) {
        TMR1IF = 0;
    }
    // TMR2
    if(TMR2IF) {
        TMR2IF = 0;
    }
    return;
}