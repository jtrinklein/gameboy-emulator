#include <iostream>
#include "cpu.h"
#include "mmu.h"
#include "render.h"
#include "gameboy.h"
#include "cycles.h"

#define READ_INC() gb->mmu->read(PC.W++)

#define WRITE(addr,v) gb->mmu->write(addr,v)

#define READ(A) gb->mmu->read(A)

#define SET_INC_FLAGS(v) AF.B.l=((v==0?FLAG_Z:0)|((v&0xF)==0?FLAG_H:0)|(AF.B.l&FLAG_C))

#define SET_DEC_FLAGS(v) AF.B.l=((v==0?FLAG_Z:0)|((v&0xF)==0xF?FLAG_H:0)|(AF.B.l&FLAG_C)|FLAG_N)

#define ADD_WORDS(w1,w2) J.W = (w1 + w2) & 0xFFFF;\
                        AF.B.l = (\
                            (AF.B.l&FLAG_Z)|\
                            (((long)w1+(long)w2)&0x10000?FLAG_C:0)|\
                            ((w1 & 0xFFF)>(J.W & 0xFFF)?FLAG_H:0)\
                        )

#define ADD_A(v) I = v;\
                J.W = AF.B.h + I;\
                AF.B.l = ((J.B.l)?0:FLAG_Z)|(((J.B.l&0x08) && (I&0x07))?FLAG_H:0)|(((J.B.h&0x01)&&(I))?FLAG_C:0);\
                AF.B.h = J.B.l

#define ADC_A(v) ADD_A(((AF.B.l & FLAG_C) ? 0x01 : 0) + v)

#define CP_A(v) I = v;\
                J.W = AF.B.h - I;\
                AF.B.l = FLAG_N|\
                              (((AF.B.h&0x07) >= (I&0x07))?FLAG_H:0)|\
                              ((AF.B.h >= I)?FLAG_C:0)|\
                              ((J.B.l)?0:FLAG_Z)

#define SUB_A(v) CP_A(v);\
                AF.B.h = J.B.l

#define SBC_A(v) SUB_A((((AF.B.l & FLAG_C)?0x01:0)+v))

#define AND_A(v) AF.B.h = AF.B.h & v; AF.B.l = FLAG_H|(AF.B.h?0:FLAG_Z)

#define OR_A(v) AF.B.h = AF.B.h | v; AF.B.l = (AF.B.h?0:FLAG_Z)

#define XOR_A(v) AF.B.h = AF.B.h ^ v; AF.B.l = (AF.B.h?0:FLAG_Z)

#define POP(r) r.B.l = READ(++(SP.W)); r.B.h = READ(++(SP.W))

#define PUSH(r) WRITE(SP.W--, r.B.h); WRITE(SP.W--, r.B.l)

#define RET() POP(J); PC.W = J.W

#define ILLEGAL(op) std::cout << "Illegal opcode: 0x" << std::hex << op << std::endl

#define SET_Z(x)\
    AF.B.l |= x ? 0:FLAG_Z

#define RLC_X(x)\
    AF.B.l = (x&0x80)?FLAG_C:0;\
    x = (x<<1)|((AF.B.l&FLAG_C)?0x01:0)

#define RLC_X_Z(x)\
    RLC_X(x);\
    SET_Z(x)

#define RL_X(x)\
    I = (x & 0x80)?FLAG_C:0;\
    x = (x<<1)|((AF.B.l&FLAG_C)?0x01:0);\
    AF.B.l = I

#define RL_X_Z(x)\
    RL_X(x);\
    SET_Z(x)

#define RRC_X(x)\
    AF.B.l = (x&0x01)?FLAG_C:0;\
    x = (x>>1)|((AF.B.l&FLAG_C)?0x80:0)

#define RRC_X_Z(x)\
    RRC_X(x);\
    SET_Z(x)

#define RR_X(x)\
    I = (x & 0x01)?FLAG_C:0;\
    x = (x>>1)|((AF.B.l&FLAG_C)?0x80:0);\
    AF.B.l = I

#define RR_X_Z(x)\
    RR_X(x);\
    SET_Z(x)

#define SLA_X(x)\
    AF.B.l = (x & 0x80)?FLAG_C:0;\
    x <<= 1;\
    SET_Z(x)

#define SRA_X(x)\
    x = (x>>1)|(x&0x80);\
    AF.B.l = x?0:FLAG_Z

#define SRL_X(x)\
    x >>= 1;\
    AF.B.l = x?0:FLAG_Z

#define SWAP_X(x)\
    x = (x>>4)|(x<<4);\
    AF.B.l = x?0:FLAG_Z

#define BIT(n,x)\
    AF.B.l = ((x&(1<<n))?0:FLAG_Z)|FLAG_H|(AF.B.l&FLAG_C)

