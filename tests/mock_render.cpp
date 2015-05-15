#include "render.h"
#include "gameboy.h"


GPU::GPU(MMU*) {
    this->SCX = this->SCY = this->scanline = 0;
    this->LCD = 0x81;
}

GPU::~GPU() {
}

bool GPU::running() {
    return true;
}

void GPU::renderStep(byte /*cycleDelta*/) {
}

byte GPU::getLCD() {
    return this->LCD;
}

void GPU::setLCD(byte data) {
    this->LCD = data;
}

void GPU::setPalette(byte /*data*/){
}

