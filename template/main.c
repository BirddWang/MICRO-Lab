#include "setting_hardaware/setting.h"
#include <stdlib.h>
#include "stdio.h"
#include "string.h"
#define _XTAL_FREQ 4000000
// using namespace std;

char str[20];
char curr;
int mode = 0;
void displayBinary(int num) {
    // Assuming TRISD0-TRISD3 are connected to your LEDs (active low)
    LATDbits.LATD0 = (num & 0x01) ? 1 : 0; // Least significant bit
    LATDbits.LATD1 = (num & 0x02) ? 1 : 0;
    LATDbits.LATD2 = (num & 0x04) ? 1 : 0;
    LATDbits.LATD3 = (num & 0x08) ? 1 : 0; // Most significant bit
}
void Mode1(){   // Todo : Mode1 
    UART_Write_Text("------MODE 1 NOW-----\n");
    char *inputStr;
    int num;
    while(1) {
        UART_Write_Text("Enter 0~9: ");
        inputStr = GetString();
        num = inputStr[0] - '0';
        
        if(num >= 0 && num<=9) {
            displayBinary(num);
        }
    }
    
    return ;
}
void Mode2(){   // Todo : Mode2 
    mode = 2;
    UART_Write_Text("IN MODE 2\r\n");
    LATD = 0;
    TRISD = 0x00;
    char message[20];
    while(1) {
        strcpy(message, GetString());
        if(strcmp(message, "exit") == 0) {
            UART_Write_Text("EXIT MODE 2\r\n");
            break;
        }
    }
    return ;
}
void main(void) 
{
    
    SYSTEM_Initialize() ;
    
    while(1) {
        strcpy(str, GetString()); // TODO : GetString() in uart.c
        if(str[0]=='m' && str[1]=='1'){ // Mode1
            Mode1();
            ClearBuffer();
        }
        else if(str[0]=='m' && str[1]=='2'){ // Mode2
            Mode2();
            ClearBuffer();  
        }
    }
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
        //READ
        MyusartRead();
        //Update
        if(mode == 2) {
            int num;
            char m2_message[20];
            strcpy(m2_message, GetString());
            num = m2_message[0] - '0';
            UART_Write(num + '0');
            if(num >= 0 && num <=9) {
                UART_Write_Text("\r\nIS NUMBER\r\n");
                curr = num;
                TMR1_START(3035); //65535 - (1*500000 / 8) = 3035;
            }
        }
        RCIF = 0;
    }
    
    // TMR1
    if(TMR1IF) {
        if(LATD < curr) LATD +=1;
        else LATD = 0;
        __delay_ms(50);
        TMR1IF = 0;
    }
    // TMR2
    if(TMR2IF) {
        TMR2IF = 0;
    }
    return;
}