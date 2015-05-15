#ifndef __CPU_H__
#define __CPU_H__

#include "types.h"

class CPU {
public:
    CPU(Gameboy *GB);
    ~CPU();

    void reset();

    void runBios(byte* bios);
    void run();
    void step();
    void stepHalt();
    void stepStop();

    void execOp(byte opcode);
    void execCB(byte cbOp);

    void timerStep();
    void gpuStep();

    void interruptCheck();

    Gameboy *gb;
    byte IME;
    pair BC,DE,HL,AF,SP,PC;
    bool powerOn;
    byte lastOp;
    byte timer; // actual timers
    byte timerTicks,divTicks; // counter for when to step timers
    byte runState;
    byte opCycles;
    int intPeriod, intCycles;

};

#endif
