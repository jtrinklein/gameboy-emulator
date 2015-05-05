#ifndef __MMU_H__
#define __MMU_H__

#include "types.h"

#define MBC1_RAM_MODE 1
#define MBC1_ROM_MODE 0

typedef struct {
    byte ramEnabled;
    byte expansionMode;
    byte rombank;
    byte rambank;
} MBCState;

class MMU {
public:
    MMU();
    ~MMU();
    
    void reset();
    byte read(word addr);
    void write(word addr, byte value);
    void loadCart(const char *path);
    
    byte *rom, *ram, *eram, *zram, *wram, *vram, *oam;
    Cart *cart;
    byte serialBuffer, serialCtrl;
    
private:
    byte scount = 0;
    void writeSerial();
    word romOffset, ramOffset;
    byte cartType;
    MBCState mbcState;
    MBCState states[2];
    
    void setMBCState();
    
};

#endif

