#define OP(x) case x:
#define END break;

OP(0x00) // noop
END

OP(0x76) // HALT
    cpu->IF |= IF_HALT;
END

OP(0x10)
    cpu->IF |= IF_STOP;
END

// LD (xx), A
OP(0x02) // LD (BC), A
    WRITE(cpu->BC.W, cpu->AF.B.h);
END

OP(0x12) // LD (DE), A
    WRITE(cpu->DE.W, cpu->AF.B.h);
END

OP(0x22) // LD (HL+), A
    WRITE(cpu->HL.W, cpu->AF.B.h);
    cpu->HL.W++;
END

OP(0x32) // LD (HL-), A
    WRITE(cpu->HL.W, cpu->AF.B.h);
    cpu->HL.W--;
END

// LD (HL), x
OP(0x70) // LD (HL), B
    WRITE(cpu->HL.W, cpu->BC.B.h);
END

OP(0x71) // LD (HL), C
    WRITE(cpu->HL.W, cpu->BC.B.l);
END

OP(0x72) // LD (HL), D
    WRITE(cpu->HL.W, cpu->DE.B.h);
END

OP(0x73) // LD (HL), E
    WRITE(cpu->HL.W, cpu->DE.B.l);
END

OP(0x74) // LD (HL), H
    WRITE(cpu->HL.W, cpu->HL.B.h);
END

OP(0x75) // LD (HL), L
    WRITE(cpu->HL.W, cpu->HL.B.l);
END

OP(0x77) // LD (HL), A
    WRITE(cpu->HL.W, cpu->AF.B.h);
END

//LD B, x
OP(0x40) // LD B, B
    cpu->BC.B.h = cpu->BC.B.h;
END

OP(0x41) // LD B, C
    cpu->BC.B.h = cpu->BC.B.l;
END

OP(0x42) // LD B, D
    cpu->BC.B.h = cpu->DE.B.h;
END

OP(0x43) // LD B, E
    cpu->BC.B.h = cpu->DE.B.l;
END

OP(0x44) // LD B, H
    cpu->BC.B.h = cpu->HL.B.h;
END

OP(0x45) // LD B, L
    cpu->BC.B.h = cpu->HL.B.l;
END

OP(0x46) // LD B, (HL)
    cpu->BC.B.h = READ(cpu->HL.W);
END

OP(0x47) // LD B, A
    cpu->BC.B.h = cpu->AF.B.h;
END

//LD D, x
OP(0x50) // LD D, B
    cpu->DE.B.h = cpu->BC.B.h;
END

OP(0x51) // LD D, C
    cpu->DE.B.h = cpu->BC.B.l;
END

OP(0x52) // LD D, D
    cpu->DE.B.h = cpu->DE.B.h;
END

OP(0x53) // LD D, E
    cpu->DE.B.h = cpu->DE.B.l;
END

OP(0x54) // LD D, H
    cpu->DE.B.h = cpu->HL.B.h;
END

OP(0x55) // LD D, L
    cpu->DE.B.h = cpu->HL.B.l;
END

OP(0x56) // LD D, (HL)
    cpu->DE.B.h = READ(cpu->HL.W);
END

OP(0x57) // LD D, A
    cpu->DE.B.h = cpu->AF.B.h;
END

//LD H, x
OP(0x60) // LD H, B
    cpu->HL.B.h = cpu->BC.B.h;
END

OP(0x61) // LD H, C
    cpu->HL.B.h = cpu->BC.B.l;
END

OP(0x62) // LD H, D
    cpu->HL.B.h = cpu->DE.B.h;
END

OP(0x63) // LD H, E
    cpu->HL.B.h = cpu->DE.B.l;
END

OP(0x64) // LD H, H
    cpu->HL.B.h = cpu->HL.B.h;
END

OP(0x65) // LD H, L
    cpu->HL.B.h = cpu->HL.B.l;
END

OP(0x66) // LD H, (HL)
    cpu->HL.B.h = READ(cpu->HL.W);
END

OP(0x67) // LD H, A
    cpu->HL.B.h = cpu->AF.B.h;
END

//LD C, x
OP(0x48) // LD C, B
    cpu->BC.B.l = cpu->BC.B.h;
END