#define RES(n,x) x &= ~(1<<n)

#define SET(n,x) x |= 1<<n

CPU::CPU(Gameboy *GB) {
    gb = GB;
    powerOn = true;
    runState = RS_RUN;
    intPeriod = DEFAULT_INTERRUPT_PERIOD;
}

CPU::~CPU() {

}

void CPU::reset() {
    PC.W = 0x100;
    AF.B.h = 0x01;
    AF.B.l = 0xB0;
    BC.W = 0x0013;
    DE.W = 0x00D8;
    HL.W = 0x014D;
    SP.W = 0xFFFE;

    WRITE(0xFF05, 0x00) ;// TIMA
    WRITE(0xFF06, 0x00) ;// TMA
    WRITE(0xFF07, 0x00) ;// TAC
    WRITE(0xFF10, 0x80) ;// NR10
    WRITE(0xFF11, 0xBF) ;// NR11
    WRITE(0xFF12, 0xF3) ;// NR12
    WRITE(0xFF14, 0xBF) ;// NR14
    WRITE(0xFF16, 0x3F) ;// NR21
    WRITE(0xFF17, 0x00) ;// NR22
    WRITE(0xFF19, 0xBF) ;// NR24
    WRITE(0xFF1A, 0x7F) ;// NR30
    WRITE(0xFF1B, 0xFF) ;// NR31
    WRITE(0xFF1C, 0x9F) ;// NR32
    WRITE(0xFF1E, 0xBF) ;// NR33
    WRITE(0xFF20, 0xFF) ;// NR41
    WRITE(0xFF21, 0x00) ;// NR42
    WRITE(0xFF22, 0x00) ;// NR43
    WRITE(0xFF23, 0xBF) ;// NR30
    WRITE(0xFF24, 0x77) ;// NR50
    WRITE(0xFF25, 0xF3) ;// NR51
    WRITE(0xFF26, 0xF1) ;//-GB//, 0xF0-SGB ; NR52
    WRITE(0xFF40, 0x91) ;// LCDC
    WRITE(0xFF42, 0x00) ;// SCY
    WRITE(0xFF43, 0x00) ;// SCX
    WRITE(0xFF45, 0x00) ;// LYC
    WRITE(0xFF47, 0xFC) ;// BGP
    WRITE(0xFF48, 0xFF) ;// OBP0
    WRITE(0xFF49, 0xFF) ;// OBP1
    WRITE(0xFF4A, 0x00) ;// WY
    WRITE(0xFF4B, 0x00) ;// WX
    WRITE(0xFFFF, 0x00) ;// IE
}
void CPU::execOp(byte opcode) {
    byte I = opcode;
    pair J;

    opCycles = cycles[I];
    intCycles -= opCycles;

    switch (I) {

#include "opcode.h"

        default:
            break;
    }
}

void CPU::execCB(byte opcode) {
    byte I = opcode;
    pair J;
    opCycles += cyclesCB[I];
    intCycles -= cyclesCB[I];
    switch (I) {

#include "opcodeCB.h"

        default:
            break;
    }
}
void CPU::runBios(byte *bios) {
    PC.W = 0;
    gb->mmu->inBios = true;
    //byte* rom = gb->mmu->rom;
    //gb->mmu->rom = bios;
    while (gb->gpu->running() && PC.W < 0x100) {
        step();
    }
    gb->mmu->inBios = false;
}
void CPU::run() {
    while (gb->gpu->running()) {
        if (runState == RS_STOP) {
            stepStop();
        } else if (runState == RS_HALT) {
            stepHalt();
        } else {
            step();
        }
    }
}


void CPU::timerStep() {
    static byte timerSpeed[4] = {64,1,4,16};
    byte machineCycles = opCycles >> 2;
    timerTicks += machineCycles;

    if(timerTicks >= 4) {
        timerTicks -= 4;
        timer++;
        if (gb->mmu->tac & 0x4 && timerTicks >= timerSpeed[gb->mmu->tac & 0x3]) {
            timerTicks -= timerSpeed[gb->mmu->tac & 0x3];
            if (++(gb->mmu->tima) == 0) {
                gb->mmu->tima = gb->mmu->tma;
                gb->mmu->IF |= IR_TIMER;
            }
        }

        if (++divTicks == 16) {
            divTicks = 0;
            gb->mmu->div++;
        }

    }
}
void CPU::step() {
    byte opcode =READ_INC();
    execOp(opcode);
    timerStep();
    gb->gpu->renderStep(opCycles);
}
void CPU::stepHalt() {
    byte opcode =READ_INC();
    execOp(opcode);
    timerStep();
    gb->gpu->renderStep(opCycles);
}

void CPU::stepStop() {
    byte opcode =READ_INC();
    execOp(opcode);
    timerStep();
}
