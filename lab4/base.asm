#INCLUDE <p18f4520.inc>
    CONFIG OSC = INTIO67
    CONFIG WDT = OFF 
    org 0x00 ; PC = 0x00

movlf macro literal, F
    movlw literal
    movwf F
    endm
    
Sub_Mul macro xh, xl, yh, yl
    ; x - y = [0x00, 0x01]
    movf yh, w
    subwf xh, w
    movwf 0x00
    clrf WREG
    movf yl, w
    subwf xl, w
    btfss STATUS, 0
	decf 0x00
    movwf 0x01
    ; 0x00 * 0x01
    clrf WREG
    movf 0x00, w
    mulwf 0x01
    movf PRODH, w
    movwf 0x10
    movf PRODL, w
    movwf 0x11
    endm

movlf 0x02, 0x20
movlf 0x0c, 0x21
movlf 0x00, 0x22
movlf 0x0f, 0x23
Sub_Mul 0x20, 0x21, 0x22, 0x23
NOP
end