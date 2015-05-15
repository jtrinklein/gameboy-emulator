#ifndef __OPCODE_H__
#define __OPCODE_H__

#define OP(x) case x:
#define END break;

OP(0x00) // noop
END

OP(0x76) // HALT
    runState = RS_HALT;
END

OP(0x10) // STOP
    runState = RS_STOP;
END

OP(0x27) // DDA
    I = AF.B.h;
    AF.B.h += (((AF.B.h&0x0F)>0x09)||(AF.B.l&FLAG_H)) ? 0x06 : 0;
    AF.B.h += (((AF.B.h&0xF0)>0x90)||(AF.B.l&FLAG_C)) ? 0x60 : 0;
    AF.B.l = (AF.B.l&FLAG_N) | (AF.B.h?0:FLAG_Z)| (I > AF.B.h?FLAG_C:0);
END

// LD (xx), A
OP(0x02) // LD (BC), A
    WRITE(BC.W, AF.B.h);
END

OP(0x12) // LD (DE), A
    WRITE(DE.W, AF.B.h);
END

OP(0x22) // LD (HL+), A
    WRITE(HL.W, AF.B.h);
    HL.W++;
END

OP(0x32) // LD (HL-), A
    WRITE(HL.W, AF.B.h);
    HL.W--;
END

// LD (HL), x
OP(0x70) // LD (HL), B
    WRITE(HL.W, BC.B.h);
END

OP(0x71) // LD (HL), C
    WRITE(HL.W, BC.B.l);
END

OP(0x72) // LD (HL), D
    WRITE(HL.W, DE.B.h);
END

OP(0x73) // LD (HL), E
    WRITE(HL.W, DE.B.l);
END

OP(0x74) // LD (HL), H
    WRITE(HL.W, HL.B.h);
END

OP(0x75) // LD (HL), L
    WRITE(HL.W, HL.B.l);
END

OP(0x77) // LD (HL), A
    WRITE(HL.W, AF.B.h);
END

//LD B, x
OP(0x40) // LD B, B
    BC.B.h = BC.B.h;
END

OP(0x41) // LD B, C
    BC.B.h = BC.B.l;
END

OP(0x42) // LD B, D
    BC.B.h = DE.B.h;
END

OP(0x43) // LD B, E
    BC.B.h = DE.B.l;
END

OP(0x44) // LD B, H
    BC.B.h = HL.B.h;
END

OP(0x45) // LD B, L
    BC.B.h = HL.B.l;
END

OP(0x46) // LD B, (HL)
    BC.B.h = READ(HL.W);
END

OP(0x47) // LD B, A
    BC.B.h = AF.B.h;
END

//LD D, x
OP(0x50) // LD D, B
    DE.B.h = BC.B.h;
END

OP(0x51) // LD D, C
    DE.B.h = BC.B.l;
END

OP(0x52) // LD D, D
    DE.B.h = DE.B.h;
END

OP(0x53) // LD D, E
    DE.B.h = DE.B.l;
END

OP(0x54) // LD D, H
    DE.B.h = HL.B.h;
END

OP(0x55) // LD D, L
    DE.B.h = HL.B.l;
END

OP(0x56) // LD D, (HL)
    DE.B.h = READ(HL.W);
END

OP(0x57) // LD D, A
    DE.B.h = AF.B.h;
END

//LD H, x
OP(0x60) // LD H, B
    HL.B.h = BC.B.h;
END

OP(0x61) // LD H, C
    HL.B.h = BC.B.l;
END

OP(0x62) // LD H, D
    HL.B.h = DE.B.h;
END

OP(0x63) // LD H, E
    HL.B.h = DE.B.l;
END

OP(0x64) // LD H, H
    HL.B.h = HL.B.h;
END

OP(0x65) // LD H, L
    HL.B.h = HL.B.l;
END

OP(0x66) // LD H, (HL)
    HL.B.h = READ(HL.W);
END

OP(0x67) // LD H, A
    HL.B.h = AF.B.h;
END

//LD C, x
OP(0x48) // LD C, B
    BC.B.l = BC.B.h;
END

OP(0x49) // LD C, C
    BC.B.l = BC.B.l;
END

OP(0x4A) // LD C, D
    BC.B.l = DE.B.h;
END

OP(0x4B) // LD C, E
    BC.B.l = DE.B.l;
END

OP(0x4C) // LD C, H
    BC.B.l = HL.B.h;
END

