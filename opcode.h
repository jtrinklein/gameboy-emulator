#define OP(x) case x:
#define END break;

OP(0x00) // noop
END



// LD (xx), A
OP(0x02) // LD (BC), A
    WRITE(cpu->BC.W, cpu->AF.B.h);
END

OP(0x12) // LD (DE), A
    WRITE(cpu->DE.W, cpu->AF.B.h);
END

OP(0x22) // LD (HL+), A
    WRITE(cpu->HL.W+1, cpu->AF.B.h);
END

OP(0x32) // LD (HL-), A
    WRITE(cpu->HL.W-1, cpu->AF.B.h);
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
    cpu->AF.B.h = READ(cpu->HL.W+1);
END

OP(0x3A) // LD A, (HL-)
    cpu->AF.B.h = READ(cpu->HL.W-1);
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
    cpu->AF.B.l++;
    SET_INC_FLAGS(cpu->AF.B.l);
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
    J.B.l = READ_INC();
    J.B.h = READ_INC();
    WRITE(J.W, cpu->SP.B.l);
    WRITE(J.W+1, cpu->SP.B.h);
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

// LD xx, nn
OP(0x01) // LD BC, nn
    cpu->BC.B.l = READ_INC();
    cpu->BC.B.h = READ_INC();
END

OP(0x11) // LD DE, nn
    cpu->DE.B.l = READ_INC();
    cpu->DE.B.h = READ_INC();
END

OP(0x21) // LD HL, nn
    cpu->HL.B.l = READ_INC();
    cpu->HL.B.h = READ_INC();
END

OP(0x31) // LD SP, nn
    cpu->SP.B.l = READ_INC();
    cpu->SP.B.h = READ_INC();
END

OP(0x2F) // CPL
    cpu->AF.B.h = ~(cpu->AF.B.h);
    cpu->AF.B.l = cpu->AF.B.l|FLAG_N|FLAG_H;
END

OP(0x3F) // CCF
    cpu->AF.B.l = cpu->AF.B.l|FLAG_Z|((cpu->AF.B.l&FLAG_C)?0:FLAG_C);
END

OP(0x37) // SCF
    cpu->AF.B.l = (cpu->AF.B.l&FLAG_Z)|FLAG_C;
END



#undef OP
#undef END
