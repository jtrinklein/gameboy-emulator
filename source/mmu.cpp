#include <cstdlib>
#include <iostream>
#include "mmu.h"
#include "cart.h"

MMU::MMU() {
    ram = new byte[0x2000];
    vram = new byte[0x2000];
    eram = new byte[0x6000];
    wram = new byte[0x2000];
    oam = new byte[0x100];
    zram = new byte[0x1000];
    cart = nullptr;
    reset();
}

MMU::~MMU() {
    
    if(ram != nullptr) {
        delete ram;
    }
    if(vram != nullptr) {
        delete vram;
    }
    if(eram != nullptr) {
        delete eram;
    }
    if(zram != nullptr) {
        delete zram;
    }
    if(wram != nullptr) {
        delete wram;
    }
    if(oam != nullptr) {
        delete oam;
    }
    
    if(cart != nullptr) {
        delete cart;
    }
    cart = nullptr;
}


void MMU::reset() {
    if (cart != nullptr) {
        delete cart;
        cart = nullptr;
    }
    
    cartType = 0x00;
    ramOffset = DEF_RAM_OFFSET;
    romOffset = DEF_ROM_OFFSET;
    
    states[1].rombank = 0x00;
    states[1].rambank = 0x00;
    states[1].ramEnabled = 0x00;
    states[1].expansionMode = 0x00;
}

byte MMU::read(word A) {
    word addr = A;
    switch (addr&0xF000){
        case 0x0000://BIOS
        case 0x1000:
        case 0x2000:
        case 0x3000:// ROM 0
            return rom[addr];
        case 0x4000:
        case 0x5000:
        case 0x6000:
        case 0x7000:// ROM (switched bank)
            return rom[romOffset + (addr & 0x3FFF)];
        case 0x8000:
        case 0x9000://VRAM
            return vram[addr & 0x1FFF];
        case 0xA000:
        case 0xB000://External RAM
            return eram[ramOffset + (addr & 0x1FFF)];
        case 0xC000:
        case 0xD000://Working RAM
        case 0xE000://Working RAM (shadow)
            return wram[addr & 0x1FFF];
        case 0xF000://Working RAM (shadow), IO, Zero-page RAM
            switch(addr&0x0F00) {
                case 0xF00: // Zero-page RAM
                    if (addr == 0xFFFF) {
                        return 0; // handled on CPU
                    } else if (addr >= 0xFF80) {
                        return zram[addr & 0x7F];
                    }
                    else {
                        switch (addr & 0x00FF) {
                            case 0x01: // SB - serial buffer
                                return serialBuffer;
                                break;
                            case 0x02: // SC - serial control
                                return serialCtrl;
                                break;
                            default:
                                break;
                                
                                // 0xFF00 - 0xFF7F is IO, not handled yet
                        }
                        return 0; // handled on CPU
                    }
                    // 0xFF00 - 0xFF7F is IO, not handled yet
                    return 0;
                    break;
                case 0xE00: // OAM
                    if(addr < 0xFEA0) {
                        return oam[addr & 0xFF];
                    }
                    return 0;
                default: // Working RAM (shadow)
                    return wram[addr&0x1FFF];
            }
            break;
            
    }
    return 0;
}

void MMU::write(word addr,byte val) {
    switch (addr&0xF000){
        case 0x0000:
        case 0x1000:
            switch (cart->type) {
                case 0x01: //MBC1
                case 0x02: //MBC1+RAM
                case 0x03: //MBC1+RAM+Battery
                    mbcState.ramEnabled = (val == 0x0A) ? 0x1 : 0x0;
                    break;
            }
            break;
        case 0x2000:
        case 0x3000:// ROM Bank
            mbcState.rombank = (val &0x31);
            if (mbcState.rombank == 0) {
                mbcState.rombank = 1;
            }
            break;
        case 0x4000:
        case 0x5000:// RAM Bank / ROM Bank (bits 6,7)
            switch (cart->type) {
                case 0x01: //MBC1
                case 0x02: //MBC1+RAM
                case 0x03: //MBC1+RAM+Battery
                    if (mbcState.expansionMode == MBC1_RAM_MODE) {
                        mbcState.rambank = val & 0x3;
                        ramOffset = mbcState.rambank * 0x2000;
                    }
                    else {
                        mbcState.rombank = (mbcState.rombank & 0x1F) + ((val & 0x3) << 5);
                        romOffset = mbcState.rombank * 0x4000;
                    }
                    break;
            }
            break;
        case 0x6000:
        case 0x7000:// RAM/ROM mode switch
            switch (cart->type) {
                case 0x02://MBC1+RAM
                case 0x03://MBC1+RAM+Battery
                    mbcState.expansionMode = val & 0x1;
                    break;
            }
            break;
        case 0x8000:
        case 0x9000://VRAM
            vram[addr & 0x1FFF] = val;
            break;
        case 0xA000:
        case 0xB000://External RAM
            eram[ramOffset + (addr & 0x1FFF)] = val;
            break;
        case 0xC000:
        case 0xD000://Working RAM
        case 0xE000://Working RAM (shadow)
            wram[addr & 0x1FFF] = val;
            break;
        case 0xF000://Working RAM (shadow), IO, Zero-page RAM
            switch(addr&0x0F00) {
                case 0xF00: // Zero-page RAM
                    if (addr >= 0xFF80) {
                        zram[addr & 0x7F] = val;
                    }
                    else {
                        switch (addr & 0x00FF) {
                            case 0x01: // SB - serial buffer
                                serialBuffer = val;
                                zram[addr & 0x7f] = val;
                                break;
                            case 0x02: // SC - serial control
                                if( val & 0x80) {
                                    writeSerial();
                                    serialCtrl = val & 0x01;
                                    zram[addr & 0x7f] = serialCtrl;
                                }
                                break;
                            default:
                                break;
                                
                            // 0xFF00 - 0xFF7F is IO, not handled yet
                        }
                    }
                    
                    break;
                case 0xE00: // OAM
                    if(addr < 0xFEA0) {
                        oam[addr & 0xFF] = val;
                    }
                    break;
                default: // Working RAM (shadow)
                    wram[addr&0x1FFF] = val;
                    break;
            }
            break;
            
    }
}

void MMU::writeSerial() {

    if(serialBuffer == 0x8E) {
        std::cout << std::endl;
        scount = 0;
    } else {
        std::cout << serialBuffer;
        if(++scount == 80) {
            std::cout << std::endl;
            scount = 0;
        }
    }
}


void MMU::loadCart(const char *path) {

    cart = new Cart(path);
    if(cart->loadRom(path)) {
        rom = cart->rom;
        setMBCState();
    };
    
}

void MMU::setMBCState() {
    switch (cart->type) {
        default:
            break;
    }
}