OP(0x49) // LD C, C
    cpu->BC.B.l = cpu->BC.B.l;
END

OP(0x4A) // LD C, D
    cpu->BC.B.l = cpu->DE.B.h;
END

OP(0x4B) // LD C, E
    cpu->BC.B.l = cpu->DE.B.l;
END

OP(0x4C) // LD C, H
    cpu->BC.B.l = cpu->HL.B.h;
END

OP(0x4D) // LD C, L
    cpu->BC.B.l = cpu->HL.B.l;
END

OP(0x4E) // LD C, (HL)
    cpu->BC.B.l = READ(cpu->HL.W);
END

OP(0x4F) // LD C, A
    cpu->BC.B.l = cpu->AF.B.h;
END

//LD E, x
OP(0x58) // LD E, B
    cpu->DE.B.l = cpu->BC.B.h;
END

OP(0x59) // LD E, C
    cpu->DE.B.l = cpu->BC.B.l;
END

OP(0x5A) // LD E, D
    cpu->DE.B.l = cpu->DE.B.h;
END

OP(0x5B) // LD E, E
    cpu->DE.B.l = cpu->DE.B.l;
END

OP(0x5C) // LD E, H
    cpu->DE.B.l = cpu->HL.B.h;
END

OP(0x5D) // LD E, L
    cpu->DE.B.l = cpu->HL.B.l;
END

OP(0x5E) // LD E, (HL)
    cpu->DE.B.l = READ(cpu->HL.W);
END

OP(0x5F) // LD E, A
    cpu->DE.B.l = cpu->AF.B.h;
END

//LD L, x
OP(0x68) // LD L, B
    cpu->HL.B.l = cpu->BC.B.h;
END

OP(0x69) // LD L, C
    cpu->HL.B.l = cpu->BC.B.l;
END

OP(0x6A) // LD L, D
    cpu->HL.B.l = cpu->DE.B.h;
END

OP(0x6B) // LD L, E
    cpu->HL.B.l = cpu->DE.B.l;
END

OP(0x6C) // LD L, H
    cpu->HL.B.l = cpu->HL.B.h;
END

OP(0x6D) // LD L, L
    cpu->HL.B.l = cpu->HL.B.l;
END

OP(0x6E) // LD L, (HL)
    cpu->HL.B.l = READ(cpu->HL.W);
END

OP(0x6F) // LD L, A
    cpu->HL.B.l = cpu->AF.B.h;
END

//LD A, x
OP(0x78) // LD A, B
    cpu->AF.B.h = cpu->BC.B.h;
END

OP(0x79) // LD A, C
    cpu->AF.B.h = cpu->BC.B.l;
END

OP(0x7A) // LD A, D
    cpu->AF.B.h = cpu->DE.B.h;
END

OP(0x7B) // LD A, E
    cpu->AF.B.h = cpu->DE.B.l;
END

OP(0x7C) // LD A, H
    cpu->AF.B.h = cpu->HL.B.h;
END

OP(0x7D) // LD A, L
    cpu->AF.B.h = cpu->HL.B.l;
END

OP(0x7E) // LD A, (HL)
    cpu->AF.B.h = READ(cpu->HL.W);
END

OP(0x7F) // LD A, A
    cpu->AF.B.h = cpu->AF.B.h;
END


// LD A, (xx)
OP(0x0A) // LD A, (BC)
    cpu->AF.B.h = READ(cpu->BC.W);
END

OP(0x1A) // LD A, (DE)
    cpu->AF.B.h = READ(cpu->DE.W);
END

OP(0x2A) // LD A, (HL+)
    cpu->AF.B.h = READ(cpu->HL.W);
    cpu->HL.W++;
END

OP(0x3A) // LD A, (HL-)
    cpu->AF.B.h = READ(cpu->HL.W);
    cpu->HL.W--;
END

OP(0xFA) // LD A, (nn)
    J.B.h = READ_INC();
    J.B.l = READ_INC();
    cpu->AF.B.h = READ(J.W);
END

OP(0xEA) // LD (nn), A
    J.B.h = READ_INC();
    J.B.l = READ_INC();
    WRITE(J.W, cpu->AF.B.h);
END


// INC xx
OP(0x03) // INC BC
    cpu->BC.W++;
