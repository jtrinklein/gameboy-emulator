#ifndef __CART_LOADER_H__
#define __CART_LOADER_H__

#include "types.h"

class Cart {
public:
    Cart(const char* path);
    ~Cart();
    bool loadRom(const char *path);
    bool loadOk;
    bool isLogoValid;
    bool isCRCValid;
    bool isCMPValid;
    byte romSize;
    word ramSize;
    byte romBanks;
    byte ramBanks;
    
    byte type;
    const char *name, *typeName;
    byte *rom;
private:
    bool checkLogo();
    bool checkCRC();
    bool checkCMP();
};

#endif
