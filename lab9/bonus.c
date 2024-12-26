#include <xc.h>
#include <pic18f4520.h>

#pragma config OSC = INTIO67    // Internal oscillator, pins 6 & 7 as I/O
#pragma config WDT = OFF       // Watchdog timer disabled
#pragma config PWRT = OFF       // Power-up timer disabled
#pragma config BOREN = ON       // Brown-out reset enabled
#pragma config PBADEN = OFF     // PORTB digital
#pragma config LVP = OFF        // Low-voltage programming disabled
#pragma config CPD = OFF        // Data EEPROM code protection disabled

#define _XTAL_FREQ 1000000  // 1 MHz internal oscillator

void __interrupt(high_priority) H_ISR(void);

void init_pwm(void);
void init_adc(void);
void init_interrupt(void);
unsigned int read_adc(void);
void set_pwm_duty(unsigned int duty);


void main(void) {
    OSCCONbits.IRCF = 0b100; // 1MHz internal oscillator
    TRISBbits.TRISB0 = 1;     // RB0 as input (for potentiometer/variable resistor)

    init_adc();
    init_pwm();
    init_interrupt();

    unsigned int adc_value;


    while (1) {
        ADCON0bits.GO = 1; // Start ADC Conversion
        while(ADCON0bits.GO); // Wait until ADC is done
        adc_value = read_adc(); 
        
        // Map ADC value (0-1023) to a suitable duty cycle range
        // For brightest at center:
        // ADC: 0 (left) -> Dim, ~512 (center) -> Bright, ~1023 (right) -> Dim
        
       
        unsigned int mapped_duty;

        if (adc_value <= 512){
             mapped_duty =  (1023 - (adc_value << 1));
        }else{
            mapped_duty = (adc_value << 1) - 1023;
        }

        set_pwm_duty(mapped_duty);
        __delay_ms(10); // Small delay to limit update rate (optional)
    }
}

void __interrupt(high_priority) H_ISR(void) {
    if(PIR1bits.ADIF){
        PIR1bits.ADIF = 0;
    }
}

void init_pwm(void) {
    T2CONbits.TMR2ON = 1;    // Timer 2 ON
    T2CONbits.T2CKPS = 0b01;  // Prescaler 4 (1MHz/4)
    PR2 = 0xff;              // Timer 2 period
    CCP1CONbits.CCP1M = 0b1100; // PWM mode, P1A is PWM output, active high
    TRISCbits.TRISC2 = 0;       // CCP1 pin (RC2) as output

    //Initial Duty 0%
    CCPR1L = 0x00;
    CCP1CONbits.DC1B = 0;
}


void init_adc(void) {
    ADCON1bits.VCFG0 = 0;   // VREF+ = VDD
    ADCON1bits.VCFG1 = 0;   // VREF- = VSS
    ADCON1bits.PCFG = 0b1110; // AN0 as Analog input, all others as digital
    ADCON2bits.ADCS = 0b000;  // ADC Clock FOSC/2 (1MHz) 
    ADCON2bits.ACQT = 0b001;  // Acquisition time 2 * TAD
    ADCON0bits.ADON = 1;     // Turn ON ADC
    ADCON2bits.ADFM = 0;      // Left justified

    ADCON0bits.CHS = 0b0000;   // Select channel AN0
}

void init_interrupt(void) {
    PIE1bits.ADIE = 1;   // Enable ADC interrupt
    PIR1bits.ADIF = 0;   // Clear ADC interrupt flag
    INTCONbits.PEIE = 1; // Enable peripheral interrupts
    INTCONbits.GIE = 1;  // Enable global interrupts
}

unsigned int read_adc(void) {
    // Read result from ADRESH:ADRESL (left justified)
    return ((unsigned int)ADRESH << 2) | ((unsigned int)ADRESL >> 6);
}

void set_pwm_duty(unsigned int duty) {
    duty = 1023 - duty;
    
    if(duty < 100) {
        CCPR1L = 0;
        CCP1CONbits.DC1B =0;
    }
    else {
        CCPR1L = duty >> 2;
        CCP1CONbits.DC1B = duty & 0x03;
    }
}