OP(0x4D) // LD C, L
    BC.B.l = HL.B.l;
END

OP(0x4E) // LD C, (HL)
    BC.B.l = READ(HL.W);
END

OP(0x4F) // LD C, A
    BC.B.l = AF.B.h;
END

//LD E, x
OP(0x58) // LD E, B
    DE.B.l = BC.B.h;
END

OP(0x59) // LD E, C
    DE.B.l = BC.B.l;
END

OP(0x5A) // LD E, D
    DE.B.l = DE.B.h;
END

OP(0x5B) // LD E, E
    DE.B.l = DE.B.l;
END

OP(0x5C) // LD E, H
    DE.B.l = HL.B.h;
END

OP(0x5D) // LD E, L
    DE.B.l = HL.B.l;
END

OP(0x5E) // LD E, (HL)
    DE.B.l = READ(HL.W);
END

OP(0x5F) // LD E, A
    DE.B.l = AF.B.h;
END

//LD L, x
OP(0x68) // LD L, B
    HL.B.l = BC.B.h;
END

OP(0x69) // LD L, C
    HL.B.l = BC.B.l;
END

OP(0x6A) // LD L, D
    HL.B.l = DE.B.h;
END

OP(0x6B) // LD L, E
    HL.B.l = DE.B.l;
END

OP(0x6C) // LD L, H
    HL.B.l = HL.B.h;
END

OP(0x6D) // LD L, L
    HL.B.l = HL.B.l;
END

OP(0x6E) // LD L, (HL)
    HL.B.l = READ(HL.W);
END

OP(0x6F) // LD L, A
    HL.B.l = AF.B.h;
END

//LD A, x
OP(0x78) // LD A, B
    AF.B.h = BC.B.h;
END

OP(0x79) // LD A, C
    AF.B.h = BC.B.l;
END

OP(0x7A) // LD A, D
    AF.B.h = DE.B.h;
END

OP(0x7B) // LD A, E
    AF.B.h = DE.B.l;
END

OP(0x7C) // LD A, H
    AF.B.h = HL.B.h;
END

OP(0x7D) // LD A, L
    AF.B.h = HL.B.l;
END

OP(0x7E) // LD A, (HL)
    AF.B.h = READ(HL.W);
END

OP(0x7F) // LD A, A
    AF.B.h = AF.B.h;
END


// LD A, (xx)
OP(0x0A) // LD A, (BC)
    AF.B.h = READ(BC.W);
END

OP(0x1A) // LD A, (DE)
    AF.B.h = READ(DE.W);
END

OP(0x2A) // LD A, (HL+)
    AF.B.h = READ(HL.W);
    HL.W++;
END

OP(0x3A) // LD A, (HL-)
    AF.B.h = READ(HL.W);
    HL.W--;
END

OP(0xFA) // LD A, (nn)
    J.B.l = READ_INC();
    J.B.h = READ_INC();
    AF.B.h = READ(J.W);
END

OP(0xEA) // LD (nn), A
    J.B.l = READ_INC();
    J.B.h = READ_INC();
    WRITE(J.W, AF.B.h);
END


// INC xx
OP(0x03) // INC BC
    BC.W++;
END

OP(0x13) // INC DE
    DE.W++;
END

OP(0x23) // INC HL
    HL.W++;
END

OP(0x33) // INC SP
    SP.W++;
END

// DEC xx
OP(0x0B) // DEC BC
    BC.W--;
END

OP(0x1B) // DEC DE
    DE.W--;
END

OP(0x2B) // DEC HL
    HL.W--;
END

OP(0x3B) // DEC SP
    SP.W--;
END

// INC x
OP(0x04) // INC B
    BC.B.h++;
    SET_INC_FLAGS(BC.B.h);
END

OP(0x14) // INC D
    DE.B.h++;
    SET_INC_FLAGS(DE.B.h);
END

OP(0x24) // INC H
    HL.B.h++;
    SET_INC_FLAGS(HL.B.h);
END

OP(0x34) // INC (HL)
    J.B.h = READ(HL.W)+1;
    WRITE(HL.W, J.B.h);
    SET_INC_FLAGS(J.B.h);
END

OP(0x0C) // INC C
BC.B.l++;
    SET_INC_FLAGS(BC.B.l);
END

OP(0x1C) // INC E
    DE.B.l++;
    SET_INC_FLAGS(DE.B.l);
END

OP(0x2C) // INC L
    HL.B.l++;
    SET_INC_FLAGS(HL.B.l);
