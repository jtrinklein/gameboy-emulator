#define PRINTOPS 0
#define LOGGING 1

#include "GBCarts.h"
#include "gameboy.h"
#include "gpu.h"
#include "cycles.h"
#include <fstream>
#include <stdio.h>
#include <string>
#include "bios.h"
#include "cart.h"
#include "mmu.h"
#include "pad.h"
#include "cpu.h"
#include "apu.h"


Gameboy::Gameboy() {
    mmu = new MMU(this);
    pad = new Pad(this);
    cpu = new CPU(this);
    gpu = new GPU(this);
    apu = new APU();
}

Gameboy::~Gameboy() {
    delete gpu;
    delete cpu;
    delete mmu;
    delete pad;
    delete apu;
}

void Gameboy::loadCart(const char* path) {
    mmu->loadCart(path);
}

#define STRICT_ROMCHECK 0

#if LOGGING
#define LOG_ERR(x)  printf("ERROR: "); printf(x); printf("\n")
#define LOG_WARN(x) printf("WARN : "); printf(x); printf("\n")
#define LOG_INFO(x) printf("INFO : %s\n", x)
#else
#define LOG_ERR(x)  /**/
#define LOG_WARN(x) /**/
#define LOG_INFO(x) /**/
#endif


void initCPU(Gameboy* gb) {
    gb->cpu->reset();
}

void cleanup(Gameboy* gb) {
    LOG_INFO("cleanup");
    delete gb;
}


void Gameboy::runBios(const char *path) {
    bios = nullptr;
    std::ifstream file(path, std::ios::in|std::ios::binary|std::ios::ate);

    if (file.is_open()) {
        std::ifstream::pos_type size = file.tellg();
        file.seekg(0, std::ios::beg);
        bios = new byte[size];
        file.read((char*)bios, size);
        file.close();
    }
    cpu->runBios(bios);
    delete [] bios;
}

void Gameboy::run() {
    cpu->run();
}

#include "testdaa.h"
void go(bool skipBios) {
//    const char *file = "/Users/jtrinklein/src/gameboy/emulator/roms/cpu_instrs/cpu_instrs.gb";
//    const char *file = "/Users/jtrinklein/src/gameboy/emulator/roms/cpu_instrs/individual/01-special.gb";
//    const char *file = "/Users/jtrinklein/src/gameboy/emulator/roms/
    std::string biosPath = "/Users/jtrinklein/src/gameboy/emulator/roms/bios.bin";
    std::string path = "/Users/jtrinklein/src/gameboy/emulator/roms/";
    /**/    path += "cpu_instrs/individual/01-special.gb";
    //**/    path += "cpu_instrs/individual/02-interrupts.gb";
    //**/    path += "cpu_instrs/individual/03-op sp,hl.gb";
    //**/    path += "cpu_instrs/individual/04-op r,imm.gb";
    //**/    path += "cpu_instrs/individual/05-op rp.gb";
    //**/    path += "cpu_instrs/individual/06-ld r,r.gb";
    //**/    path += "cpu_instrs/individual/07-jr,jp,call,ret,rst.gb";
    //**/    path += "cpu_instrs/individual/08-misc instrs.gb";
    //**/    path += "cpu_instrs/individual/09-op r,r.gb";
    //**/    path += "cpu_instrs/individual/10-bit ops.gb";
    //**/    path += "cpu_instrs/individual/11-op a,(hl).gb";
    //**/    path += "cpu_instrs/cpu_instrs.gb";
    //**/    path += "mem_timing/mem_timing.gb";




    //**/    path += "vblank.gb";
    //**/    path += "joypad.gb";
    //**/    path += "blocks.gb";
//**/    path += "test.gb";

    //**/   path += "instr_timing/instr_timing.gb";

    LOG_INFO("create Gameboy");
    Gameboy* gb = new Gameboy();
    LOG_INFO("init cpu");
    initCPU(gb);
    LOG_INFO("loading rom:");
    LOG_INFO(path.c_str());
    gb->loadCart(path.c_str());

    //if(!skipBios){
    //    gb->runBios(biosPath.c_str());
    //}

    testDAA(gb);


    LOG_INFO("check graphic");
    if(!gb->mmu->cart->isLogoValid) {
        if(STRICT_ROMCHECK) {
            LOG_ERR("Nintendo Logo is corrupted!");
            cleanup(gb);
        } else {
            LOG_WARN("Nintendo Logo is corrupted! This would fail normal hardware");
        }
    }
    LOG_INFO("starting cpu");

    gb->run();

    cleanup(gb);
}
