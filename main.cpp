#include "EMULib/GBCarts.h"
#include "gameboy.h"
#include "cycles.h"
#include <stdio.h>

#define INLINE static inline

byte *Page,*ROM;
INLINE byte readMem(word A)        { return Page[A]; }
INLINE void writeMem(word A,byte V) { Page[A]=V; }
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
#define ADD_A(v) J.W = cpu->AF.B.h + v;\
                cpu->AF.B.l = ((J.B.l)?0:FLAG_Z)|(((J.B.l&0x08) && (v&0x07))?FLAG_H:0)|(((J.B.h&0x01)&&(v))?FLAG_C:0);\
                cpu->AF.B.h = J.B.l

#define ADC_A(v) ADD_A(((cpu->AF.B.l & FLAG_C) ? 0x01 : 0) + v)

#define SUB_A(v) J.W = cpu->AF.B.h - v;\
                cpu->AF.B.l = FLAG_N|\
                              (((cpu->AF.B.h&0x07) >= (v&0x07))?FLAG_H:0)|\
                              ((cpu->AF.B.h >= v)?FLAG_C:0)|\
                              ((J.B.l)?0:FLAG_Z);\
                cpu->AF.B.h = J.B.l

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

}

void runGB(CPU* cpu) {
    byte I; // instruction
    pair J;

    for(;;) {
        I = getOp(cpu->PC.W++);

        cpu->ICycles -= cycles[I];

        switch(I) {
#include "opcode.h"
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
}

int main(int arc, char** argv) {
    CPU* cpu = new CPU();
    Page = new byte[0xFFFF];
    ROM = new byte[0xFFFF];
    runGB(cpu);
    delete [] ROM;
    delete [] Page;
    delete cpu;
}