END

OP(0x13) // INC DE
    cpu->DE.W++;
END

OP(0x23) // INC HL
    cpu->HL.W++;
END

OP(0x33) // INC SP
    cpu->SP.W++;
END

// DEC xx
OP(0x0B) // DEC BC
    cpu->BC.W--;
END

OP(0x1B) // DEC DE
    cpu->DE.W--;
END

OP(0x2B) // DEC HL
    cpu->HL.W--;
END

OP(0x3B) // DEC SP
    cpu->SP.W--;
END

// INC x
OP(0x04) // INC B
    cpu->BC.B.h++;
    SET_INC_FLAGS(cpu->BC.B.h);
END

OP(0x14) // INC D
    cpu->DE.B.h++;
    SET_INC_FLAGS(cpu->DE.B.h);
END

OP(0x24) // INC H
    cpu->HL.B.h++;
    SET_INC_FLAGS(cpu->HL.B.h);
END

OP(0x34) // INC (HL)
    J.B.h = READ(cpu->HL.W)+1;
    WRITE(cpu->HL.W, J.B.h);
    SET_INC_FLAGS(J.B.h);
END

OP(0x0C) // INC C
    SET_INC_FLAGS(cpu->BC.B.l++);
END

OP(0x1C) // INC E
    cpu->DE.B.l++;
    SET_INC_FLAGS(cpu->DE.B.l);
END

OP(0x2C) // INC L
    cpu->HL.B.l++;
    SET_INC_FLAGS(cpu->HL.B.l);
END

OP(0x3C) // INC A
    cpu->AF.B.h++;
    SET_INC_FLAGS(cpu->AF.B.h);
END

// DEC x
OP(0x05) // DEC B
    cpu->BC.B.h--;
    SET_DEC_FLAGS(cpu->BC.B.h);
END

OP(0x15) // DEC D
    cpu->DE.B.h--;
    SET_DEC_FLAGS(cpu->DE.B.h);
END

OP(0x25) // DEC H
    cpu->HL.B.h--;
    SET_DEC_FLAGS(cpu->HL.B.h);
END

OP(0x35) // DEC (HL)
    J.B.h = READ(cpu->HL.W)-1;
    WRITE(cpu->HL.W, J.B.h);
    SET_DEC_FLAGS(cpu->HL.B.h);
END

OP(0x0D) // DEC C
    SET_DEC_FLAGS(cpu->BC.B.l--);
END

OP(0x1D) // DEC E
    cpu->DE.B.l--;
    SET_DEC_FLAGS(cpu->DE.B.l);
END

OP(0x2D) // DEC L
    cpu->HL.B.l--;
    SET_DEC_FLAGS(cpu->HL.B.l);
END

OP(0x3D) // DEC A
    cpu->AF.B.l--;
    SET_DEC_FLAGS(cpu->AF.B.l);
END

// LD x, n
OP(0x06) // LD B, n
    cpu->BC.B.h = READ_INC();
END

OP(0x16) // LD D, n
    cpu->DE.B.h = READ_INC();
END

OP(0x26) // LD H, n
    cpu->HL.B.h = READ_INC();
END

OP(0x36) // LD (HL), n
    WRITE(cpu->HL.W, READ_INC());
END

OP(0x0E) // LD C, n
    cpu->BC.B.l = READ_INC();
END

OP(0x1E) // LD E, n
    cpu->DE.B.l = READ_INC();
END

OP(0x2E) // LD L, n
    cpu->HL.B.l = READ_INC();
END

OP(0x3E) // LD A, n
    cpu->AF.B.h = READ_INC();
END

//ROTATE
OP(0x07) // RLCA
    cpu->AF.B.l = (cpu->AF.B.h&0x80)?FLAG_C:0;
    cpu->AF.B.h = (cpu->AF.B.h<<1)|((cpu->AF.B.l&FLAG_C)?0x01:0);
END

OP(0x17) // RLA
    I = (cpu->AF.B.h & 0x80)?FLAG_C:0;
    cpu->AF.B.h = (cpu->AF.B.h<<1)|((cpu->AF.B.l&FLAG_C)?0x01:0);
    cpu->AF.B.l = I;
END

