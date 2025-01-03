#include "p18f4520.inc"

; CONFIG1H
  CONFIG  OSC = INTIO67         ; Oscillator Selection bits (Internal oscillator block, port function on RA6 and RA7)
  CONFIG  FCMEN = OFF           ; Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
  CONFIG  IESO = OFF            ; Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

; CONFIG2L
  CONFIG  PWRT = OFF            ; Power-up Timer Enable bit (PWRT disabled)
  CONFIG  BOREN = SBORDIS       ; Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
  CONFIG  BORV = 3              ; Brown Out Reset Voltage bits (Minimum setting)

; CONFIG2H
  CONFIG  WDT = OFF             ; Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
  CONFIG  WDTPS = 32768         ; Watchdog Timer Postscale Select bits (1:32768)

; CONFIG3H
  CONFIG  CCP2MX = PORTC        ; CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
  CONFIG  PBADEN = ON           ; PORTB A/D Enable bit (PORTB<4:0> pins are configured as analog input channels on Reset)
  CONFIG  LPT1OSC = OFF         ; Low-Power Timer1 Oscillator Enable bit (Timer1 configured for higher power operation)
  CONFIG  MCLRE = ON            ; MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

; CONFIG4L
  CONFIG  STVREN = ON           ; Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
  CONFIG  LVP = OFF             ; Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
  CONFIG  XINST = OFF           ; Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

; CONFIG5L
  CONFIG  CP0 = OFF             ; Code Protection bit (Block 0 (000800-001FFFh) not code-protected)
  CONFIG  CP1 = OFF             ; Code Protection bit (Block 1 (002000-003FFFh) not code-protected)
  CONFIG  CP2 = OFF             ; Code Protection bit (Block 2 (004000-005FFFh) not code-protected)
  CONFIG  CP3 = OFF             ; Code Protection bit (Block 3 (006000-007FFFh) not code-protected)

; CONFIG5H
  CONFIG  CPB = OFF             ; Boot Block Code Protection bit (Boot block (000000-0007FFh) not code-protected)
  CONFIG  CPD = OFF             ; Data EEPROM Code Protection bit (Data EEPROM not code-protected)

; CONFIG6L
  CONFIG  WRT0 = OFF            ; Write Protection bit (Block 0 (000800-001FFFh) not write-protected)
  CONFIG  WRT1 = OFF            ; Write Protection bit (Block 1 (002000-003FFFh) not write-protected)
  CONFIG  WRT2 = OFF            ; Write Protection bit (Block 2 (004000-005FFFh) not write-protected)
  CONFIG  WRT3 = OFF            ; Write Protection bit (Block 3 (006000-007FFFh) not write-protected)

; CONFIG6H
  CONFIG  WRTC = OFF            ; Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
  CONFIG  WRTB = OFF            ; Boot Block Write Protection bit (Boot block (000000-0007FFh) not write-protected)
  CONFIG  WRTD = OFF            ; Data EEPROM Write Protection bit (Data EEPROM not write-protected)

; CONFIG7L
  CONFIG  EBTR0 = OFF           ; Table Read Protection bit (Block 0 (000800-001FFFh) not protected from table reads executed in other blocks)
  CONFIG  EBTR1 = OFF           ; Table Read Protection bit (Block 1 (002000-003FFFh) not protected from table reads executed in other blocks)
  CONFIG  EBTR2 = OFF           ; Table Read Protection bit (Block 2 (004000-005FFFh) not protected from table reads executed in other blocks)
  CONFIG  EBTR3 = OFF           ; Table Read Protection bit (Block 3 (006000-007FFFh) not protected from table reads executed in other blocks)

; CONFIG7H
  CONFIG  EBTRB = OFF           ; Boot Block Table Read Protection bit (Boot block (000000-0007FFh) not protected from table reads executed in other blocks)

org 0x00
goto INiT
ISR:
    org 0x08
    btfss PIR1, TMR2IF
	bra change_state
    bra check_1
change_state:
    rcall toggle_time
    incf 0x10
    movlw 0x03
    cpfslt 0x10
	clrf 0x10
    
    clrf LATA
    movlw 0x02
    cpfseq 0x10
	bra isr_end
    rcall all_1_lata
    bra isr_end
    
check_1:
    movlw 0x00
    cpfseq 0x10
	bra check_2
    bra state_1
check_2:
    movlw 0x01
    cpfseq 0x10
	bra check_3
    bra state_2
check_3:
    movlw 0x02
    cpfseq 0x10
	bra isr_end
    bra state_3
state_1:
    incf LATA
    btfsc LATA, 3
	clrf LATA
    bra isr_end
state_2:
    incf LATA
    btfsc LATA, 4
	clrf LATA
    bra isr_end
state_3:
    decf LATA
    bra isr_end
isr_end:
    bcf INTCON, INT0F
    bcf PIR1, TMR2IF
    RETFIE
    
 
INiT:
    movlw 0x0f
    movwf ADCON1
    
    clrf TRISA
    clrf LATA
    
    bsf RCON, IPEN
    bsf INTCON, GIE
    bcf INTCON, INT0IF
    bsf INTCON, INT0IE
    
    bcf PIR1, TMR2IF ;flag
    bsf PIE1, TMR2IE ;enable
    bsf IPR1, TMR2IP ;priority
    
    movlw 0xff
    movwf T2CON ;PRE/POST SCALER == 16 --> 256C/per TIMER2
    movlw d'61'
    movwf PR2
    
    movlw b'00100000'
    movwf OSCCON
    
    clrf 0x10
    
main:
    bra main
    
toggle_time:
    movlw d'61'
    cpfseq PR2
	bra _tge_61
    bra _tge_122
_tge_122:
    movlw d'122'
    movwf PR2
    return
_tge_61:
    movlw d'61'
    movwf PR2
    return

all_1_lata:
    movlw 0x0f
    movwf LATA
    return
    
end