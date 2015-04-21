#include "EMULib/GBCarts.h"
#include "gameboy.h"
#include "render.h"
#include "cycles.h"
#include <stdio.h>
#define DPRINT(x) printf(x); printf("\n");
#define DPRINT_ARG(x,i) printf(x,i); printf("\n");
#define INLINE static inline

#include "cart.h"

byte *Page,*ROM;

INLINE byte readMem(word A)        { return ROM[A]; }
INLINE void writeMem(word A,byte V) { ROM[A]=V; }
#define READ(a) readMem(a)
#define READ_INC() readMem(cpu->PC.W++)
#define READ_LAST() readMem(cpu->PC.W-1)
#define WRITE(a,v) writeMem(a, v)
#define SET_INC_FLAGS(v) cpu->AF.B.l=((v==0?FLAG_Z:0)|((v&0xF)==0x0?0:FLAG_H)|(cpu->AF.B.l&FLAG_C))
#define SET_DEC_FLAGS(v) cpu->AF.B.l=((v==0?FLAG_Z:0)|((v&0xF)==0xF?FLAG_H:0)|(cpu->AF.B.l&FLAG_C)|FLAG_N)
#define ADD_WORDS(w1,w2) J.W = (w1 + w2) & 0xFFFF;\
                        cpu->AF.B.l = (\
                            (cpu->AF.B.l&FLAG_Z)|\
                            (((long)w1+(long)w2)&0x10000?FLAG_C:0)|\
                            ((w1 & 0xFFF)>(J.W & 0xFFF)?FLAG_H:0)\
                        )
#define ADD_A(v) I = v;\
                J.W = cpu->AF.B.h + I;\
                cpu->AF.B.l = ((J.B.l)?0:FLAG_Z)|(((J.B.l&0x08) && (I&0x07))?FLAG_H:0)|(((J.B.h&0x01)&&(I))?FLAG_C:0);\
                cpu->AF.B.h = J.B.l

#define ADC_A(v) ADD_A(((cpu->AF.B.l & FLAG_C) ? 0x01 : 0) + v)

#define CP_A(v) I = v;\
                J.W = cpu->AF.B.h - I;\
                cpu->AF.B.l = FLAG_N|\
                              (((cpu->AF.B.h&0x07) >= (I&0x07))?FLAG_H:0)|\
                              ((cpu->AF.B.h >= I)?FLAG_C:0)|\
                              ((J.B.l)?0:FLAG_Z)
#define SUB_A(v) CP_A(v);\
                cpu->AF.B.h = J.B.l
#define SBC_A(v) SUB_A((((cpu->AF.B.l & FLAG_C)?0x01:0)+v))
#define AND_A(v) cpu->AF.B.h = cpu->AF.B.h & v; cpu->AF.B.l = FLAG_H|(cpu->AF.B.h?0:FLAG_Z)
#define OR_A(v) cpu->AF.B.h = cpu->AF.B.h | v; cpu->AF.B.l = (cpu->AF.B.h?0:FLAG_Z)
#define XOR_A(v) cpu->AF.B.h = cpu->AF.B.h ^ v; cpu->AF.B.l = (cpu->AF.B.h?0:FLAG_Z)
#define POP(r) r.B.h = READ(cpu->SP.W++); r.B.l = READ(cpu->SP.W++)
#define PUSH(r) WRITE(cpu->SP.W--, r.B.l); WRITE(cpu->SP.W--, r.B.h)
#define RET() POP(J); cpu->PC.W = J.W
#define ILLEGAL(op) printf("Illegal opcode: 0x%02X", op)

void reset(CPU* cpu) {
    cpu->PC.W = 0x0000;
    cpu->SP.W = 0xF000;
    cpu->AF.W = 0x0000;
    cpu->BC.W = 0x0000;
    cpu->DE.W = 0x0000;
    cpu->HL.W = 0x0000;
    cpu->ICycles = cpu->IPeriod;
    cpu->IRequest = INT_NONE;
}

byte getOp(word addr) {
    return readMem(addr);
}

void interrupt(CPU* cpu, word vector) {
    cpu->IF &= ~(IF_HALT); // clear HALT flag
    if (vector == INT_RST00) {
        cpu->PC.W = 0x00;
    }
}

