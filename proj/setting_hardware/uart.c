#include <xc.h>
    //setting TX/RX
long long int constant_pwm = 2000;
float curr_duty_per = 0.2;
float curr_duty_per_2 = 0.2;
char mystring[20];
int lenStr = 0;
 void XL_Motor_Control(char direction) {
    // ??????????
    if (direction == 'W') { // Forward
        LATAbits.LATA0 = 1;   // IN1 = H
        LATAbits.LATA1 = 0;   // IN2 = L
        curr_duty_per = 0.2;         // ??? 100%
    } else if (direction == 'S') { // Backward
        LATAbits.LATA0 = 0;   // IN1 = L
        LATAbits.LATA1 = 1;   // IN2 = H
        curr_duty_per = 0.2;         // ??? 100%
    } else if (direction == 'A') {
        LATAbits.LATA2 = 1;   // IN1 = H
        LATAbits.LATA3 = 0;   // IN2 = L
        curr_duty_per = 0.2;         // ??? 100%
    } else if (direction == 'D') {
        LATAbits.LATA2 = 0;   // IN1 = L
        LATAbits.LATA3 = 1;   // IN2 = H
    } else { // Stop
        LATAbits.LATA0 = 0;
        LATAbits.LATA1 = 0;
        LATAbits.LATA2 = 0;   // IN1 = L
        LATAbits.LATA3 = 0;   // IN2 = L
        curr_duty_per = 0;           // ??? 0%
    }
}
 
void UART_Initialize() {
           
    /*       TODObasic   
           Serial Setting      
        1.   Setting Baud rate
        2.   choose sync/async mode 
        3.   enable Serial port (configures RX/DT and TX/CK pins as serial port pins)
        3.5  enable Tx, Rx Interrupt(optional)
        4.   Enable Tx & RX
    */
           
    TRISCbits.TRISC6 = 1;            
    TRISCbits.TRISC7 = 1;            
    
    //  Setting baud rate //for 1MHz //1200 baud rate
    TXSTAbits.SYNC = 0;           
    BAUDCONbits.BRG16 = 1;          
    TXSTAbits.BRGH = 0;
    SPBRG = 25;      
    
   //   Serial enable
    RCSTAbits.SPEN = 1;              
    PIR1bits.TXIF = 0;
    PIR1bits.RCIF = 1;
    TXSTAbits.TXEN = 1;           
    RCSTAbits.CREN = 1;             
    PIE1bits.TXIE = 0;       
    IPR1bits.TXIP = 0;             
    PIE1bits.RCIE = 1;              
    IPR1bits.RCIP = 0;    
}

void UART_Write(unsigned char data)  // Output on Terminal
{
    while(!TXSTAbits.TRMT);
    TXREG = data;              //write to TXREG will send data 
}


void UART_Write_Text(char* text) { // Output on Terminal, limit:10 chars
    for(int i=0;text[i]!='\0';i++)
        UART_Write(text[i]);
}

void ClearBuffer(){
    for(int i = 0; i < 10 ; i++)
        mystring[i] = '\0';
    lenStr = 0;
}

void MyusartRead()
{
    /* TODObasic: try to use UART_Write to finish this function */
    char received = RCREG;
    XL_Motor_Control(received);
    UART_Write(received);
    if(received == '\n' || received == '\r' || lenStr>=19) {
        ClearBuffer();
    }
    else {
        mystring[lenStr++] = received;
        mystring[lenStr] = '\0';
    }
    
}

char *GetString(){
    return mystring;
}

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
        CCPR1H = (curr>>4);
        CCPR1L = curr & 0b1111;
        TMR1H = 0;
        TMR1L = 0;
        CCP1IF = 0;
    }
    if(CCP2IF) {
        T3CONbits.TMR3ON = 0;
        long long int curr = (CCPR2H << 4) | CCPR1L;
        if(curr == 65535 - 2000*curr_duty_per_2) {
            curr = 65535 - 2000*(1-curr_duty_per_2);
        }
        else {
            curr = 65535 - 2000*curr_duty_per_2;
        }
        CCPR2H = (curr>>4);
        CCPR2L = curr & 0b1111;
        TMR3H = 0;
        TMR3L = 0;
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
