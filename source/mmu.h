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
    MMU(Gameboy* gb);
    ~MMU();

    void reset();
    byte read(word addr);
    void write(word addr, byte value);
    void loadCart(const char *path);
    Gameboy *gb;
    byte *rom, *ram, *eram, *zram, *wram, *vram, *oam;
    Cart *cart;
    byte serialBuffer, serialCtrl, lyc,div,tac,tma,tima,p1;
    byte IF; // Interrupt flag register
    byte IE; // Interrupt flag register
    byte keys[3];
    bool inBios;
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