void ExecCB(CPU* cpu) {
    byte I;
    //pair J;
    I = getOp(cpu->PC.W++);

    cpu->ICycles -= cyclesCB[I];

    switch(I) {
#include "opcodeCB.h"
        default:
            printf("[GameBoy %lX] Unrecognized instruction: %02X at PC=%04X\n", (long)cpu->User, getOp(cpu->PC.W-1), cpu->PC.W-1);
            break;
    }

    if(cpu->ICycles <= 0) {
        cpu->ICycles += cpu->IPeriod;
        if(cpu->IRequest != INT_NONE && cpu->IRequest !=INT_QUIT) {
            interrupt(cpu, cpu->IRequest);
        }
        else if (cpu->IRequest == INT_QUIT) {
            return;
        }
    }
}
#include "opnames.h"
#define OPNAME(x) OpNameTable[x]
void runGB(CPU* cpu, Render* gpu) {
    byte I; // instruction
    pair J;

    for(;gpu->device->run() && gpu->device;) {
        I = getOp(cpu->PC.W++);

        DPRINT_ARG("instruction: %02X", I);
        DPRINT_ARG("name: %s",OPNAME(I));
        cpu->ICycles -= cycles[I];

        switch(I) {
#include "opcode.h"
            default:
                printf("[GameBoy %lX] Unrecognized instruction: %02X at PC=%04X\n", (long)cpu->User, getOp(cpu->PC.W-1), cpu->PC.W-1);
                break;
        }

        if(cpu->ICycles <= 0) {
            cpu->ICycles += cpu->IPeriod;
            gpu->renderStep();
            if(cpu->IRequest != INT_NONE && cpu->IRequest !=INT_QUIT) {
                interrupt(cpu, cpu->IRequest);
            }
            else if (cpu->IRequest == INT_QUIT) {
                return;
            }
        }
    }
}

void initCPU(CPU* cpu) {
    cpu->PC.W = 0x100;
    cpu->AF.B.h = 0x01;
    cpu->AF.B.l = 0xB0;
    cpu->BC.W = 0x0013;
    cpu->DE.W = 0x00D8;
    cpu->HL.W = 0x014D;
    cpu->SP.W = 0xFFFE;
    ROM[0xFF05] = 0x00 ;// TIMA
    ROM[0xFF06] = 0x00 ;// TMA
    ROM[0xFF07] = 0x00 ;// TAC
    ROM[0xFF10] = 0x80 ;// NR10
    ROM[0xFF11] = 0xBF ;// NR11
    ROM[0xFF12] = 0xF3 ;// NR12
    ROM[0xFF14] = 0xBF ;// NR14
    ROM[0xFF16] = 0x3F ;// NR21
    ROM[0xFF17] = 0x00 ;// NR22
    ROM[0xFF19] = 0xBF ;// NR24
    ROM[0xFF1A] = 0x7F ;// NR30
    ROM[0xFF1B] = 0xFF ;// NR31
    ROM[0xFF1C] = 0x9F ;// NR32
    ROM[0xFF1E] = 0xBF ;// NR33
    ROM[0xFF20] = 0xFF ;// NR41
    ROM[0xFF21] = 0x00 ;// NR42
    ROM[0xFF22] = 0x00 ;// NR43
    ROM[0xFF23] = 0xBF ;// NR30
    ROM[0xFF24] = 0x77 ;// NR50
    ROM[0xFF25] = 0xF3 ;// NR51
    ROM[0xFF26] = 0xF1 ;//-GB//, 0xF0-SGB ; NR52
    ROM[0xFF40] = 0x91 ;// LCDC
    ROM[0xFF42] = 0x00 ;// SCY
    ROM[0xFF43] = 0x00 ;// SCX
    ROM[0xFF45] = 0x00 ;// LYC
    ROM[0xFF47] = 0xFC ;// BGP
    ROM[0xFF48] = 0xFF ;// OBP0
    ROM[0xFF49] = 0xFF ;// OBP1
    ROM[0xFF4A] = 0x00 ;// WY
    ROM[0xFF4B] = 0x00 ;// WX
    ROM[0xFFFF] = 0x00 ;// IE
}

void go() {
//    const char *file = "/Users/jtrinklein/gameboy/emulator/roms/cpu_instrs/cpu_instrs.gb";
    const char *file = "/Users/jtrinklein/gameboy/emulator/roms/instr_timing/instr_timing.gb";
    DPRINT_ARG("loading rom: %s", file);

    CPU* cpu = new CPU();
    Page = new byte[0xFFFF];
    ROM = loadRom(file);
    initCPU(cpu);
    Render* gpu = new Render(ROM);
    DPRINT("check graphic");
    bool noGraphicRequired = true;
    bool ok = checkNintendoGraphic(ROM);
    if(noGraphicRequired || ok) {
        DPRINT("starting cpu");
        runGB(cpu,gpu);
    } else {
        DPRINT("check failed");
    }
    DPRINT("cleanup");

    delete [] ROM;
    delete [] Page;
    delete cpu;
}