OP(0x0F) // RRCA
    cpu->AF.B.l = (cpu->AF.B.h&0x01)?FLAG_C:0;
    cpu->AF.B.h = (cpu->AF.B.h>>1)|((cpu->AF.B.l&FLAG_C)?0x80:0);
END

OP(0x1F) // RRA
    I = (cpu->AF.B.h & 0x01)?FLAG_C:0;
    cpu->AF.B.h = (cpu->AF.B.h>>1)|((cpu->AF.B.l&FLAG_C)?0x80:0);
    cpu->AF.B.l = I;
END

// JP
OP(0xC3) // JP nn
    J.B.h = READ_INC();
    J.B.l = READ_INC();
    cpu->PC.W = J.W;
END

OP(0xE9) // JP (HL)
    cpu->PC.W = READ(cpu->HL.W);
END

OP(0xC2) // JP NZ, nn
    if ((cpu->AF.B.l&FLAG_Z) == 0) {
        cpu->ICycles += 4;
        J.B.h = READ_INC();
        J.B.l = READ_INC();
        cpu->PC.W = J.W;
    }
END

OP(0xD2) // JP NC, nn
    if ((cpu->AF.B.l&FLAG_C) == 0) {
        cpu->ICycles += 4;
        J.B.h = READ_INC();
        J.B.l = READ_INC();
        cpu->PC.W = J.W;
    }
END

OP(0xCA) // JP Z, nn
    if (cpu->AF.B.l&FLAG_Z) {
        cpu->ICycles += 4;
        J.B.h = READ_INC();
        J.B.l = READ_INC();
        cpu->PC.W = J.W;
    }
END

OP(0xDA) // JP C, nn
    if (cpu->AF.B.l&FLAG_C) {
        cpu->ICycles += 4;
        J.B.h = READ_INC();
        J.B.l = READ_INC();
        cpu->PC.W = J.W;
    }
END


// JR
OP(0x18) // JR n
    I = READ_INC();
    cpu->PC.W += ((signedbyte)I) + 1; // +1 for read_inc()
END

OP(0x20) // JR NZ, n
    I = READ_INC();
    if ((cpu->AF.B.l & FLAG_Z) == 0) {
        cpu->ICycles += 4;
        cpu->PC.W += ((signedbyte)I);
    }
END

OP(0x28) // JR Z, n
    I = READ_INC();
    if (cpu->AF.B.l & FLAG_Z) {
        cpu->ICycles += 4;
        cpu->PC.W += ((signedbyte)I);
    }
END

OP(0x30) // JR NC, n
    I = READ_INC();
    if ((cpu->AF.B.l & FLAG_C) == 0) {
        cpu->ICycles += 4;
        cpu->PC.W += ((signedbyte)I);
    }
END

OP(0x38) // JR C, n
    I = READ_INC();
    if (cpu->AF.B.l & FLAG_C) {
        cpu->ICycles += 4;
        cpu->PC.W += ((signedbyte)I);
    }
END

OP(0x08) // LD (nn), SP
    J.B.h = READ_INC();
    J.B.l = READ_INC();
    WRITE(J.W, cpu->SP.B.l);
    WRITE(J.W+1, cpu->SP.B.h);
END

OP(0xF9) // LD SP, HL
    cpu->SP.W = cpu->HL.W;
END

OP(0xF8) // LD HL, SP+n
    cpu->HL.W = cpu->SP.W + ((signedbyte)READ_INC());
    cpu->AF.B.l = ((cpu->HL.B.l&0x08)>(cpu->SP.B.l&0x08)?FLAG_H:0)|((cpu->HL.B.l&0x80)>(cpu->SP.B.l&0x80)?FLAG_C:0);
END

// ADD xx, yy
OP(0x09) // ADD HL, BC
    ADD_WORDS(cpu->HL.W, cpu->BC.W);
    cpu->HL.W = J.W;
END

OP(0x19) // ADD HL, DE
    ADD_WORDS(cpu->HL.W, cpu->DE.W);
    cpu->HL.W = J.W;
END

OP(0x29) // ADD HL, HL
    ADD_WORDS(cpu->HL.W, cpu->HL.W);
    cpu->HL.W = J.W;
END

