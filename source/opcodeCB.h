#ifndef __OPCODECB_H__
#define __OPCODECB_H__

#define OP(x) case x:
#define END break

OP(0x00) // RLC B
    RLC(BC.B.h);
END;

OP(0x01) // RLC C
    RLC(BC.B.l);
END;

OP(0x02) // RLC D
    RLC(DE.B.h);
END;

OP(0x03) // RLC E
    RLC(DE.B.l);
END;

OP(0x04) // RLC H
    RLC(HL.B.h);
END;

OP(0x05) // RLC L
    RLC(HL.B.l);
END;

OP(0x06) // RLC (HL)
    J.B.h = READ(HL.W);
    RLC(J.B.h);
    WRITE(HL.W, J.B.h);
END;

OP(0x07) // RLC A
    RLC(AF.B.h);
END;


OP(0x10) // RL B
    RL(BC.B.h);
END;

OP(0x11) // RL C
    RL(BC.B.l);
END;

OP(0x12) // RL D
    RL(DE.B.h);
END;

OP(0x13) // RL E
    RL(DE.B.l);
END;

OP(0x14) // RL H
    RL(HL.B.h);
END;

OP(0x15) // RL L
    RL(HL.B.l);
END;

OP(0x16) // RL (HL)
    J.B.h = READ(HL.W);
    RL(J.B.h);
    WRITE(HL.W, J.B.h);
END;

OP(0x17) // RL A
    RL(AF.B.h);
END;



OP(0x08) // RRC B
    RRC(BC.B.h);
END;

OP(0x09) // RRC C
    RRC(BC.B.l);
END;

OP(0x0A) // RRC D
    RRC(DE.B.h);
END;

OP(0x0B) // RRC E
    RRC(DE.B.l);
END;

OP(0x0C) // RRC H
    RRC(HL.B.h);
END;

OP(0x0D) // RRC L
    RRC(HL.B.l);
END;

OP(0x0E) // RRC (HL)
    J.B.h = READ(HL.W);
    RRC(J.B.h);
    WRITE(HL.W, J.B.h);
END;

OP(0x0F) // RRC A
    RRC(AF.B.h);
END;


OP(0x18) // RR B
    RR(BC.B.h);
END;

OP(0x19) // RR C
    RR(BC.B.l);
END;

OP(0x1A) // RR D
    RR(DE.B.h);
END;

OP(0x1B) // RR E
    RR(DE.B.l);
END;

OP(0x1C) // RR H
    RR(HL.B.h);
END;

OP(0x1D) // RR L
    RR(HL.B.l);
END;

OP(0x1E) // RR (HL)
    J.B.h = READ(HL.W);
    RR(J.B.h);
    WRITE(HL.W, J.B.h);
END;

OP(0x1F) // RR A
    RR(AF.B.h);
END;


// SLA
OP(0x20) // SLA B
    SLA_X(BC.B.h);
END;

OP(0x21) // SLA C
    SLA_X(BC.B.l);
END;

OP(0x22) // SLA D
    SLA_X(DE.B.h);
END;

OP(0x23) // SLA E
    SLA_X(DE.B.l);
END;

OP(0x24) // SLA H
    SLA_X(HL.B.h);
END;

OP(0x25) // SLA L
    SLA_X(HL.B.l);
END;

OP(0x26) // SLA (HL)
    J.B.h = READ(HL.W);
    SLA_X(J.B.h);
    WRITE(HL.W, J.B.h);
END;

OP(0x27) // SLA A
    SLA_X(AF.B.h);
END;

// SRA
OP(0x28) // SRA B
    SRA_X(BC.B.h);
END;

OP(0x29) // SRA C
    SRA_X(BC.B.l);
END;

OP(0x2A) // SRA D
    SRA_X(DE.B.h);
END;

OP(0x2B) // SRA E
    SRA_X(DE.B.l);
END;

OP(0x2C) // SRA H
    SRA_X(HL.B.h);
END;

OP(0x2D) // SRA L
    SRA_X(HL.B.l);
END;

