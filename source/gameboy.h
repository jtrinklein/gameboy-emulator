#ifndef __GAMEBOY_H__
#define __GAMEBOY_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"

class Gameboy {
    public:
    Gameboy();
    ~Gameboy();

    CPU* cpu;
    MMU* mmu;
    GPU* gpu;
    Pad* pad;
    APU* apu;
    
    byte* bios;

    void runBios(const char *path);
    void loadCart(const char *path);
};
    /*
class CPU {
public:
    CPU();
    ~CPU();

    MMU *mmu;
    GPU *gpu;

    pair AF, BC, DE, HL, SP, PC;
    byte opCycles;

    int IPeriod,ICycles;
    word IRequest;      // set to adress of pending IRQ
    byte IAutoReset;    // set to 1 to automatically reset IRequest
    byte IE; // Interrupt Enabled register
    byte IME;// Interrupt Master Enable
    byte runState; // halt, stop, etc...

    byte divtick,ticker,timertick;
    void timerStep(byte cycles);
    void init();
    void reset();
};
    */

//const int DEFAULT_INTERRUPT_PERIOD = 175782; // from 2.6GHz processor
const int DEFAULT_INTERRUPT_PERIOD = 75782; // from 2.6GHz processor
const int INITIAL_PC = 100;
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
