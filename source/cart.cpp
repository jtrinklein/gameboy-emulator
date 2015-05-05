
#include "cart.h"
#include "GBCarts.h"
#include <fstream>

static const byte NintendoGraphic[] = {
 0xCE, 0xED, 0x66, 0x66, 0xCC, 0x0D, 0x00, 0x0B, 0x03, 0x73, 0x00, 0x83, 0x00, 0x0C, 0x00, 0x0D,
 0x00, 0x08, 0x11, 0x1F, 0x88, 0x89, 0x00, 0x0E, 0xDC, 0xCC, 0x6E, 0xE6, 0xDD, 0xDD, 0xD9, 0x99,
 0xBB, 0xBB, 0x67, 0x63, 0x6E, 0x0E, 0xEC, 0xCC, 0xDD, 0xDC, 0x99, 0x9F, 0xBB, 0xB9, 0x33, 0x3E
};
static byte RamBanks[4] = { 1, 1, 4, 16 };

Cart::Cart(const char *path) {
    rom = NULL;
    if(!strncmp(path, "TEST", 4)) {
        rom = new byte[0x8000];
        type = 0x02; // mbc1 + ram
        isLogoValid = true;
        isCMPValid = true;
        isCRCValid = true;
        romBanks = 1;
        ramBanks = 1;
    } else {
        loadOk = loadRom(path);
    }
    
}

Cart::~Cart() {
    if (rom != NULL) {
        delete [] rom;
    }
}

bool Cart::loadRom(const char *path) {
    if(rom != NULL) {
        delete [] rom;
    }
    
    rom = NULL;
    isLogoValid = isCRCValid = isCMPValid = false;
    romBanks = ramBanks = romSize = ramSize = 0x00;
    
    std::ifstream file(path, std::ios::in|std::ios::binary|std::ios::ate);

    if (file.is_open()) {
        std::ifstream::pos_type size = file.tellg();
        file.seekg(0, std::ios::beg);
        rom = new byte[size];
        file.read((char*)rom, size);
        file.close();
    }
    
    if (rom == NULL) {
        return false;
    }
    romBanks = GB_ROMBanks(rom);
    romSize = GB_ROMSize(rom);
    ramBanks = RamBanks[rom[0x149]];
    ramSize = GB_RAMSize(rom);
    
    isLogoValid = checkLogo();
    isCRCValid = checkCRC();
    isCMPValid = checkCMP();
    
    return true;
}


bool Cart::checkCMP() {
    byte realCMP = GB_RealCMP(rom);
    byte cmp = rom[0x14D];
    return realCMP == cmp;
}

bool Cart::checkCRC() {
    word realCRC = GB_RealCRC(rom);
    pair crc;
    crc.B.h = rom[0x14E];
    crc.B.l = rom[0x14F];
    return realCRC == crc.W;
}

bool Cart::checkLogo() {
    for (byte i = 0; i < 48; ++i) {
        if (rom[0x0104+i] != NintendoGraphic[i]) {
            return false;
        }
    }
    return true;
}