OP(0x2E) // SRA (HL)
    J.B.h = READ(HL.W);
    SRA_X(J.B.h);
    WRITE(HL.W, J.B.h);
END;

OP(0x2F) // SRA A
    SRA_X(AF.B.h);
END;

// SWAP
OP(0x30) // SWAP B
    SWAP_X(BC.B.h);
END;

OP(0x31) // SWAP C
    SWAP_X(BC.B.l);
END;

OP(0x32) // SWAP D
    SWAP_X(DE.B.h);
END;

OP(0x33) // SWAP E
    SWAP_X(DE.B.l);
END;

OP(0x34) // SWAP H
    SWAP_X(HL.B.h);
END;

OP(0x35) // SWAP L
    SWAP_X(HL.B.l);
END;

OP(0x36) // SWAP (HL)
    J.B.h = READ(HL.W);
    SWAP_X(J.B.h);
    WRITE(HL.W, J.B.h);
END;

OP(0x37) // SWAP A
    SWAP_X(AF.B.h);
END;


// SRL
OP(0x38) // SRL B
    SRL_X(BC.B.h);
END;

OP(0x39) // SRL C
    SRL_X(BC.B.l);
END;

OP(0x3A) // SRL D
    SRL_X(DE.B.h);
END;

OP(0x3B) // SRL E
    SRL_X(DE.B.l);
END;

OP(0x3C) // SRL H
    SRL_X(HL.B.h);
END;

OP(0x3D) // SRL L
    SRL_X(HL.B.l);
END;

OP(0x3E) // SRL (HL)
    J.B.h = READ(HL.W);
    SRL_X(J.B.h);
    WRITE(HL.W, J.B.h);
END;

OP(0x3F) // SRL A
    SRL_X(AF.B.h);
END;

OP(0x40) // BIT 0, B
    BIT(0, BC.B.h);
END;

OP(0x41) // BIT 0, C
    BIT(0, BC.B.l);
END;

OP(0x42) // BIT 0, D
    BIT(0, DE.B.h);
END;

OP(0x43) // BIT 0, E
    BIT(0, DE.B.l);
END;

OP(0x44) // BIT 0, H
    BIT(0, HL.B.h);
END;

OP(0x45) // BIT 0, L
    BIT(0, HL.B.l);
END;

OP(0x47) // BIT 0, A
    BIT(0, AF.B.h);
END;

OP(0x46) // BIT 0, (HL)
    J.B.h = READ(HL.W);
    BIT(0, J.B.l);
    WRITE(HL.W, J.B.h);
END;


OP(0x50) // BIT 2, B
    BIT(2, BC.B.h);
END;

OP(0x51) // BIT 2, C
    BIT(2, BC.B.l);
END;

OP(0x52) // BIT 2, D
    BIT(2, DE.B.h);
END;

OP(0x53) // BIT 2, E
    BIT(2, DE.B.l);
END;

OP(0x54) // BIT 2, H
    BIT(2, HL.B.h);
END;

OP(0x55) // BIT 2, L
    BIT(2, HL.B.l);
END;

OP(0x57) // BIT 2, A
    BIT(2, AF.B.h);
END;

OP(0x56) // BIT 2, (HL)
    J.B.h = READ(HL.W);
    BIT(2, J.B.l);
    WRITE(HL.W, J.B.h);
END;


OP(0x60) // BIT 4, B
    BIT(4, BC.B.h);
END;

OP(0x61) // BIT 4, C
    BIT(4, BC.B.l);
END;

OP(0x62) // BIT 4, D
    BIT(4, DE.B.h);
END;

OP(0x63) // BIT 4, E
    BIT(4, DE.B.l);
END;

OP(0x64) // BIT 4, H
    BIT(4, HL.B.h);
END;

OP(0x65) // BIT 4, L
    BIT(4, HL.B.l);
END;

OP(0x67) // BIT 4, A
    BIT(4, AF.B.h);
END;

OP(0x66) // BIT 4, (HL)
    J.B.h = READ(HL.W);
    BIT(4, J.B.l);
    WRITE(HL.W, J.B.h);