OP(0x39) // ADD HL, SP
    ADD_WORDS(cpu->HL.W, cpu->SP.W);
    cpu->HL.W = J.W;
END


// ADD A, x
OP(0x80) // ADD A, B
    ADD_A(cpu->BC.B.h);
END

OP(0x81) // ADD A, C
    ADD_A(cpu->BC.B.l);
END

OP(0x82) // ADD A, D
    ADD_A(cpu->DE.B.h);
END

OP(0x83) // ADD A, E
    ADD_A(cpu->DE.B.l);
END

OP(0x84) // ADD A, H
    ADD_A(cpu->HL.B.h);
END

OP(0x85) // ADD A, L
    ADD_A(cpu->HL.B.l);
END

OP(0x87) // ADD A, A
    ADD_A(cpu->AF.B.h);
END

OP(0x86) // ADD A, (HL)
    ADD_A(READ(cpu->HL.W));
END

OP(0xC6) // ADD A, n
    ADD_A(READ_INC());
END

// SUB A, x
OP(0x90) // SUB A, B
    SUB_A(cpu->BC.B.h);
END

OP(0x91) // SUB A, C
    SUB_A(cpu->BC.B.l);
END

OP(0x92) // SUB A, D
    SUB_A(cpu->DE.B.h);
END

OP(0x93) // SUB A, E
    SUB_A(cpu->DE.B.l);
END

OP(0x94) // SUB A, H
    SUB_A(cpu->HL.B.h);
END

OP(0x95) // SUB A, L
    SUB_A(cpu->HL.B.l);
END

OP(0x97) // SUB A, A
    SUB_A(cpu->AF.B.h);
END

OP(0x96) // SUB A, (HL)
    SUB_A(READ(cpu->HL.W));
END

OP(0xD6) // SUB A, n
    SUB_A(READ_INC());
END

// AND A, x
OP(0xA0) // AND A, B
    AND_A(cpu->BC.B.h);
END

OP(0xA1) // AND A, C
    AND_A(cpu->BC.B.l);
END

OP(0xA2) // AND A, D
    AND_A(cpu->DE.B.h);
END

OP(0xA3) // AND A, E
    AND_A(cpu->DE.B.l);
END

OP(0xA4) // AND A, H
    AND_A(cpu->HL.B.h);
END

OP(0xA5) // AND A, L
    AND_A(cpu->HL.B.l);
END

OP(0xA7) // AND A, A
    AND_A(cpu->AF.B.h);
END

OP(0xA6) // AND A, (HL)
    AND_A(READ(cpu->HL.W));
END

OP(0xE6) // AND A, n
    AND_A(READ_INC());
END

// OR A, x
OP(0xB0) // OR A, B
    OR_A(cpu->BC.B.h);
END

OP(0xB1) // OR A, C
    OR_A(cpu->BC.B.l);
END

OP(0xB2) // OR A, D
    OR_A(cpu->DE.B.h);
END

OP(0xB3) // OR A, E
    OR_A(cpu->DE.B.l);
END

OP(0xB4) // OR A, H
    OR_A(cpu->HL.B.h);
END

OP(0xB5) // OR A, L
    OR_A(cpu->HL.B.l);
END

OP(0xB7) // OR A, A
    OR_A(cpu->AF.B.h);
END

OP(0xB6) // OR A, (HL)
    OR_A(READ(cpu->HL.W));
END

OP(0xF6) // OR A, n
    OR_A(READ_INC());
END

// ADC A, x
OP(0x88) // ADC A, B
    ADC_A(cpu->BC.B.h);
END

OP(0x89) // ADC A, C
    ADC_A(cpu->BC.B.l);
END

OP(0x8A) // ADC A, D
    ADC_A(cpu->DE.B.h);
END

OP(0x8B) // ADC A, E
    ADC_A(cpu->DE.B.l);
END

OP(0x8C) // ADC A, H
    ADC_A(cpu->HL.B.h);
END

OP(0x8D) // ADC A, L
    ADC_A(cpu->HL.B.l);
END

OP(0x8E) // ADC A, A
    ADC_A(cpu->AF.B.h);
END

OP(0x8F) // ADC A, (HL)
    ADC_A(READ(cpu->HL.W));
END

