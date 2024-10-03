#INCLUDE <p18f4520.inc>
    CONFIG OSC = INTIO67
    CONFIG WDT = OFF 
    org 0x00 ; PC = 0x00

    movlw 0x2a
    movwf 0x00
    movlw 0x41
    movwf 0x01
    
    movlw 0x11
    movwf 0x10 ; count
    movlw 0x00
    movwf 0x11 ; result
    
find_MSB:
    dcfsnz 0x10
	goto done
    ; find MSB
    clrf STATUS, 0
    rlcf 0x01, f
    rlcf 0x00, f
    btfss STATUS, 0
	goto find_MSB
    movff 0x10, 0x11
    goto find_if_carry
   
find_if_carry:
    dcfsnz 0x10
	goto done
    clrf STATUS, 0
    rlcf 0x01, f
    rlcf 0x00, f
    btfss STATUS, 0
	goto find_if_carry
    incf 0x11
done:
end