END;


OP(0x70) // BIT 6, B
    BIT(6, BC.B.h);
END;

OP(0x71) // BIT 6, C
    BIT(6, BC.B.l);
END;

OP(0x72) // BIT 6, D
    BIT(6, DE.B.h);
END;

OP(0x73) // BIT 6, E
    BIT(6, DE.B.l);
END;

OP(0x74) // BIT 6, H
    BIT(6, HL.B.h);
END;

OP(0x75) // BIT 6, L
    BIT(6, HL.B.l);
END;

OP(0x77) // BIT 6, A
    BIT(6, AF.B.h);
END;

OP(0x76) // BIT 6, (HL)
    J.B.h = READ(HL.W);
    BIT(6, J.B.l);
    WRITE(HL.W, J.B.h);
END;


OP(0x48) // BIT 1, B
    BIT(1, BC.B.h);
END;

OP(0x49) // BIT 1, C
    BIT(1, BC.B.l);
END;

OP(0x4A) // BIT 1, D
    BIT(1, DE.B.h);
END;

OP(0x4B) // BIT 1, E
    BIT(1, DE.B.l);
END;

OP(0x4C) // BIT 1, H
    BIT(1, HL.B.h);
END;

OP(0x4D) // BIT 1, L
    BIT(1, HL.B.l);
END;

OP(0x4E) // BIT 1, A
    BIT(1, AF.B.h);
END;

OP(0x4F) // BIT 1, (HL)
    J.B.h = READ(HL.W);
    BIT(1, J.B.l);
    WRITE(HL.W, J.B.h);
END;



OP(0x58) // BIT 3, B
    BIT(3, BC.B.h);
END;

OP(0x59) // BIT 3, C
    BIT(3, BC.B.l);
END;

OP(0x5A) // BIT 3, D
    BIT(3, DE.B.h);
END;

OP(0x5B) // BIT 3, E
    BIT(3, DE.B.l);
END;

OP(0x5C) // BIT 3, H
    BIT(3, HL.B.h);
END;

OP(0x5D) // BIT 3, L
    BIT(3, HL.B.l);
END;

OP(0x5E) // BIT 3, A
    BIT(3, AF.B.h);
END;

OP(0x5F) // BIT 3, (HL)
    J.B.h = READ(HL.W);
    BIT(3, J.B.l);
    WRITE(HL.W, J.B.h);
END;



OP(0x68) // BIT 5, B
    BIT(5, BC.B.h);
END;

OP(0x69) // BIT 5, C
    BIT(5, BC.B.l);
END;

OP(0x6A) // BIT 5, D
    BIT(5, DE.B.h);
END;

OP(0x6B) // BIT 5, E
    BIT(5, DE.B.l);
END;

OP(0x6C) // BIT 5, H
    BIT(5, HL.B.h);
END;

OP(0x6D) // BIT 5, L
    BIT(5, HL.B.l);
END;

OP(0x6E) // BIT 5, A
    BIT(5, AF.B.h);
END;

OP(0x6F) // BIT 5, (HL)
    J.B.h = READ(HL.W);
    BIT(5, J.B.l);
    WRITE(HL.W, J.B.h);
END;



OP(0x78) // BIT 7, B
    BIT(7, BC.B.h);
END;

OP(0x79) // BIT 7, C
    BIT(7, BC.B.l);
END;

OP(0x7A) // BIT 7, D
    BIT(7, DE.B.h);
END;

OP(0x7B) // BIT 7, E
    BIT(7, DE.B.l);
END;

OP(0x7C) // BIT 7, H
    BIT(7, HL.B.h);
END;

OP(0x7D) // BIT 7, L
    BIT(7, HL.B.l);
END;

OP(0x7E) // BIT 7, A
    BIT(7, AF.B.h);
END;

OP(0x7F) // BIT 7, (HL)
    J.B.h = READ(HL.W);
    BIT(7, J.B.l);
    WRITE(HL.W, J.B.h);
END;