END

OP(0x3C) // INC A
    AF.B.h++;
    SET_INC_FLAGS(AF.B.h);
END

// DEC x
OP(0x05) // DEC B
    BC.B.h--;
    SET_DEC_FLAGS(BC.B.h);
END

OP(0x15) // DEC D
    DE.B.h--;
    SET_DEC_FLAGS(DE.B.h);
END

OP(0x25) // DEC H
    HL.B.h--;
    SET_DEC_FLAGS(HL.B.h);
END

OP(0x35) // DEC (HL)
    J.B.h = READ(HL.W)-1;
    WRITE(HL.W, J.B.h);
    SET_DEC_FLAGS(J.B.h);
END

OP(0x0D) // DEC C
    BC.B.l--;
    SET_DEC_FLAGS(BC.B.l);
END

OP(0x1D) // DEC E
    DE.B.l--;
    SET_DEC_FLAGS(DE.B.l);
END

OP(0x2D) // DEC L
    HL.B.l--;
    SET_DEC_FLAGS(HL.B.l);
END

OP(0x3D) // DEC A
    AF.B.h--;
    SET_DEC_FLAGS(AF.B.h);
END

// LD x, n
OP(0x06) // LD B, n
    BC.B.h = READ_INC();
END

OP(0x16) // LD D, n
    DE.B.h = READ_INC();
END

OP(0x26) // LD H, n
    HL.B.h = READ_INC();
END

OP(0x36) // LD (HL), n
    WRITE(HL.W, READ_INC());
END

OP(0x0E) // LD C, n
    BC.B.l = READ_INC();
END

OP(0x1E) // LD E, n
    DE.B.l = READ_INC();
END

OP(0x2E) // LD L, n
    HL.B.l = READ_INC();
END

OP(0x3E) // LD A, n
    AF.B.h = READ_INC();
END

//ROTATE
OP(0x07) // RLCA
    RLC_X(AF.B.h);
END

OP(0x17) // RLA
    RL_X(AF.B.h);
END

OP(0x0F) // RRCA
    RRC_X(AF.B.h);
END

OP(0x1F) // RRA
    RR_X(AF.B.h);
END

// JP
OP(0xC3) // JP nn
    J.B.l = READ_INC();
    J.B.h = READ_INC();
    PC.W = J.W;
END

OP(0xE9) // JP (HL)
    PC.W = HL.W;
END

OP(0xC2) // JP NZ, nn
    J.B.l = READ_INC();
    J.B.h = READ_INC();
    if ((AF.B.l&FLAG_Z) == 0) {
        intCycles -= 4;
        opCycles += 4;
        PC.W = J.W;
    }
END

OP(0xD2) // JP NC, nn
    J.B.l = READ_INC();
    J.B.h = READ_INC();
    if ((AF.B.l&FLAG_C) == 0) {
        intCycles -= 4;
        opCycles += 4;
        PC.W = J.W;
    }
END

OP(0xCA) // JP Z, nn
    J.B.l = READ_INC();
    J.B.h = READ_INC();
    if (AF.B.l&FLAG_Z) {
        intCycles -= 4;
        opCycles += 4;
        PC.W = J.W;
    }
END

OP(0xDA) // JP C, nn
    J.B.l = READ_INC();
    J.B.h = READ_INC();
    if (AF.B.l&FLAG_C) {
        intCycles -= 4;
        opCycles += 4;
        PC.W = J.W;
    }
END


// JR
OP(0x18) // JR n
    I = READ_INC();
    PC.W += ((signedbyte)I);
END

OP(0x20) // JR NZ, n
    I = READ_INC();
    if ((AF.B.l & FLAG_Z) == 0) {
        intCycles -= 4;
        opCycles+= 4;
        PC.W += ((signedbyte)I);
    }
END

OP(0x28) // JR Z, n
    I = READ_INC();
    if (AF.B.l & FLAG_Z) {
        intCycles -= 4; opCycles += 4;
        PC.W += ((signedbyte)I);
    }
END

OP(0x30) // JR NC, n
    I = READ_INC();
    if ((AF.B.l & FLAG_C) == 0) {
        intCycles -= 4; opCycles += 4;
        PC.W += ((signedbyte)I);
    }
END

OP(0x38) // JR C, n
    I = READ_INC();
    if (AF.B.l & FLAG_C) {
        intCycles -= 4; opCycles += 4;
        PC.W += ((signedbyte)I);
    }
END

