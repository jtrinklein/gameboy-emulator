#define PRINTOPS 0
#define LOGGING 1

#include "GBCarts.h"
#include "gameboy.h"
#include "render.h"
#include "cycles.h"
#include <fstream>
#include <stdio.h>
#include <string>
#include "bios.h"
#include "cart.h"
#include "mmu.h"
#include "pad.h"
#include "cpu.h"
#include "apu.h"


Gameboy::Gameboy() {
    mmu = new MMU(this);
    pad = new Pad(this);
    cpu = new CPU(this);
    gpu = new GPU(this);
    apu = new APU();
}

Gameboy::~Gameboy() {
    delete gpu;
    delete cpu;
    delete mmu;
    delete pad;
    delete apu;
}

void Gameboy::loadCart(const char* path) {
    mmu->loadCart(path);
}

#define STRICT_ROMCHECK 0

#if LOGGING
#define LOG_ERR(x)  printf("ERROR: "); printf(x); printf("\n")
#define LOG_WARN(x) printf("WARN : "); printf(x); printf("\n")
#define LOG_INFO(x) printf("INFO : %s\n", x)
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


/*
byte readMem(CPU* cpu, word A) {
    word addr = A;
    return cpu->mmu->read(addr);
}

void writeMem(CPU* cpu, word A,byte V) {
    word addr = A;
    switch(addr) {
        default:
            cpu->mmu->write(addr, V);
            break;
    }

}*/

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
/*
void CPU::reset() {
    PC.W = 0x0000;
    SP.W = 0xF000;
    AF.W = 0x0000;
    BC.W = 0x0000;
    DE.W = 0x0000;
    HL.W = 0x0000;
    ICycles = IPeriod;
    IRequest = INT_NONE;
}*/

/*
byte getOp(CPU* cpu, word addr) {
    return readMem(cpu, addr);
}

void interrupt(CPU* cpu, byte vector) {
    if (vector == INT_RST00) {
        cpu->PC.W = 0x0000;
    }
}*/
/*
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
    byte x = 0; // debugging
    word irq = 0;
    word pc;
    for(;cpu->gpu->running();) {

        pc = cpu->PC.W;
        if (irq) {
            PUSH(cpu->PC);
            cpu->PC.W = irq;
            cpu->opCycles = 12;
            cpu->ICycles -= 12;
        } else {
            if( cpu->runState == RS_HALT) {
                if( cpu->mmu->IE == IE_DISABLED) {
                    I = READ(cpu->PC.W);
                    cpu->runState &= ~(RS_HALT); // clear HALT flag
                } else {
                    I = 0x00;
                }
            } else {
                I =  READ_INC(); //getop
            }
            const char*name = OpNameTable[I]; // debugging

            if(pc > 0xc0e6) { // debugging
                x = 0;
            }
            if(I == 0x40) { //debugging
                x = 1;
            }
            if(!execOp(cpu, I)) {
                return;
            }
        }
        if(cpu->runState != RS_HALT) {
            cpu->timerStep(cpu->opCycles >> 2); // opcycles / 4
        }

        cpu->gpu->renderStep(cpu->opCycles);

        if(cpu->ICycles <=0) {
            cpu->ICycles += cpu->IPeriod;


            if (cpu->IME && cpu->IE && cpu->mmu->IF) {
                cpu->runState &= ~(RS_HALT); // clear HALT flag

                byte flagged = cpu->IE & cpu->mmu->IF;
                if (flagged &IR_VBLANK) { // priority 1
                    irq = 0x0040;
                    cpu->mmu->IF &= ~IR_VBLANK;
                    cpu->IME = 0;
                }
                if (flagged &IR_LCD) { // priority 2
                    irq = 0x0048;
                    cpu->mmu->IF &= ~IR_LCD;
                    cpu->IME = 0;
                }
                if (flagged &IR_TIMER) { // priority 3
                    irq = 0x0050;
                    cpu->mmu->IF &= ~IR_TIMER;
                    cpu->IME = 0;
                }
                if (flagged &IR_SERIAL) { // priority 4
                    irq = 0x0058;
                    cpu->mmu->IF &= ~IR_SERIAL;
                    cpu->IME = 0;
                }
                if (flagged &IR_JOYPAD) { // priority 5
                    irq = 0x0060;
                    cpu->mmu->IF &= ~IR_JOYPAD;
                    cpu->IME = 0;
                }
            }
        }

    }
}

static byte timerSpeed[4] = {64,1,4,16};
void CPU::timerStep(byte cycles) {
    ticker += cycles;
    if(ticker >= 4) {
        ticker -= 4;
        timertick++;
        if (mmu->tac & 0x4 && timertick >= timerSpeed[mmu->tac & 0x3]) {
            timertick -= timerSpeed[mmu->tac & 0x3];
            if (++mmu->tima == 0) {
                mmu->tima = mmu->tma;
                mmu->IF |= IR_TIMER;
            }
        }

        if (++divtick == 16) {
            divtick = 0;
            mmu->div++;
        }

    }

}
*/
void initCPU(Gameboy* gb) {
    gb->cpu->reset();
}

