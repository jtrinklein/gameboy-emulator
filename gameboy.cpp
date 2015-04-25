#define DEBUG

#include "EMULib/GBCarts.h"
#include "gameboy.h"
#include "render.h"
#include "cycles.h"
#include <stdio.h>
#include "bios.h"

#define STRICT_ROMCHECK 0

#ifdef DEBUG
#include "opnames.h"
#define OPNAME(x) OpNameTable[x]
#define DPRINT(x) printf(x); printf("\n");
#define DPRINT_ARG(x,i) printf(x,i); printf("\n");
#else
#define OPNAME(x)
#define DPRINT(x)
#define DPRINT_ARG(x,i)
#endif

#define INLINE static inline

#include "cart.h"

INLINE byte readMem(CPU* cpu, word A)        {
    word addr = A;
    switch (addr&0xF000){
        case 0x0000://BIOS
            if (cpu->inBios){
                return cpu->BIOS[addr];
            }
            return cpu->ROM[addr];
            break;
        case 0x1000:
        case 0x2000:
        case 0x3000:// ROM 0
            return cpu->ROM[addr];
            break;
        case 0x4000:
        case 0x5000:
        case 0x6000:
        case 0x7000:// ROM 1
            return cpu->ROM[addr];
            break;
        case 0x8000:
        case 0x9000://VRAM
            return cpu->VRAM[addr & 0x1FFF];
            break;
        case 0xA000:
        case 0xB000://External RAM
            return cpu->ERAM[addr & 0x1FFF];
            break;
        case 0xC000:
        case 0xD000://Working RAM
        case 0xE000://Working RAM (shadow)
            return cpu->WRAM[addr & 0x1FFF];
            break;
        case 0xF000://Working RAM (shadow), IO, Zero-page RAM
            switch(addr&0x0F00) {
                case 0xF00: // Zero-page RAM
                    if (addr >= 0xFF80) {
                        return cpu->ZRAM[addr & 0x7F];
                    }
                    else {
                        switch (addr & 0x00FF) {
                            case 0x40: // LCD Control
                                return cpu->gpu->getLCD();
                                break;
                            case 0x42: //scx
                                return cpu->gpu->SCX;
                                break;
                            case 0x43: // scy
                                return cpu->gpu->SCY;
                                break;
                            case 0x44: // scanline
                                return cpu->gpu->scanline;
                                break;
                        }
                    }
                    // 0xFF00 - 0xFF7F is IO, not handled yet
                    return 0;
                    break;
                case 0xE00: // OAM
                    if(addr < 0xFEA0) {
                        return cpu->OAM[addr & 0xFF];
                    }
                    return 0;
                    break;
                default: // Working RAM (shadow)
                    return cpu->WRAM[addr&0x1FFF];
                    break;
            }
            break;
            
    }
    return 0;
}
INLINE void writeMem(CPU* cpu, word A,byte V) {
    word addr = A;
    switch (addr&0xF000){
        case 0x0000://BIOS
            if (cpu->inBios){
                cpu->BIOS[addr] = V;
            }
            cpu->ROM[addr] = V;
            break;
        case 0x1000:
        case 0x2000:
        case 0x3000:// ROM 0
            cpu->ROM[addr] = V;
            break;
        case 0x4000:
        case 0x5000:
        case 0x6000:
        case 0x7000:// ROM 1
            cpu->ROM[addr] = V;
            break;
        case 0x8000:
        case 0x9000://VRAM
            cpu->VRAM[addr & 0x1FFF] = V;
            break;
        case 0xA000:
        case 0xB000://External RAM
            cpu->ERAM[addr & 0x1FFF] = V;
            break;
        case 0xC000:
        case 0xD000://Working RAM
        case 0xE000://Working RAM (shadow)
            cpu->WRAM[addr & 0x1FFF] = V;
            break;
        case 0xF000://Working RAM (shadow), IO, Zero-page RAM
            switch(addr&0x0F00) {
                case 0xF00: // Zero-page RAM
                    if (addr >= 0xFF80) {
                        cpu->ZRAM[addr & 0x7F] = V;
                    }
                    else {
                        switch (addr & 0x00FF) {
                            case 0x40: // LCD Control
                                cpu->gpu->setLCD(V);
                                break;
                            case 0x42: //scx
                                cpu->gpu->SCX = V;
                                break;
                            case 0x43: // scy
                                cpu->gpu->SCY = V;
                                break;
                            case 0x47: // scanline
                                cpu->gpu->setPalette(V);
                                break;
                        }
                    }
                    // 0xFF00 - 0xFF7F is IO, not handled yet

                    break;
                case 0xE00: // OAM
                    if(addr < 0xFEA0) {
                        cpu->OAM[addr & 0xFF] = V;
                    }
                    break;
                default: // Working RAM (shadow)
                    cpu->WRAM[addr&0x1FFF] = V;
                    break;
            }
            break;
            
    }
}

#define READ(a) readMem(cpu, a)
#define READ_INC() readMem(cpu, cpu->PC.W++)
#define READ_LAST() readMem(cpu, cpu->PC.W-1)
#define WRITE(a,v) writeMem(cpu, a, v)
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
#define POP(r) r.B.h = READ(++(cpu->SP.W)); r.B.l = READ(++(cpu->SP.W))
#define PUSH(r) WRITE(cpu->SP.W--, r.B.l); WRITE(cpu->SP.W--, r.B.h)
#define RET() POP(J); cpu->PC.W = J.W
#define ILLEGAL(op) printf("Illegal opcode: 0x%02X", op)
#define SET_Z(x)\
    cpu->AF.B.l |= x ? 0:FLAG_Z

