#INCLUDE <p18f4520.inc>
    CONFIG OSC = INTIO67
    CONFIG WDT = OFF 
    org 0x00 ; PC = 0x00
movlf macro literal, F
    movlw literal
    movwf F
    endm
init:
    movlf 0x09, 0x03 ; n
main:
    movlf 0x00, 0x10 ; 0
    movlf 0x01, 0x11 ; 1
    LFSR 0, 0x10; pt0
    LFSR 1, 0x20; pt1
   rcall fib
   rcall finish
fib:
    movff POSTINC0, WREG
    addwf POSTINC0, w
    movwf POSTDEC0
    movff POSTDEC0, WREG
    movff WREG, POSTINC0
    
    movff PREINC0, WREG
    movff POSTDEC0, WREG
    movff WREG, POSTDEC0 ; NEED TEST
    
    movff POSTINC1, WREG
    addwfc POSTINC1, w
    movwf POSTDEC1
    movff POSTDEC1, WREG
    movff WREG, POSTINC1
    
    movff PREINC1, WREG
    movff POSTDEC1, WREG
    movff WREG, POSTDEC1 ; NEED TEST
    
    decfsz 0x03
	rcall fib
    return
finish:
    movff POSTINC0, WREG
    movff WREG, 0x01
    movff POSTINC1, WREG
    movff WREG, 0x00
    nop
    end