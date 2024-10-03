#INCLUDE <p18f4520.inc>
    CONFIG OSC = INTIO67
    CONFIG WDT = OFF 
    org 0x00 ; PC = 0x00

    movlw 0xc8
    movwf TRISA
    clrf WREG
    
    movf TRISA, W
    bcf STATUS, C
    rlcf WREG, W
    movwf TRISA
    
    bcf STATUS, C
    btfsc TRISA, 7
	bsf STATUS, C
    rrcf TRISA, F
    btfsc STATUS, C
	bsf TRISA, 7 
    bcf STATUS, C
end