OP(0x08) // LD (nn), SP
    J.B.l = READ_INC();
    J.B.h = READ_INC();
    WRITE(J.W, SP.B.l);
    WRITE(J.W+1, SP.B.h);
END

OP(0xF9) // LD SP, HL
    SP.W = HL.W;
END

OP(0xF8) // LD HL, SP+n
    HL.W = SP.W + ((signedbyte)READ_INC());
    AF.B.l = ((HL.B.l&0x08)>(SP.B.l&0x08)?FLAG_H:0)|((HL.B.l&0x80)>(SP.B.l&0x80)?FLAG_C:0);
END

// ADD xx, yy
OP(0x09) // ADD HL, BC
    ADD_WORDS(HL.W, BC.W);
    HL.W = J.W;
END

OP(0x19) // ADD HL, DE
    ADD_WORDS(HL.W, DE.W);
    HL.W = J.W;
END

OP(0x29) // ADD HL, HL
    ADD_WORDS(HL.W, HL.W);
    HL.W = J.W;
END

OP(0x39) // ADD HL, SP
    ADD_WORDS(HL.W, SP.W);
    HL.W = J.W;
END


// ADD A, x
OP(0x80) // ADD A, B
    ADD_A(BC.B.h);
END

OP(0x81) // ADD A, C
    ADD_A(BC.B.l);
END

OP(0x82) // ADD A, D
    ADD_A(DE.B.h);
END

OP(0x83) // ADD A, E
    ADD_A(DE.B.l);
END

OP(0x84) // ADD A, H
    ADD_A(HL.B.h);
END

OP(0x85) // ADD A, L
    ADD_A(HL.B.l);
END

OP(0x87) // ADD A, A
    ADD_A(AF.B.h);
END

OP(0x86) // ADD A, (HL)
    ADD_A(READ(HL.W));
END

OP(0xC6) // ADD A, n
    ADD_A(READ_INC());
END

// SUB A, x
OP(0x90) // SUB A, B
    SUB_A(BC.B.h);
END

OP(0x91) // SUB A, C
    SUB_A(BC.B.l);
END

OP(0x92) // SUB A, D
    SUB_A(DE.B.h);
END

OP(0x93) // SUB A, E
    SUB_A(DE.B.l);
END

OP(0x94) // SUB A, H
    SUB_A(HL.B.h);
END

OP(0x95) // SUB A, L
    SUB_A(HL.B.l);
END

OP(0x97) // SUB A, A
    SUB_A(AF.B.h);
END

OP(0x96) // SUB A, (HL)
    SUB_A(READ(HL.W));
END

OP(0xD6) // SUB A, n
    SUB_A(READ_INC());
END

// AND A, x
OP(0xA0) // AND A, B
    AND_A(BC.B.h);
END

OP(0xA1) // AND A, C
    AND_A(BC.B.l);
END

OP(0xA2) // AND A, D
    AND_A(DE.B.h);
END

OP(0xA3) // AND A, E
    AND_A(DE.B.l);
END

OP(0xA4) // AND A, H
    AND_A(HL.B.h);
END

OP(0xA5) // AND A, L
    AND_A(HL.B.l);
END

OP(0xA7) // AND A, A
    AND_A(AF.B.h);
END

OP(0xA6) // AND A, (HL)
    AND_A(READ(HL.W));
END

OP(0xE6) // AND A, n
    AND_A(READ_INC());
END

// OR A, x
OP(0xB0) // OR A, B
    OR_A(BC.B.h);
END

OP(0xB1) // OR A, C
    OR_A(BC.B.l);
END

OP(0xB2) // OR A, D
    OR_A(DE.B.h);
END

OP(0xB3) // OR A, E
    OR_A(DE.B.l);
END

OP(0xB4) // OR A, H
    OR_A(HL.B.h);
END

OP(0xB5) // OR A, L
    OR_A(HL.B.l);
END

OP(0xB7) // OR A, A
    OR_A(AF.B.h);
END

OP(0xB6) // OR A, (HL)
    OR_A(READ(HL.W));
END

OP(0xF6) // OR A, n
    OR_A(READ_INC());
END

// ADC A, x
OP(0x88) // ADC A, B
    ADC_A(BC.B.h);
END

OP(0x89) // ADC A, C
    ADC_A(BC.B.l);
END

OP(0x8A) // ADC A, D
    ADC_A(DE.B.h);
END

OP(0x8B) // ADC A, E
    ADC_A(DE.B.l);
END

