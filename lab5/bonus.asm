#include "xc.inc"
GLOBAL _multi_signed
PSECT mytext,local,class=CODE,reloc=2

_multi_signed:
    movff WREG, 0x050 ; a 8bit -128-127
    movff 0x001, 0x051 ; b 4bit -8, 7
    ; check msb of b
    ; signed flag: 0x060
    clrf 0x060
check_a:
    btfsc 0x050, 7
	goto neg_a
    goto check_b
neg_a:
    incf 0x060
    negf 0x050
    goto check_b

check_b:
    btfsc 0x051, 7
	goto neg_b
    goto multiply
neg_b:
    incf 0x060
    negf 0x051
    
multiply:
    rcall mul_main
    btfsc 0x060, 0
	goto negation
    goto result

mul_main:
    movlw 0x08
    movwf 0x61 ; time
    ; 0x050 * 0x051 = PRODH:PRODL
mul_loop:
    btfsc 0x50, 7
	rcall add_
    decfsz 0x61
	goto rotate
    return
rotate:
    bcf STATUS, 0
    rlcf PRODL
    rlcf PRODH
    bcf STATUS, 0
    rlcf 0x50
    goto mul_loop
    
add_:
    movff 0x051, WREG
    addwf PRODL, f
    btfsc STATUS, 0
	incf PRODH
    return
    
negation:
    clrf WREG
    cpfseq PRODH
	goto neg_high
    goto neg_low
neg_high:
    negf PRODH
    decf PRODH
neg_low:
    negf PRODL
    goto result
    
result:
    movff PRODL, 0x001
    movff PRODH, 0x002
    return