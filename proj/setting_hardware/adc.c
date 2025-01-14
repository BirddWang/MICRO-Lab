#include <xc.h>
#include <pic18f4520.h>

// 25us > T_{AD} > 0.7us
// 4MHz --> 1clock/0.25us --> TAD = 1us --> TACQ = 4us
void ADC_Initialize(void) {
    TRISA = 0xff;		// Set as input port
    ADCON1 = 0x0e;  	// Ref vtg is VDD & Configure pin as analog pin

    //ADCON2
    ADFM = 1 ;          // Right Justified
    ADCON2bits.ADCS = 0b100;       // A/D Conversion Clock Select Bits
    // 000[/2] | 001[/8] | 010[/32]
    // 100[/4] | 101[/16] | 110[/64]
    // 111[FRC] | 011[FRC] (1.2us)
    ADCON2bits.ACQT = 0b010;       // ADC Acquisition Time Select Bits
    // 111[20] | 110[16] | 101[12]
    // 100[08] | 011[06] | 010[04]
    // 001[02] | 000[00]
    
    ADRESH=0;  			// Flush ADC output Register
    ADRESL=0;  
}

int ADC_Read(int channel)
{
    int digital;
    
    ADCON0bits.CHS = channel; //Select Channel 0
    ADCON0bits.GO = 1;
    ADCON0bits.ADON = 1;
    
    while(ADCON0bits.GO_nDONE==1);

    digital = (ADRESH*256) | (ADRESL);
    return(digital);
}