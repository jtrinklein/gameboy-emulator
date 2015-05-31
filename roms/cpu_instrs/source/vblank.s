.include "shell.inc"

main:
    call load_stat_tiles
    ld hl,$9800
    xor a
    ld (hl),a


wait_last_line:
    lda  LY          ; read scanline
    cp  $8F               ; wait until rendering last scanline
    jp  nz,wait_last_line
    ld d,$12
    ei                    ; master interrupt enable
    wreg IE,$01           ; enable vblank int

wait_next_line:
    lda $FF44
    cp $90                  ; wait for next scanline, triggers vblank
    jp nz,wait_next_line
    ld a,d
    ld hl,$9800
    ld (hl),a


load_stat_tiles:
    ld b,>status_data
    ld c,<status_data
    ld hl,$8110
    ld a,$02
    jp btn_loop

btn_loop:
    call load_btn
    dec a
    jr nz,btn_loop
    ret

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
status_data:
    .byte $ff,$ff,$c3,$c3,$c3,$c3,$ff,$ff
    .byte $c3,$66,$3c,$18,$18,$3c,$66,$c3

.org $40
    ld d,$11
    ret
