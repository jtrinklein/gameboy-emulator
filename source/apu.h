#ifndef __APU_H__
#define __APU_H__

#include "types.h"
#include "Gb_Apu.h"
#include "Multi_Buffer.h"


class APU {
public:
    APU();
    ~APU();
    
    void reset();
    void step(byte cycles);
    byte read(word addr);
    void write(word addr, byte val);
    void frameEnd();
    void clockTick(byte opCycles);
  
    Gameboy* gb;
    Gb_Apu apu;
    Stereo_Buffer sbuff;
    gb_time_t time;
    
private:
};

#endif

