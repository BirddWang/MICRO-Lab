#INCLUDE <p18f4520.inc>
    CONFIG OSC = INTIO67
    CONFIG WDT = OFF 
    org 0x00 ; PC = 0x00

movlf macro literal, F
    movlw literal
    movwf F
    endm

movlf 0x04, 0x00 ; a2
movlf 0x05, 0x01 ; a3
movlf 0x03, 0x02 ; a1

movlf 0x07, 0x10 ; b2
movlf 0x08, 0x11 ; b3
movlf 0x06, 0x12 ; b1

main:
    movff 0x00, 0x03
    movff 0x10, 0x13
    LFSR 0, 0x000
    LFSR 1, 0x011
    LFSR 2, 0x020
    rcall cross
    goto finish
    
cross:
    ; tmp: 0x050, 0x051
    movff POSTINC0, WREG
    mulwf POSTDEC1
    movff PRODH, 0x050
    movff PRODL, 0x051
    clrf PRODH
    clrf PRODL
    movff INDF0, WREG
    mulwf POSTINC1
    clrf WREG
    addwf PRODL, w
    subwf 0x051
    btfss STATUS, 0
	decf 0x50
    movwf PRODH
    subwf 0x50
    movff 0x51, POSTINC2
    movff POSTINC1, WREG ; update
    clrf WREG
    cpfseq INDF1
	goto cross
    return
    
finish:
    end