OP(0xCE) // ADC A, n
    ADC_A(READ_INC());
END

// SBC A, x
OP(0x98) // SBC A, B
    SBC_A(cpu->BC.B.h);
END

OP(0x99) // SBC A, C
    SBC_A(cpu->BC.B.l);
END

OP(0x9A) // SBC A, D
    SBC_A(cpu->DE.B.h);
END

OP(0x9B) // SBC A, E
    SBC_A(cpu->DE.B.l);
END

OP(0x9C) // SBC A, H
    SBC_A(cpu->HL.B.h);
END

OP(0x9D) // SBC A, L
    SBC_A(cpu->HL.B.l);
END

OP(0x9E) // SBC A, A
    SBC_A(cpu->AF.B.h);
END

OP(0x9F) // SBC A, (HL)
    SBC_A(READ(cpu->HL.W));
END

OP(0xDE) // SBC A, n
    SBC_A(READ_INC());
END

// XOR A, x
OP(0xA8) // XOR A, B
    XOR_A(cpu->BC.B.h);
END

OP(0xA9) // XOR A, C
    XOR_A(cpu->BC.B.l);
END

OP(0xAA) // XOR A, D
    XOR_A(cpu->DE.B.h);
END

OP(0xAB) // XOR A, E
    XOR_A(cpu->DE.B.l);
END

OP(0xAC) // XOR A, H
    XOR_A(cpu->HL.B.h);
END

OP(0xAD) // XOR A, L
    XOR_A(cpu->HL.B.l);
END

OP(0xAE) // XOR A, A
    XOR_A(cpu->AF.B.h);
END

OP(0xAF) // XOR A, (HL)
    XOR_A(READ(cpu->HL.W));
END

OP(0xEE) // XOR A, n
    XOR_A(READ_INC());
END

// CP A, x
OP(0xB8) // CP A, B
    CP_A(cpu->BC.B.h);
END

OP(0xB9) // CP A, C
    CP_A(cpu->BC.B.l);
END

OP(0xBA) // CP A, D
    CP_A(cpu->DE.B.h);
END

OP(0xBB) // CP A, E
    CP_A(cpu->DE.B.l);
END

OP(0xBC) // CP A, H
    CP_A(cpu->HL.B.h);
END

OP(0xBD) // CP A, L
    CP_A(cpu->HL.B.l);
END

OP(0xBE) // CP A, A
    CP_A(cpu->AF.B.h);
END

OP(0xBF) // CP A, (HL)
    CP_A(READ(cpu->HL.W));
END

OP(0xFE) // CP A, n
    CP_A(READ_INC());
END

// POP xx
OP(0xC1) // POP BC
    POP(cpu->BC);
END

OP(0xD1) // POP DE
    POP(cpu->DE);
END

OP(0xE1) // POP HL
    POP(cpu->HL);
END

OP(0xF1) // POP AF
    POP(cpu->AF);
END

// PUSH xx
OP(0xC5) // PUSH BC
    PUSH(cpu->BC);
END

OP(0xD5) // PUSH DE
    PUSH(cpu->DE);
END

OP(0xE5) // PUSH HL
    PUSH(cpu->HL);
END

OP(0xF5) // PUSH AF
    PUSH(cpu->AF);
END

// RST
OP(0xC7) // RST 0x00
    PUSH(cpu->PC);
    cpu->PC.W = 0x0000;
END

OP(0xD7) // RST 0x10
    PUSH(cpu->PC);
    cpu->PC.W = 0x0010;
END

OP(0xE7) // RST 0x20
    PUSH(cpu->PC);
    cpu->PC.W = 0x0020;
END

OP(0xF7) // RST 0x30
    PUSH(cpu->PC);
    cpu->PC.W = 0x0030;
END

OP(0xCF) // RST 0x08
    PUSH(cpu->PC);
    cpu->PC.W = 0x0008;
END

OP(0xDF) // RST 0x18
    PUSH(cpu->PC);
    cpu->PC.W = 0x0018;
END

OP(0xEF) // RST 0x28
    PUSH(cpu->PC);
    cpu->PC.W = 0x0028;
END

OP(0xFF) // RST 0x38
    PUSH(cpu->PC);
    cpu->PC.W = 0x0038;
