#include "Basic_Gb_Apu.h"
#include "apu.h"
#include "gameboy.h"
#include "cpu.h"

const gb_time_t FRAME_LENGTH = 70224;

APU::APU() {
    reset();
}

APU::~APU() {
}

void APU::reset() {
    time = 0;
    apu.output(sbuff.center(), sbuff.left(), sbuff.right());
    sbuff.clock_rate(4194304);
    sbuff.set_sample_rate(44100);
    apu.treble_eq(-20);
    sbuff.bass_freq(460);
}

void APU::write(word addr, byte val) {
    apu.write_register(time, addr, val);
}

byte APU::read(word addr) {
    return apu.read_register(time, addr);
}

void APU::clockTick(byte opCycles) {
    time += opCycles;
}

void APU::frameEnd() {
    time = 0;
    bool stereo = apu.end_frame(FRAME_LENGTH);
    sbuff.end_frame(FRAME_LENGTH, stereo);
}


