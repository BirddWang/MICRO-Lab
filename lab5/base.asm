#include "xc.inc"
GLOBAL _mysqrt
PSECT mytext,local,class=CODE,reloc=2
    
_mysqrt:
    movwf 0x60 ; n
    movwf 0x50 ;high = n
    movlw 0x00
    movwf 0x51
    ; mid: 0x52
loop:
    movff 0x50, WREG
    addwf 0x51, w
    rrcf WREG ; (h+l)/2
    bcf STATUS, 0
    movff WREG, 0x052 ; mid = wreg
    
    mulwf 0x52 ; mid**2
    ; PRODH:PRODL
    ; if(PRODH != 0) --> > else <
    
    clrf WREG
    cpfseq PRODH
	goto bigger
    
    
    movff PRODL, WREG
    subwf 0x60, w ; mid**2 - n
    
    btfss STATUS, 0
	goto bigger
    movff 0x052, 0x051 ; low = mid
    incf 0x51
    goto check_end
    
bigger:
    clrf PRODH
    movff 0x052, 0x050 ; high = mid
    
check_end:
    movff 0x050, WREG
    subwf 0x051, w
    bnz loop
    
    decf 0x50
    movff 0x050, WREG
    mulwf 0x050
    movff 0x060, WREG
    subwf PRODL, w
    btfss STATUS, 0
	incf 0x50
    movff 0x050, WREG
    return