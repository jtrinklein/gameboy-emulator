#include <cstdlib>
#include <iostream>
#include "mmu.h"
#include "gameboy.h"
#include "gpu.h"
#include "apu.h"
#include "cart.h"
#include "logo.h"

MMU::MMU(Gameboy* g) {
    std::cout << "creating memory banks" << std::endl;
    gb = g;
    ram = new byte[0x2000];
    vram = new byte[0x2000];
    eram = new byte[0x6000];
    wram = new byte[0x2000];
    oam = new byte[0x100];
    zram = new byte[0x1000];
    std::cout << "finished creating memory banks" << std::endl;
    inBios = false;
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
    
    std::cout << "MMU - reset" << std::endl;
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
            if (inBios) {
                if(addr < 0x100) {
                    if(addr >= 0xD8 && addr < 0xE0) {
                        return rIcon[addr - 0xD8];
                    }
                    return gb->bios[addr];
                }
                else if (addr >= 0x104 && addr <= 0x133) {
                    return logo[addr - 0x104];
                }
            }
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
                        return IE;
                    } else if (addr >= 0xFF80) {
                        return zram[addr & 0x7F];
                    }
                    else {
                        switch (addr & 0x00FF) {
                            case 0x00: // JOYPAD
                                return p1 | keys[p1>>4];
                            case 0x01: // SB - serial buffer
                                return serialBuffer;
                            case 0x02: // SC - serial control
                                return serialCtrl;
                            case 0x04:
                                return div;

                            case 0x14: //NR14 - Channel 1 frequency hi
                            case 0x19: //NR24 - Channel 2 frequency hi
                            case 0x1E: //NR34 - Channel 3 Frequency hi
                                //return gb->apu->read(addr) & 0x40; // only bit 6 is readable xBxxxxxx

                                //Tone & Sweep
                            case 0x10: //NR10 - Channel 1 Sweep register
                            case 0x11: //NR11 - Channel 1 sound length/wave pattern duty
                            case 0x12: //NR12 - Channel 1 volume envelope
                                //return gb->apu->read(addr);

                                //Tone
                            case 0x16: //NR21 - Channel 2 sound length/wave pattern duty
                            case 0x17: //NR22 - Channel 2 volume envelope
                                //return gb->apu->read(addr);

                                //Noise
                            case 0x20: //NR40 - Channel 4 sound length
                            case 0x21: //NR41 - Channel 4 volume envelope
                            case 0x22: //NR42 - Channel 4 polynomial counter
                            case 0x23: //NR43 - Channel 4 counter/consecutive; initial
                                //return gb->apu->read(addr);

                                //Sound control
                            case 0x26: //NR52 - Sound on/off
                            case 0x24: //NR50 - Channel control / on-off / Volume
                            case 0x25: //NR51 - Selection of sound output terminal

                                //Wave Pattern RAM
                            case 0x30: case 0x31: case 0x32: case 0x33:
                            case 0x34: case 0x35: case 0x36: case 0x37:
                            case 0x38: case 0x39: case 0x3A: case 0x3B:
                            case 0x3C: case 0x3D: case 0x3E: case 0x3F:

                                //Wave Output
                            case 0x1A: //NR30 - Channel 3 sound on/off
                            case 0x1B: //NR31 - Channel 3 Sound length
                            case 0x1C: //NR32 - Channel 3 select output level
                                return 0;
                                //return gb->apu->read(addr);

                            case 0x40:// LCDC
                                return gb->gpu->getLCD();
                            case 0x42:// SCY
                                return gb->gpu->SCY;
                            case 0x43:// SCX
                                return gb->gpu->SCX;
                            case 0x44:// scanline
                                return gb->gpu->scanline;
                            case 0x45:// LYC
                                return lyc;
                            case 0x05: //TIMA - timer
                                return tima;
                            case 0x06: //TMA - timer modulo
                                return tma;
                            case 0x07: //TAC - timer control
                                return tac;
                            case 0x0F: //IF
                                return IF;
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
                    if (addr == 0xFFFF) {
                        IE = val;
                    }
                    else if (addr >= 0xFF80) {
                        zram[addr & 0x7F] = val;
                    }
                    else {
                        switch (addr & 0x00FF) {
                            case 0x00: // JOYPAD
                                p1 = val & INPUT_SRC_MASK;
                                break;
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

                                //Sound control
                            case 0x26: //NR52 - Sound on/off
                                val &= 0x80; // only bit 7 is writable Bxxxxxx for on/off register
                            case 0x24: //NR50 - Channel control / on-off / Volume
                            case 0x25: //NR51 - Selection of sound output terminal

                                //Tone & Sweep
                            case 0x10: //NR10 - Channel 1 Sweep register
                            case 0x11: //NR11 - Channel 1 sound length/wave pattern duty
                            case 0x12: //NR12 - Channel 1 volume envelope
                            case 0x13: //NR13 - Channel 1 frequency low
                            case 0x14: //NR14 - Channel 1 frequency hi

                                //Tone
                            case 0x16: //NR21 - Channel 2 sound length/wave pattern duty
                            case 0x17: //NR22 - Channel 2 volume envelope
                            case 0x18: //NR23 - Channel 2 frequency low
                            case 0x19: //NR24 - Channel 2 frequency hi

                                //Wave Output
                            case 0x1A: //NR30 - Channel 3 sound on/off
                            case 0x1B: //NR31 - Channel 3 Sound length
                            case 0x1C: //NR32 - Channel 3 select output level
                            case 0x1D: //NR33 - Channel 3 Frequency low
                            case 0x1E: //NR34 - Channel 3 Frequency hi

                                //Noise
                            case 0x20: //NR40 - Channel 4 sound length
                            case 0x21: //NR41 - Channel 4 volume envelope
                            case 0x22: //NR42 - Channel 4 polynomial counter
                            case 0x23: //NR43 - Channel 4 counter/consecutive; initial

                                //Wave Pattern RAM
                            case 0x30: case 0x31: case 0x32: case 0x33:
                            case 0x34: case 0x35: case 0x36: case 0x37:
                            case 0x38: case 0x39: case 0x3A: case 0x3B:
                            case 0x3C: case 0x3D: case 0x3E: case 0x3F:

                                //gb->apu->write(addr, val);
                                break;

                            case 0x40: // Zero-page RAM
                                gb->gpu->setLCD(val);
                                break;
                            case 0x42: // scy
                                gb->gpu->SCY = val;
                                break;
                            case 0x43: //scx
                                gb->gpu->SCX = val;
                                break;
                            case 0x47: // bg palette
                                gb->gpu->setBgPalette(val);
                                break;
                            case 0x48: // oam palette 0
                                gb->gpu->setOamPalette(0, val);
                                break;
                            case 0x49: // oam palette 1
                                gb->gpu->setOamPalette(1, val);
                                break;
                            case 0x45:// LYC
                                lyc = val;
                                break;
                            case 0x04: //DIV
                                div = 0;
                                break;
                            case 0x05: //TIMA - timer
                                tima = val;
                                break;
                            case 0x06: //TMA - timer modulo
                                tma = val;
                                break;
                            case 0x07: //TAC - timer control
                                tac = val;
                                break;
                            case 0x0F: //IF
                                IF = val;
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
    
    std::cout << "MMU - Load cart" << std::endl;
    cart = new Cart(path);
    if(cart->loadOk) {
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
