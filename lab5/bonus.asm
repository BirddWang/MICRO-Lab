#include "xc.inc"
GLOBAL _multi_signed
PSECT mytext,local,class=CODE,reloc=2

_multi_signed:
    movff WREG, 0x050 ; a
    movff 0x001, 0x051 ; b
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
    movff 0x050, WREG
    mulwf 0x051
    btfsc 0x060, 0
	goto negation
    goto result

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