OP(0x80) // RES 0, B
    RES(0, BC.B.h);
END;

OP(0x81) // RES 0, C
    RES(0, BC.B.l);
END;

OP(0x82) // RES 0, D
    RES(0, DE.B.h);
END;

OP(0x83) // RES 0, E
    RES(0, DE.B.l);
END;

OP(0x84) // RES 0, H
    RES(0, HL.B.h);
END;

OP(0x85) // RES 0, L
    RES(0, HL.B.l);
END;

OP(0x87) // RES 0, A
    RES(0, AF.B.h);
END;

OP(0x86) // RES 0, (HL)
    J.B.h = READ(HL.W);
    RES(0, J.B.l);
    WRITE(HL.W, J.B.h);
END;


OP(0x90) // RES 2, B
    RES(2, BC.B.h);
END;

OP(0x91) // RES 2, C
    RES(2, BC.B.l);
END;

OP(0x92) // RES 2, D
    RES(2, DE.B.h);
END;

OP(0x93) // RES 2, E
    RES(2, DE.B.l);
END;

OP(0x94) // RES 2, H
    RES(2, HL.B.h);
END;

OP(0x95) // RES 2, L
    RES(2, HL.B.l);
END;

OP(0x97) // RES 2, A
    RES(2, AF.B.h);
END;

OP(0x96) // RES 2, (HL)
    J.B.h = READ(HL.W);
    RES(2, J.B.l);
    WRITE(HL.W, J.B.h);
END;


OP(0xA0) // RES 4, B
    RES(4, BC.B.h);
END;

OP(0xA1) // RES 4, C
    RES(4, BC.B.l);
END;

OP(0xA2) // RES 4, D
    RES(4, DE.B.h);
END;

OP(0xA3) // RES 4, E
    RES(4, DE.B.l);
END;

OP(0xA4) // RES 4, H
    RES(4, HL.B.h);
END;

OP(0xA5) // RES 4, L
    RES(4, HL.B.l);
END;

OP(0xA7) // RES 4, A
    RES(4, AF.B.h);
END;

OP(0xA6) // RES 4, (HL)
    J.B.h = READ(HL.W);
    RES(4, J.B.l);
    WRITE(HL.W, J.B.h);
END;


OP(0xB0) // RES 6, B
    RES(6, BC.B.h);
END;

OP(0xB1) // RES 6, C
    RES(6, BC.B.l);
END;

OP(0xB2) // RES 6, D
    RES(6, DE.B.h);
END;

OP(0xB3) // RES 6, E
    RES(6, DE.B.l);
END;

OP(0xB4) // RES 6, H
    RES(6, HL.B.h);
END;

OP(0xB5) // RES 6, L
    RES(6, HL.B.l);
END;

OP(0xB7) // RES 6, A
    RES(6, AF.B.h);
END;

OP(0xB6) // RES 6, (HL)
    J.B.h = READ(HL.W);
    RES(6, J.B.l);
    WRITE(HL.W, J.B.h);
END;


OP(0x88) // RES 1, B
    RES(1, BC.B.h);
END;

OP(0x89) // RES 1, C
    RES(1, BC.B.l);
END;

OP(0x8A) // RES 1, D
    RES(1, DE.B.h);
END;

OP(0x8B) // RES 1, E
    RES(1, DE.B.l);
END;

OP(0x8C) // RES 1, H
    RES(1, HL.B.h);
END;

OP(0x8D) // RES 1, L
    RES(1, HL.B.l);
END;

OP(0x8E) // RES 1, A
    RES(1, AF.B.h);
END;

OP(0x8F) // RES 1, (HL)
    J.B.h = READ(HL.W);
    RES(1, J.B.l);
    WRITE(HL.W, J.B.h);
END;



OP(0x98) // RES 3, B
    RES(3, BC.B.h);
END;

OP(0x99) // RES 3, C
    RES(3, BC.B.l);
END;

OP(0x9A) // RES 3, D
    RES(3, DE.B.h);
END;

OP(0x9B) // RES 3, E
    RES(3, DE.B.l);