OP(0x8C) // ADC A, H
    ADC_A(HL.B.h);
END

OP(0x8D) // ADC A, L
    ADC_A(HL.B.l);
END

OP(0x8E) // ADC A, A
    ADC_A(AF.B.h);
END

OP(0x8F) // ADC A, (HL)
    ADC_A(READ(HL.W));
END

OP(0xCE) // ADC A, n
    ADC_A(READ_INC());
END

// SBC A, x
OP(0x98) // SBC A, B
    SBC_A(BC.B.h);
END

OP(0x99) // SBC A, C
    SBC_A(BC.B.l);
END

OP(0x9A) // SBC A, D
    SBC_A(DE.B.h);
END

OP(0x9B) // SBC A, E
    SBC_A(DE.B.l);
END

OP(0x9C) // SBC A, H
    SBC_A(HL.B.h);
END

OP(0x9D) // SBC A, L
    SBC_A(HL.B.l);
END

OP(0x9E) // SBC A, A
    SBC_A(AF.B.h);
END

OP(0x9F) // SBC A, (HL)
    SBC_A(READ(HL.W));
END

OP(0xDE) // SBC A, n
    SBC_A(READ_INC());
END

// XOR A, x
OP(0xA8) // XOR A, B
    XOR_A(BC.B.h);
END

OP(0xA9) // XOR A, C
    XOR_A(BC.B.l);
END

OP(0xAA) // XOR A, D
    XOR_A(DE.B.h);
END

OP(0xAB) // XOR A, E
    XOR_A(DE.B.l);
END

OP(0xAC) // XOR A, H
    XOR_A(HL.B.h);
END

OP(0xAD) // XOR A, L
    XOR_A(HL.B.l);
END

OP(0xAE) // XOR A, A
    XOR_A(AF.B.h);
END

OP(0xAF) // XOR A, (HL)
    XOR_A(READ(HL.W));
END

OP(0xEE) // XOR A, n
    XOR_A(READ_INC());
END

// CP A, x
OP(0xB8) // CP A, B
    CP_A(BC.B.h);
END

OP(0xB9) // CP A, C
    CP_A(BC.B.l);
END

OP(0xBA) // CP A, D
    CP_A(DE.B.h);
END

OP(0xBB) // CP A, E
    CP_A(DE.B.l);
END

OP(0xBC) // CP A, H
    CP_A(HL.B.h);
END

OP(0xBD) // CP A, L
    CP_A(HL.B.l);
END

OP(0xBE) // CP A, A
    CP_A(AF.B.h);
END

OP(0xBF) // CP A, (HL)
    CP_A(READ(HL.W));
END

OP(0xFE) // CP A, n
    CP_A(READ_INC());
END

// POP xx
OP(0xC1) // POP BC
    POP(BC);
END

OP(0xD1) // POP DE
    POP(DE);
END

OP(0xE1) // POP HL
    POP(HL);
END

OP(0xF1) // POP AF
    POP(AF);
END

// PUSH xx
OP(0xC5) // PUSH BC
    PUSH(BC);
END

OP(0xD5) // PUSH DE
    PUSH(DE);
END

OP(0xE5) // PUSH HL
    PUSH(HL);
END

OP(0xF5) // PUSH AF
    PUSH(AF);
END

// RST
OP(0xC7) // RST 0x00
    PUSH(PC);
    PC.W = 0x0000;
END

OP(0xD7) // RST 0x10
    PUSH(PC);
    PC.W = 0x0010;
END

OP(0xE7) // RST 0x20
    PUSH(PC);
    PC.W = 0x0020;
END

OP(0xF7) // RST 0x30
    PUSH(PC);
    PC.W = 0x0030;
END

OP(0xCF) // RST 0x08
    PUSH(PC);
    PC.W = 0x0008;
END

OP(0xDF) // RST 0x18
    PUSH(PC);
    PC.W = 0x0018;
END

OP(0xEF) // RST 0x28
    PUSH(PC);
    PC.W = 0x0028;
END

OP(0xFF) // RST 0x38
    PUSH(PC);
    PC.W = 0x0038;
END

// RET
OP(0xC9) // RET
    POP(J);
    PC.W = J.W;
END

OP(0xD9) // RETI
    RET();
    IME = IE_ENABLED;
END

OP(0xC8) // RET Z
    if (AF.B.l&FLAG_Z) {
        intCycles -= 12;
        opCycles += 12;
        RET();
    }
END

