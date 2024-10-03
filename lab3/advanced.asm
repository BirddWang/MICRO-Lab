#INCLUDE <p18f4520.inc>
    CONFIG OSC = INTIO67
    CONFIG WDT = OFF 
    org 0x00 ; PC = 0x00

    movlw 0x12
    movwf 0x00
    movlw 0xcb
    movwf 0x01
    
    movlw 0x09
    movwf 0x10
    movlw 0x35
    movwf 0x11
    clrf WREG

START:
    movf 0x001, w
    mulwf 0x011
    movff PRODH, 0x022
    movff PRODL, 0x023
    
    bcf STATUS, 0
    movf 0x000, w
    mulwf 0x011
    movff PRODH, 0x021
    movf PRODL, w
    addwf 0x022, f
    btfsc STATUS, 0
	incf 0x021, f

    bcf STATUS, 0
    movf 0x001, w
    mulwf 0x010
    movf PRODL, w
    addwf 0x022, f
    btfsc STATUS, 0
	incf 0x021, f
    bcf STATUS, 0
    movf PRODH, w
    addwf 0x021, f
    btfsc STATUS, 0
	incf 0x020, f
    
    bcf STATUS, 0
    movf 0x000, w
    mulwf 0x010
    movf PRODL, w
    addwf 0x021, f
    btfsc STATUS, 0
	incf 0x020, f
    bcf STATUS, 0
    movf PRODH, w
    addwf 0x020, f

end