END;

OP(0x9C) // RES 3, H
    RES(3, HL.B.h);
END;

OP(0x9D) // RES 3, L
    RES(3, HL.B.l);
END;

OP(0x9E) // RES 3, A
    RES(3, AF.B.h);
END;

OP(0x9F) // RES 3, (HL)
    J.B.h = READ(HL.W);
    RES(3, J.B.l);
    WRITE(HL.W, J.B.h);
END;



OP(0xA8) // RES 5, B
    RES(5, BC.B.h);
END;

OP(0xA9) // RES 5, C
    RES(5, BC.B.l);
END;

OP(0xAA) // RES 5, D
    RES(5, DE.B.h);
END;

OP(0xAB) // RES 5, E
    RES(5, DE.B.l);
END;

OP(0xAC) // RES 5, H
    RES(5, HL.B.h);
END;

OP(0xAD) // RES 5, L
    RES(5, HL.B.l);
END;

OP(0xAE) // RES 5, A
    RES(5, AF.B.h);
END;

OP(0xAF) // RES 5, (HL)
    J.B.h = READ(HL.W);
    RES(5, J.B.l);
    WRITE(HL.W, J.B.h);
END;



OP(0xB8) // RES 7, B
    RES(7, BC.B.h);
END;

OP(0xB9) // RES 7, C
    RES(7, BC.B.l);
END;

OP(0xBA) // RES 7, D
    RES(7, DE.B.h);
END;

OP(0xBB) // RES 7, E
    RES(7, DE.B.l);
END;

OP(0xBC) // RES 7, H
    RES(7, HL.B.h);
END;

OP(0xBD) // RES 7, L
    RES(7, HL.B.l);
END;

OP(0xBE) // RES 7, A
    RES(7, AF.B.h);
END;

OP(0xBF) // RES 7, (HL)
    J.B.h = READ(HL.W);
    RES(7, J.B.l);
    WRITE(HL.W, J.B.h);
END;



OP(0xC0) // SET 0, B
    SET(0, BC.B.h);
END;

OP(0xC1) // SET 0, C
    SET(0, BC.B.l);
END;

OP(0xC2) // SET 0, D
    SET(0, DE.B.h);
END;

OP(0xC3) // SET 0, E
    SET(0, DE.B.l);
END;

OP(0xC4) // SET 0, H
    SET(0, HL.B.h);
END;

OP(0xC5) // SET 0, L
    SET(0, HL.B.l);
END;

OP(0xC7) // SET 0, A
    SET(0, AF.B.h);
END;

OP(0xC6) // SET 0, (HL)
    J.B.h = READ(HL.W);
    SET(0, J.B.l);
    WRITE(HL.W, J.B.h);
END;


OP(0xD0) // SET 2, B
    SET(2, BC.B.h);
END;

OP(0xD1) // SET 2, C
    SET(2, BC.B.l);
END;

OP(0xD2) // SET 2, D
    SET(2, DE.B.h);
END;

OP(0xD3) // SET 2, E
    SET(2, DE.B.l);
END;

OP(0xD4) // SET 2, H
    SET(2, HL.B.h);
END;

OP(0xD5) // SET 2, L
    SET(2, HL.B.l);
END;

OP(0xD7) // SET 2, A
    SET(2, AF.B.h);
END;

OP(0xD6) // SET 2, (HL)
    J.B.h = READ(HL.W);
    SET(2, J.B.l);
    WRITE(HL.W, J.B.h);
END;


OP(0XE0) // SET 4, B
    SET(4, BC.B.h);
END;

OP(0XE1) // SET 4, C
    SET(4, BC.B.l);
END;

OP(0XE2) // SET 4, D
    SET(4, DE.B.h);
END;

OP(0XE3) // SET 4, E
    SET(4, DE.B.l);
END;

OP(0XE4) // SET 4, H
    SET(4, HL.B.h);
END;

OP(0XE5) // SET 4, L
    SET(4, HL.B.l);
END;

OP(0XE7) // SET 4, A
    SET(4, AF.B.h);
