#define PRINTOPS 0
#define LOGGING 1

#include "GBCarts.h"
#include "gameboy.h"
#include "render.h"
#include "cycles.h"
#include <stdio.h>
#include "bios.h"
#include "mmu.h"

#define STRICT_ROMCHECK 0

#if LOGGING
#define LOG_ERR(x)  printf("ERROR: "); printf(x); printf("\n")
#define LOG_WARN(x) printf("WARN : "); printf(x); printf("\n")
#define LOG_INFO(x) printf("INFO : "); printf(x); printf("\n")
#else
#define LOG_ERR(x)  /**/
#define LOG_WARN(x) /**/
#define LOG_INFO(x) /**/
#endif

#include "opnames.h"
#if PRINTOPS
#define OPNAME(x) OpNameTable[x]
#define DPRINT(x) printf(x); printf("\n");
#define DPRINT_ARG(x,i) printf(x,i); printf("\n");
#else
#define OPNAME(x) /**/
#define DPRINT(x) /**/
#define DPRINT_ARG(x,i) /**/
#endif

#include "cart.h"


byte readMem(CPU* cpu, word A) {
    word addr = A;
    if ((addr & 0xF000) == 0xF000) {
        switch(addr&0x0F00) {
            case 0xF00: // Zero-page RAM
                if (addr == 0xFFFF) {
                    return cpu->IE;
                } else if (addr >= 0xFF80) {
                    return cpu->mmu->read(addr);
                }
                else {
                    switch (addr & 0x00FF) {
                        case 0x0F: // Interrupt Flags
                            return cpu->IF;
                        case 0x40: // LCD Control
                            return cpu->gpu->getLCD();
                        case 0x42: //scx
                            return cpu->gpu->SCX;
                        case 0x43: // scy
                            return cpu->gpu->SCY;
                        case 0x44: // scanline
                            return cpu->gpu->scanline;
                    }
                }
                // 0xFF00 - 0xFF7F is IO, not handled yet
                return 0;
                break;
            case 0xE00: // OAM
                if(addr < 0xFEA0) {
                    return cpu->mmu->read(addr);
                }
                return 0;
            default: // Working RAM (shadow)
                return cpu->mmu->read(addr);
        }

    }
    
    return cpu->mmu->read(addr);
}

void writeMem(CPU* cpu, word A,byte V) {
    word addr = A;
    if ((addr & 0xF000) == 0xF000) { //Working RAM (shadow), IO, Zero-page RAM

        switch(addr&0x0F00) {
            case 0xF00: // Zero-page RAM
                if (addr >= 0xFF80) {
                    cpu->mmu->write(addr, V);
                }
                else {
                    switch (addr & 0x00FF) {
                        case 0x40: // LCD Control
                            cpu->gpu->setLCD(V);
                            break;
                        case 0x43: //scx
                            cpu->gpu->SCX = V;
                            break;
                        case 0x42: // scy
                            cpu->gpu->SCY = V;
                            break;
                        case 0x47: // scanline
                            cpu->gpu->setPalette(V);
                            break;
                        default:
                            cpu->mmu->write(addr, V);
                            break;
                    }
                }
                // 0xFF00 - 0xFF7F is IO, not handled yet
                
                break;
            case 0xE00: // OAM
                if(addr < 0xFEA0) {
                    cpu->mmu->write(addr, V);
                }
                break;
            default: // Working RAM (shadow)
                cpu->mmu->write(addr, V);
                break;
        }
    }
    else {
        cpu->mmu->write(A, V);
    }
}

#define READ(a) readMem(cpu, a)
#define READ_INC() readMem(cpu, cpu->PC.W++)
#define READ_LAST() readMem(cpu, cpu->PC.W-1)
#define WRITE(a,v) writeMem(cpu, a, v)
#define SET_INC_FLAGS(v) cpu->AF.B.l=((v==0?FLAG_Z:0)|((v&0xF)==0?FLAG_H:0)|(cpu->AF.B.l&FLAG_C))
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
#define POP(r) r.B.l = READ(++(cpu->SP.W)); r.B.h = READ(++(cpu->SP.W))
#define PUSH(r) WRITE(cpu->SP.W--, r.B.h); WRITE(cpu->SP.W--, r.B.l)
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

void CPU::reset() {
    PC.W = 0x0000;
    SP.W = 0xF000;
    AF.W = 0x0000;
    BC.W = 0x0000;
    DE.W = 0x0000;
    HL.W = 0x0000;
    ICycles = IPeriod;
    IRequest = INT_NONE;
}

CPU::CPU() {
    
}
CPU::~CPU() {
    
}

byte getOp(CPU* cpu, word addr) {
    return readMem(cpu, addr);
}

void interrupt(CPU* cpu, byte vector) {
    cpu->runState &= ~(RS_HALT); // clear HALT flag
    if (vector == INT_RST00) {
        cpu->PC.W = 0x0000;
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

bool execOp(CPU* cpu, byte I) {
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

    return true;
}

void runGB(CPU* cpu) {
    byte I; // instruction
    for(;cpu->gpu->running();) {

        word pc = cpu->PC.W;
        I = READ_INC(); //getop
        
        //herp = cpu->BC.B.l;
        if(!execOp(cpu, I)) {
            return;
        }

        cpu->gpu->renderStep(cpu->opCycles);

        if(cpu->ICycles <=0) {
            cpu->ICycles += cpu->IPeriod;
        }

    }
}

void initCPU(CPU* cpu) {
    cpu->init();
}
void CPU::init() {
    //cpu->inBios = 0x00;
    PC.W = 0x100;
    AF.B.h = 0x01;
    AF.B.l = 0xB0;
    BC.W = 0x0013;
    DE.W = 0x00D8;
    HL.W = 0x014D;
    SP.W = 0xFFFE;
    
    CPU* cpu =this;
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

void cleanup(CPU* cpu) {
    LOG_INFO("cleanup");
    delete cpu->mmu;
    delete cpu->gpu;
    delete cpu;
}

void go() {
//    const char *file = "/Users/jtrinklein/src/gameboy/emulator/roms/cpu_instrs/cpu_instrs.gb";
    const char *file = "/Users/jtrinklein/src/gameboy/emulator/roms/cpu_instrs/individual/01-special.gb";
//    const char *file = "/Users/jtrinklein/src/gameboy/emulator/roms/cpu_instrs/individual/03-op sp,hl.gb";
//    const char *file = "/Users/jtrinklein/gameboy/emulator/roms/instr_timing/instr_timing.gb";
    LOG_INFO("loading rom:");
    LOG_INFO(file);


    CPU* cpu = new CPU();
    cpu->mmu = new MMU();
    cpu->gpu = new Render(cpu->mmu);
    initCPU(cpu);
    cpu->mmu->loadCart(file);

    LOG_INFO("check graphic");
    if(!cpu->mmu->cart->isLogoValid) {
        if(STRICT_ROMCHECK) {
            LOG_ERR("Nintendo Logo is corrupted!");
            cleanup(cpu);
        } else {
            LOG_WARN("Nintendo Logo is corrupted! This would fail normal hardware");
        }
    }
    LOG_INFO("starting cpu");
    
    runGB(cpu);
    
    cleanup(cpu);
}
