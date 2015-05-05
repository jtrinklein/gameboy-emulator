#include "render.h"
#include "gameboy.h"


Render::Render(MMU*) {
    this->SCX = this->SCY = this->scanline = 0;
    this->LCD = 0x81;
}

Render::~Render() {
}

bool Render::running() {
    return true;
}

void Render::renderStep(byte /*cycleDelta*/) {
}

byte Render::getLCD() {
    return this->LCD;
}

void Render::setLCD(byte data) {
    this->LCD = data;
}

void Render::setPalette(byte /*data*/){
}