END

// RET
OP(0xC9) // RET
    RET();
END

OP(0xD9) // RETI
    RET();
    cpu->IF |= IF_IE;
END

OP(0xC8) // RET Z
    if (cpu->AF.B.l&FLAG_Z) {
        cpu->ICycles += 12;
        RET();
    }
END

OP(0xD8) // RET C
    if (cpu->AF.B.l&FLAG_C) {
        cpu->ICycles += 12;
        RET();
    }
END

OP(0xC0) // RET NZ
    if ((cpu->AF.B.l&FLAG_Z) == 0) {
        cpu->ICycles += 12;
        RET();
    }
END

OP(0xD0) // RET NC
    if ((cpu->AF.B.l&FLAG_C) == 0) {
        cpu->ICycles += 12;
        RET();
    }
END

// LD xx, nn
OP(0x01) // LD BC, nn
    cpu->BC.B.h = READ_INC();
    cpu->BC.B.l = READ_INC();
END

OP(0x11) // LD DE, nn
    cpu->DE.B.h = READ_INC();
    cpu->DE.B.l = READ_INC();
END

OP(0x21) // LD HL, nn
    cpu->HL.B.h = READ_INC();
    cpu->HL.B.l = READ_INC();
END

OP(0x31) // LD SP, nn
    cpu->SP.B.h = READ_INC();
    cpu->SP.B.l = READ_INC();
END

OP(0xE2) // LD (C), A
    WRITE(0xFF00+cpu->BC.B.l, cpu->AF.B.h);
END


OP(0xF2) // LD A, (C)
    cpu->AF.B.h = READ(0xFF00+cpu->BC.B.l);
END

OP(0xE0) // LDH (n), A
    WRITE(0xFF00+READ_INC(), cpu->AF.B.h);
END

OP(0xF0) // LD A, (n)
    cpu->AF.B.h = READ(0xFF00+READ_INC());
END


OP(0x2F) // CPL
    cpu->AF.B.l = ~(cpu->AF.B.h);
    cpu->AF.B.h = cpu->AF.B.l|FLAG_N|FLAG_H;
END

OP(0x3F) // CCF
    cpu->AF.B.l = cpu->AF.B.l|FLAG_Z|((cpu->AF.B.l&FLAG_C)?0:FLAG_C);
END

OP(0x37) // SCF
    cpu->AF.B.l = (cpu->AF.B.l&FLAG_Z)|FLAG_C;
END

OP(0xCB) // CB Prefix
    ExecCB(cpu);
END

OP(0xE8) // ADD SP, n
    cpu->SP.W += (signedbyte)READ_INC();
END

// CALL
OP(0xCD) // CALL nn
    J.B.h = READ_INC();
    J.B.l = READ_INC();
    PUSH(cpu->PC);
    cpu->PC.W = J.W;
END

OP(0xC4) // CALL NZ, nn
    J.B.h = READ_INC();
    J.B.l = READ_INC();
    if ((cpu->AF.B.l&FLAG_Z) == 0) {
        cpu->ICycles += 12;
        PUSH(cpu->PC);
        cpu->PC.W = J.W;
    }
END

OP(0xCC) // CALL Z, nn
    J.B.h = READ_INC();
    J.B.l = READ_INC();
    if (cpu->AF.B.l&FLAG_Z) {
        cpu->ICycles += 12;
        PUSH(cpu->PC);
        cpu->PC.W = J.W;
    }
END

OP(0xD4) // CALL NC, nn
    J.B.h = READ_INC();
    J.B.l = READ_INC();
    if ((cpu->AF.B.l&FLAG_C) == 0) {
        cpu->ICycles += 12;
        PUSH(cpu->PC);
        cpu->PC.W = J.W;
    }
END

OP(0xDC) // CALL C, nn
    J.B.h = READ_INC();
    J.B.l = READ_INC();
    if (cpu->AF.B.l&FLAG_C) {
        cpu->ICycles += 12;
        PUSH(cpu->PC);
        cpu->PC.W = J.W;
    }
END

// Interrupt Enable/disable
OP(0xF3) // ID
    cpu->IF &= (~IF_IE);
END

OP(0xFB) // IE
    cpu->IF |= IF_IE;
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