END;

OP(0XE6) // SET 4, (HL)
    J.B.h = READ(HL.W);
    SET(4, J.B.l);
    WRITE(HL.W, J.B.h);
END;


OP(0xF0) // SET 6, B
    SET(6, BC.B.h);
END;

OP(0xF1) // SET 6, C
    SET(6, BC.B.l);
END;

OP(0xF2) // SET 6, D
    SET(6, DE.B.h);
END;

OP(0xF3) // SET 6, E
    SET(6, DE.B.l);
END;

OP(0xF4) // SET 6, H
    SET(6, HL.B.h);
END;

OP(0xF5) // SET 6, L
    SET(6, HL.B.l);
END;

OP(0xF7) // SET 6, A
    SET(6, AF.B.h);
END;

OP(0xF6) // SET 6, (HL)
    J.B.h = READ(HL.W);
    SET(6, J.B.l);
    WRITE(HL.W, J.B.h);
END;


OP(0xC8) // SET 1, B
    SET(1, BC.B.h);
END;

OP(0xC9) // SET 1, C
    SET(1, BC.B.l);
END;

OP(0xCA) // SET 1, D
    SET(1, DE.B.h);
END;

OP(0xCB) // SET 1, E
    SET(1, DE.B.l);
END;

OP(0xCC) // SET 1, H
    SET(1, HL.B.h);
END;

OP(0xCD) // SET 1, L
    SET(1, HL.B.l);
END;

OP(0xCE) // SET 1, A
    SET(1, AF.B.h);
END;

OP(0xCF) // SET 1, (HL)
    J.B.h = READ(HL.W);
    SET(1, J.B.l);
    WRITE(HL.W, J.B.h);
END;



OP(0xD8) // SET 3, B
    SET(3, BC.B.h);
END;

OP(0xD9) // SET 3, C
    SET(3, BC.B.l);
END;

OP(0xDA) // SET 3, D
    SET(3, DE.B.h);
END;

OP(0xDB) // SET 3, E
    SET(3, DE.B.l);
END;

OP(0xDC) // SET 3, H
    SET(3, HL.B.h);
END;

OP(0xDD) // SET 3, L
    SET(3, HL.B.l);
END;

OP(0xDE) // SET 3, A
    SET(3, AF.B.h);
END;

OP(0xDF) // SET 3, (HL)
    J.B.h = READ(HL.W);
    SET(3, J.B.l);
    WRITE(HL.W, J.B.h);
END;



OP(0XE8) // SET 5, B
    SET(5, BC.B.h);
END;

OP(0XE9) // SET 5, C
    SET(5, BC.B.l);
END;

OP(0XEA) // SET 5, D
    SET(5, DE.B.h);
END;

OP(0XEB) // SET 5, E
    SET(5, DE.B.l);
END;

OP(0XEC) // SET 5, H
    SET(5, HL.B.h);
END;

OP(0XED) // SET 5, L
    SET(5, HL.B.l);
END;

OP(0XEE) // SET 5, A
    SET(5, AF.B.h);
END;

OP(0XEF) // SET 5, (HL)
    J.B.h = READ(HL.W);
    SET(5, J.B.l);
    WRITE(HL.W, J.B.h);
END;



OP(0xF8) // SET 7, B
    SET(7, BC.B.h);
END;

OP(0xF9) // SET 7, C
    SET(7, BC.B.l);
END;

OP(0xFA) // SET 7, D
    SET(7, DE.B.h);
END;

OP(0xFB) // SET 7, E
    SET(7, DE.B.l);
END;

OP(0xFC) // SET 7, H
    SET(7, HL.B.h);
END;

OP(0xFD) // SET 7, L
    SET(7, HL.B.l);
END;

OP(0xFE) // SET 7, A
    SET(7, AF.B.h);
END;

OP(0xFF) // SET 7, (HL)
    J.B.h = READ(HL.W);
    SET(7, J.B.l);
    WRITE(HL.W, J.B.h);
END;

#undef OP
#undef END
#endif
