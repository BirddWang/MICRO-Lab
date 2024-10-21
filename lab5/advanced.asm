#include "xc.inc"
GLOBAL _gcd
PSECT mytext,local,class=CODE,reloc=2

_gcd:
    movff 0x001, 0x050 ;a_low
    movff 0x002, 0x051 ;a_high
    movff 0x003, 0x060 ;b_low
    movff 0x004, 0x061 ;b_high
    ; a always > b
    ; check if a<b
    ; a_high < b_high
check_high:
    movff 0x051, WREG
    subwf 0x061, w
    bz check_low
    btfsc STATUS, 0
	goto swap
    bnz minus_loop ;a_high > b_high
check_low:
    ; a_low < b_low (a_high == b_high)
    movff 0x050, WREG
    subwf 0x060, w
    bz endi
    btfsc STATUS, 0
	goto swap
minus_loop:
    movff 0x061, WREG
    subwf 0x051, f
    movff 0x060, WREG
    subwf 0x050, f
    
    btfss STATUS, 0
	decf 0x051
    goto check_high
    
endi:
    movff 0x060, 0x001
    movff 0x061, 0x002
    return
    
swap:
    ; tmp 0x040, 0x041
    movff 0x050, 0x040
    movff 0x060, 0x050
    movff 0x040, 0x060
    clrf 0x040
    movff 0x051, 0x041
    movff 0x061, 0x051
    movff 0x041, 0x061
    clrf 0x041
    goto minus_loop