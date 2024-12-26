#include <xc.h>
#include <pic18f4520.h>

#pragma config OSC = INTIO67    
#pragma config WDT = OFF       
#pragma config PWRT = OFF       
#pragma config BOREN = ON       
#pragma config PBADEN = OFF     
#pragma config LVP = OFF       
#pragma config CPD = OFF        

#define _XTAL_FREQ 125000

// +90 2400
// x = 75
// CCPR1L = 0x12
// DC1B = 0x03
// -90 500
// x = 15
// CCPR1L = 0x03
// DC1B = 0x11
#define SERVO_MIN 15
#define SERVO_MID 45         
#define SERVO_MAX 75

int angle = -90;        
int direction = 1;              

void __init_pwm__() {
    T2CONbits.TMR2ON = 1;
    T2CONbits.T2CKPS = 0b01; //prescalar = 4
    PR2 = 0xff; //max
    CCP1CONbits.CCP1M = 0b1100; //PWM, high, high
    TRISC = 0x00;
    //-90
    CCPR1L = 0x03;
    CCP1CONbits.DC1B = 0x11;
}


void __update__(int angle) {
    int pwm_value = SERVO_MID + (angle * (SERVO_MAX - SERVO_MIN) / 180);
    CCPR1L = pwm_value >> 2;
    CCP1CONbits.DC1B = pwm_value & 0x03;
}

int _button_pressed() {
    if (PORTBbits.RB0 == 0) {
        __delay_ms(50); 
        while (PORTBbits.RB0 == 0);
        return 1;              
    }
    return 0;
}


void main(void) {
    OSCCONbits.IRCF = 0b001; //250kHz
    TRISBbits.TRISB0 = 1; //Input

    __init_pwm__();

    while (1) {
        if (_button_pressed()) {
            angle += 90 * direction;

            if (angle >= 90) {
                direction = -1;
            } else if (angle <= -90) {
                direction = 1;
            }

            __update__(angle);
        }
    }
}