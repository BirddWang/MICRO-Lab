#include <xc.h>
#include <pic18f4520.h>
#include <stdio.h>

#pragma config OSC = INTIO67 // Oscillator Selection bits
#pragma config WDT = OFF     // Watchdog Timer Enable bit
#pragma config PWRT = OFF    // Power-up Enable bit
#pragma config BOREN = ON    // Brown-out Reset Enable bit
#pragma config PBADEN = OFF  // Watchdog Timer Enable bit
#pragma config LVP = OFF     // Low Voltage (single -supply) In-Circute Serial Pragramming Enable bit
#pragma config CPD = OFF     // Data EEPROM?Memory Code Protection bit (Data EEPROM code protection off)
#define _XTAL_FREQ 1000000

unsigned char mapValueToOddEven(int adcVal);
unsigned char mapValueToID(int adcVal);

volatile unsigned char ledValue = 0;
const unsigned char ID[8] = {7, 4, 1, 1, 1, 1, 0, 2};
static int preVal = 0;

void __interrupt(high_priority)H_ISR(){
    
    if(PIR1bits.ADIF) {
        // 8bits --> 256
        int adcVal = ADRESH;
        
//        ledValue = mapValueToID(adcVal); //base
        ledValue = mapValueToOddEven(adcVal); //advanced
        LATD = ledValue;
        
        PIR1bits.ADIF = 0;
        __delay_us(50);
        ADCON0bits.GO = 1;
    }
    return;
}

void main(void) 
{
    //configure OSC and port
    OSCCONbits.IRCF = 0b100; //1MHz
    TRISAbits.RA0 = 1;       //analog input port
    
    //ANALOG
    ADCON1bits.VCFG0 = 0;
    ADCON1bits.VCFG1 = 0;
    ADCON1bits.PCFG = 0b1110; //AN0 為analog input,其他則是 digital
    ADCON0bits.CHS = 0b0000;  //AN0 當作 analog input
    ADCON2bits.ADCS = 0b000;  //查表後設000(1Mhz < 2.86Mhz)
    ADCON2bits.ACQT = 0b001;  //Tad = 2 us acquisition time設2Tad = 4 > 2.4
    ADCON0bits.ADON = 1;
    ADCON2bits.ADFM = 0;    //left justified 
    
    
    //Interrupt
    PIE1bits.ADIE = 1;
    PIR1bits.ADIF = 0;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
    
    TRISD= 0x00;
    LATD = 0x00;

    ADCON0bits.GO = 1;
    
    while(1);
    
    return;
}

unsigned char mapValueToID(int adcVal) {
    return ID[adcVal/32];
}

unsigned char mapValueToOddEven(int adcVal) {
    if(adcVal == preVal) return ledValue;
    __delay_ms(50);
    int isOdd = (adcVal > preVal);
    preVal = adcVal;
    if(isOdd) return (adcVal>>5) * 2 + 1;
    else return (adcVal>>5) * 2;
}