void cleanup(Gameboy* gb) {
    LOG_INFO("cleanup");
    delete gb;
}

void loadGpuData(MMU* mmu) {
    //0: 00000000 = 0x00
    //1: 00111100 = 0x3C
    //2: 01100110 = 0x66
    //3: 01111110 = 0x7e
    //4: 01100110 = 0x66
    //5: 01100110 = 0x66
    //6: 01100110 = 0x66
    //7: 00000000 = 0x00
    byte a[8] = {
        0x00,
        0x3c,
        0x66,
        0x7e,
        0x66,
        0x66,
        0x66,
        0x00};
    //0: 00000000 = 0x00
    //1: 01111100 = 0x7C
    //2: 01100110 = 0x66
    //3: 01111100 = 0x7c
    //4: 01100110 = 0x66
    //5: 01100110 = 0x66
    //6: 01111100 = 0x7c
    //7: 00000000 = 0x00
    byte b[8] = {
        0x00,
        0x7c,
        0x66,
        0x7c,
        0x66,
        0x66,
        0x7c,
        0x00};
    byte* tiledata = mmu->vram;
    byte tiles = 22;
    srand(123145);
    for (byte t = 0; t < tiles; ++t) {
        tiledata += 16;
        for (byte i = 0; i < 16; ++i) {
            tiledata[i] = rand() % 0x100;
        }
    }
    for (word m = 0; m < 1024; ++m) {
        mmu->vram[0x1800 + m] = rand() % tiles;
    }

}
void Gameboy::runBios(const char *path) {
    bios = nullptr;
    std::ifstream file(path, std::ios::in|std::ios::binary|std::ios::ate);
    
    if (file.is_open()) {
        std::ifstream::pos_type size = file.tellg();
        file.seekg(0, std::ios::beg);
        bios = new byte[size];
        file.read((char*)bios, size);
        file.close();
    }
    cpu->runBios(bios);
    delete [] bios;
}

void go() {
//    const char *file = "/Users/jtrinklein/src/gameboy/emulator/roms/cpu_instrs/cpu_instrs.gb";
//    const char *file = "/Users/jtrinklein/src/gameboy/emulator/roms/cpu_instrs/individual/01-special.gb";
//    const char *file = "/Users/jtrinklein/src/gameboy/emulator/roms/
    std::string biosPath = "/Users/jtrinklein/src/gameboy/emulator/roms/bios.bin";
    std::string path = "/Users/jtrinklein/src/gameboy/emulator/roms/";
    //**/    path += "cpu_instrs/individual/06-ld r,r.gb";
    /**/    path += "cpu_instrs/cpu_instrs.gb";

    //**/    path += "cpu_instrs/individual/01-special.gb";

    //**/    path += "cpu_instrs/individual/10-bit ops.gb";
    //**/    path += "blocks.gb";
//**/    path += "test.gb";
//**/    path += "cpu_instrs/individual/03-op sp,hl.gb";
//**/   path += "instr_timing/instr_timing.gb";
    LOG_INFO("loading rom:");
    LOG_INFO(path.c_str());

    Gameboy* gb = new Gameboy();

    initCPU(gb);
    gb->loadCart(path.c_str());

    gb->runBios(biosPath.c_str());
    LOG_INFO("check graphic");
    if(!gb->mmu->cart->isLogoValid) {
        if(STRICT_ROMCHECK) {
            LOG_ERR("Nintendo Logo is corrupted!");
            cleanup(gb);
        } else {
            LOG_WARN("Nintendo Logo is corrupted! This would fail normal hardware");
        }
    }
    LOG_INFO("starting cpu");

    gb->cpu->run();

    cleanup(gb);
}