OP(0xD8) // RET C
    if (AF.B.l&FLAG_C) {
        intCycles -= 12;
        opCycles += 12;
        RET();
    }
END

OP(0xC0) // RET NZ
    if ((AF.B.l&FLAG_Z) == 0) {
        intCycles -= 12;
        opCycles += 12;
        RET();
    }
END

OP(0xD0) // RET NC
    if ((AF.B.l&FLAG_C) == 0) {
        intCycles -= 12;
        opCycles += 12;
        RET();
    }
END

// LD xx, nn
OP(0x01) // LD BC, nn
    BC.B.l = READ_INC();
    BC.B.h = READ_INC();
END

OP(0x11) // LD DE, nn
    DE.B.l = READ_INC();
    DE.B.h = READ_INC();
END

OP(0x21) // LD HL, nn
    HL.B.l = READ_INC();
    HL.B.h = READ_INC();
END

OP(0x31) // LD SP, nn
    SP.B.l = READ_INC();
    SP.B.h = READ_INC();
END

OP(0xE2) // LD (C), A
    WRITE(0xFF00+BC.B.l, AF.B.h);
END


OP(0xF2) // LD A, (C)
    AF.B.h = READ(0xFF00+BC.B.l);
END

OP(0xE0) // LDH (n), A
    WRITE(0xFF00+READ_INC(), AF.B.h);
END

OP(0xF0) // LD A, (n)
    AF.B.h = READ(0xFF00+READ_INC());
END


OP(0x2F) // CPL
    AF.B.h = ~(AF.B.h);
    AF.B.l |= FLAG_N|FLAG_H;
END

OP(0x3F) // CCF
    AF.B.l = (AF.B.l&FLAG_Z)|((AF.B.l&FLAG_C)?0:FLAG_C);
END

OP(0x37) // SCF
    AF.B.l = (AF.B.l&FLAG_Z)|FLAG_C;
END

OP(0xCB) // CB Prefix
    execCB(READ_INC());
END

OP(0xE8) // ADD SP, n
    SP.W += (signedbyte)READ_INC();
END

// CALL
OP(0xCD) // CALL nn
    J.B.l = READ_INC();
    J.B.h = READ_INC();
    PUSH(PC);
    PC.W = J.W;
END

OP(0xC4) // CALL NZ, nn
    J.B.l = READ_INC();
    J.B.h = READ_INC();
    if ((AF.B.l&FLAG_Z) == 0) {
        intCycles -= 12;
        opCycles += 12;
        PUSH(PC);
        PC.W = J.W;
    }
END

OP(0xCC) // CALL Z, nn
    J.B.l = READ_INC();
    J.B.h = READ_INC();
    if (AF.B.l&FLAG_Z) {
        intCycles -= 12;
        opCycles += 12;
        PUSH(PC);
        PC.W = J.W;
    }
END

OP(0xD4) // CALL NC, nn
    J.B.l = READ_INC();
    J.B.h = READ_INC();
    if ((AF.B.l&FLAG_C) == 0) {
        intCycles -= 12;
        opCycles += 12;
        PUSH(PC);
        PC.W = J.W;
    }
END

OP(0xDC) // CALL C, nn
    J.B.l = READ_INC();
    J.B.h = READ_INC();
    if (AF.B.l&FLAG_C) {
        intCycles -= 12;
        opCycles += 12;
        PUSH(PC);
        PC.W = J.W;
    }
END

// Interrupt Enable/disable
OP(0xF3) // ID
    IME = IE_DISABLED;
END

OP(0xFB) // IE
    IME = IE_ENABLED;
END

// Illegals
OP(0xD3) // illegal
    ILLEGAL(0xD3);
END

OP(0xE3) // illegal
    ILLEGAL(0xE3);
END

OP(0xDB) // illegal
    ILLEGAL(0xDB);
END

OP(0xEB) // illegal
    ILLEGAL(0xEB);
END

OP(0xF4) // illegal
    ILLEGAL(0xF4);
END

OP(0xE4) // illegal
    ILLEGAL(0xE4);
END

OP(0xFC) // illegal
    ILLEGAL(0xFC);
END

OP(0xFD) // illegal
    ILLEGAL(0xFD);
END

OP(0xEC) // illegal
    ILLEGAL(0xEC);
END

OP(0xED) // illegal
    ILLEGAL(0xED);
END

OP(0xDD) // illegal
    ILLEGAL(0xDD);
END

#undef OP
#undef END
#endif
