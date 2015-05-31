.include "shell.inc"


joy_check:
    push bc
    wreg P1,$10
    lda P1
    lda P1
    lda P1
    ld b,a
    wreg P1,$20
    lda P1
    lda P1
    lda P1
    ld c,a
w_:
    lda LY
    cp 144
    jr nz,w_
    nop
    ld hl,$9800
    ld a,b
    ldi (hl),a
    ld a,c
    ldi (hl),a
    pop bc
    ret


main:
    lda LY
    cp 144
    jr nz,main
    call load_btn_tiles
    call load_dpad_tiles
    ld hl,BGMAP0
    xor a
    ld bc,$0400
lp_:
    ldi (hl),a
    dec bc
    cp c
    jp nz,lp_
    cp b
    jp nz,lp_
    wreg IE,$10
    ei
    wreg SCY,$00

-   nop
    call joy_check
    jr -

load_btn_tiles:
    ld b,>btn_data
    ld c,<btn_data
    ld hl,$8110
    ld a,$0F
btn_loop:
    call load_btn
    dec a
    jr nz,btn_loop
    ret

load_dpad_tiles:
    ld b,>dpad_data
    ld c,<dpad_data
    ld hl,$8210
    ld a,$0F
    jp btn_loop

load_btn:
    push af
    ld d,$08
lp:
    ld a,(bc)
    ldi (hl),a
    ldi (hl),a
    inc bc
    dec d
    jp nz,lp
    pop af
    ret

btn_data:
    .byte $e0,$a0,$e0,$a0,$00,$00,$00,$00 ; a
    .byte $0c,$0a,$0c,$0a,$0c,$00,$00,$00 ; b
    .byte $ec,$aa,$ec,$aa,$0c,$00,$00,$00 ; ab
    .byte $00,$00,$00,$00,$00,$08,$08,$0e ; e
    .byte $e0,$a0,$e0,$a0,$00,$08,$08,$0e ; ae
    .byte $0c,$0a,$0c,$0a,$0c,$08,$08,$0e ; be
    .byte $ec,$aa,$ec,$aa,$0c,$08,$08,$0e ; abe
    .byte $00,$00,$00,$00,$00,$e0,$40,$40 ; t
    .byte $e0,$a0,$e0,$a0,$00,$e0,$40,$40 ; at
    .byte $0c,$0a,$0c,$0a,$0c,$e0,$40,$40 ; bt
    .byte $ec,$aa,$ec,$aa,$0c,$e0,$40,$40 ; abt
    .byte $00,$00,$00,$00,$00,$e8,$48,$4e ; et
    .byte $e0,$a0,$e0,$a0,$00,$e8,$48,$4e ; aet
    .byte $0c,$0a,$0c,$0a,$0c,$e8,$48,$4e ; bet
    .byte $ec,$aa,$ec,$aa,$0c,$e8,$48,$4e ; abet

dpad_data:
    .byte $02,$0f,$02,$00,$00,$00,$00,$00 ; r
    .byte $40,$f0,$40,$00,$00,$00,$00,$00 ; l
    .byte $42,$ff,$42,$00,$00,$00,$00,$00 ; rl
    .byte $00,$00,$00,$00,$20,$70,$20,$20 ; u
    .byte $02,$0f,$02,$00,$20,$70,$20,$20 ; ru
    .byte $40,$f0,$40,$00,$20,$70,$20,$20 ; lu
    .byte $42,$ff,$42,$00,$20,$70,$20,$20 ; rlu
    .byte $00,$00,$00,$00,$04,$04,$0e,$04 ; d
    .byte $02,$0f,$02,$00,$04,$04,$0e,$04 ; rd
    .byte $40,$f0,$40,$00,$04,$04,$0e,$04 ; ld
    .byte $42,$ff,$42,$00,$04,$04,$0e,$04 ; rld
    .byte $00,$00,$00,$00,$24,$74,$2e,$24 ; ud
    .byte $02,$0f,$02,$00,$24,$74,$2e,$24 ; rud
    .byte $40,$f0,$40,$00,$24,$74,$2e,$24 ; lud
    .byte $42,$ff,$42,$00,$24,$74,$2e,$24 ; rlud

.org $60
    ld a,1
    reti
