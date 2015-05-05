#ifndef __GAMEBOY_H__
#define __GAMEBOY_H__
#ifdef __cplusplus
extern "C" {
#endif
    
#include "types.h"

    
    
class CPU {
public:
    CPU();
    ~CPU();
    
    MMU *mmu;
    Render *gpu;

    pair AF, BC, DE, HL, SP, PC;
    byte opCycles;

    int IPeriod,ICycles;
    word IRequest;      // set to adress of pending IRQ
    byte IAutoReset;    // set to 1 to automatically reset IRequest
    byte IE; // Interrupt Enabled register
    byte IF; // Interrupt flag register
    byte IME;// Interrupt Master Enable
    byte runState; // halt, stop, etc...
    
    void init();
    void reset();
};

const int DEFAULT_INTERRUPT_PERIOD = 175782; // from 2.6GHz processor
const int INITIAL_PC = 0;
byte readMem(CPU* cpu, word A);
void writeMem(CPU* cpu, word A,byte V);
bool execOp(CPU* cpu, byte I);
void runGB(CPU* gb);
void initCPU(CPU* cpu);

void go();
#ifdef __cplusplus
}
#endif
#endif