#define RLC_X(x)\
    cpu->AF.B.l = (x&0x80)?FLAG_C:0;\
    x = (x<<1)|((cpu->AF.B.l&FLAG_C)?0x01:0)
#define RLC_X_Z(x)\
    RLC_X(x);\
    SET_Z(x)

#define RL_X(x)\
    I = (x & 0x80)?FLAG_C:0;\
    x = (x<<1)|((cpu->AF.B.l&FLAG_C)?0x01:0);\
    cpu->AF.B.l = I
#define RL_X_Z(x)\
    RL_X(x);\
    SET_Z(x)

#define RRC_X(x)\
    cpu->AF.B.l = (x&0x01)?FLAG_C:0;\
    x = (x>>1)|((cpu->AF.B.l&FLAG_C)?0x80:0)
#define RRC_X_Z(x)\
    RRC_X(x);\
    SET_Z(x)

#define RR_X(x)\
    I = (x & 0x01)?FLAG_C:0;\
    x = (x>>1)|((cpu->AF.B.l&FLAG_C)?0x80:0);\
    cpu->AF.B.l = I
#define RR_X_Z(x)\
    RR_X(x);\
    SET_Z(x)

#define SLA_X(x)\
    cpu->AF.B.l = (x & 0x80)?FLAG_C:0;\
    x <<= 1;\
    SET_Z(x)

#define SRA_X(x)\
    x = (x>>1)|(x&0x80);\
    cpu->AF.B.l = x?0:FLAG_Z

#define SRL_X(x)\
    x >>= 1;\
    cpu->AF.B.l = x?0:FLAG_Z

#define SWAP_X(x)\
    x = (x>>4)|(x<<4);\
    cpu->AF.B.l = x?0:FLAG_Z

#define BIT(n,x)\
    cpu->AF.B.l = ((x&(1<<n))?0:FLAG_Z)|FLAG_H|(cpu->AF.B.l&FLAG_C)

#define RES(n,x)\
    x &= ~(1<<n)

#define SET(n,x)\
    x |= 1<<n

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

byte getOp(CPU* cpu, word addr) {
    return readMem(cpu, addr);
}

void interrupt(CPU* cpu, word vector) {
    cpu->IF &= ~(IF_HALT); // clear HALT flag
    if (vector == INT_RST00) {
        cpu->PC.W = 0x00;
    }
}

void ExecCB(CPU* cpu) {
    byte I;
    pair J;
    I = READ_INC(); // getOp

    cpu->ICycles -= cyclesCB[I];
    cpu->opCycles += cyclesCB[I];

    switch(I) {
#include "opcodeCB.h"
        default:
            printf("[GameBoy] Unrecognized instruction: %02X at PC=%04X\n", READ(cpu->PC.W-1), cpu->PC.W-1);
            break;
    }
}

bool execOp(CPU* cpu, Render* gpu, byte I) {
    pair J;
    DPRINT_ARG("instruction: %02X", I);
    DPRINT_ARG("name: %s",OPNAME(I));
    cpu->ICycles -= cycles[I];
    cpu->opCycles = cycles[I];
    switch(I) {
#include "opcode.h"
        default:
            printf("[GameBoy] Unrecognized instruction: %02X at PC=%04X\n", READ(cpu->PC.W-1), cpu->PC.W-1);
            break;
    }
    
    gpu->renderStep(cpu->opCycles);
    
    if(cpu->ICycles <=0) {
        cpu->ICycles += cpu->IPeriod;
    }

    return true;
}
void runGB(CPU* cpu, Render* gpu) {
    byte I; // instruction
    pair J;
    for(;gpu->running();) {
        
        I = READ_INC(); //getop
        if(!execOp(cpu, gpu, I)) {
            return;
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

void doBios(CPU* cpu, Render* gpu) {

    
    while (cpu->PC.W < 0x100) {
        execOp(cpu, gpu, READ_INC());
    }
}

void go() {
    const char *file = "/Users/jtrinklein/gameboy/emulator/roms/cpu_instrs/cpu_instrs.gb";
//    const char *file = "/Users/jtrinklein/gameboy/emulator/roms/instr_timing/instr_timing.gb";
    DPRINT_ARG("loading rom: %s", file);
    
    
    CPU* cpu = new CPU();
    cpu->OAM  = new byte[0x0100];
    cpu->ZRAM = new byte[0x0100];
    cpu->WRAM = new byte[0x2000];
    cpu->ERAM = new byte[0x2000];
    cpu->VRAM = new byte[0x2000];
    cpu->ROM = loadRom(file);
    
    cpu->gpu = new Render(cpu->VRAM, cpu->OAM);
    initCPU(cpu);
    DPRINT("check graphic");
    bool noGraphicRequired = true;
    bool ok = checkNintendoGraphic(cpu->ROM);
    if(noGraphicRequired || ok) {
        DPRINT("starting cpu");
        runGB(cpu,cpu->gpu);
    } else {
        DPRINT("check failed");
    }
    DPRINT("cleanup");

    delete [] cpu->ROM;
    delete [] cpu->OAM;
    delete [] cpu->WRAM;
    delete [] cpu->ZRAM;
    delete [] cpu->ERAM;
    delete [] cpu->VRAM;
    delete cpu->gpu;
    delete cpu;
}
