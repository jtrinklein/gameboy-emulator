#include "pad.h"
#include "irrlicht.h"
#include "gameboy.h"
#include "mmu.h"
#include "cpu.h"
#include <iostream>

using namespace irr;
Pad::Pad(Gameboy* g) {
    gb = g;
    std::cout << "PAD - ctor" << std::endl;
    mmu = gb->mmu;
    mmu->keys[0] = 0;
    mmu->keys[1] = 0;
    mmu->keys[2] = 0;
    bindA(KEY_KEY_A);
    bindB(KEY_KEY_B);
    bindSelect(KEY_SPACE);
    bindStart(KEY_RETURN);
    bindUp(KEY_UP);
    bindDown(KEY_DOWN);
    bindLeft(KEY_LEFT);
    bindRight(KEY_RIGHT);
    std::cout << "PAD - ctor finished" << std::endl;
}

Pad::~Pad() {

}

void Pad::bindA(byte irrKey){
    byte oldKey = JoyToKey[INPUT_SRC_BTNS >> 4][INPUT_BTN_A];
    JoyToKey[INPUT_SRC_BTNS >> 4][INPUT_BTN_A] = irrKey;
    KeyToJoy[oldKey] = 0;
    KeyToJoy[irrKey] = INPUT_BTN_A;
    src[oldKey] = 0;
    src[irrKey] = INPUT_SRC_BTNS >> 4;
}

void Pad::bindB(byte irrKey){
    byte oldKey = JoyToKey[INPUT_SRC_BTNS >> 4][INPUT_BTN_B];
    JoyToKey[INPUT_SRC_BTNS >> 4][INPUT_BTN_B] = irrKey;
    KeyToJoy[oldKey] = 0;
    KeyToJoy[irrKey] = INPUT_BTN_B;
    src[oldKey] = 0;
    src[irrKey] = INPUT_SRC_BTNS >> 4;
}

void Pad::bindStart(byte irrKey){
    byte oldKey = JoyToKey[INPUT_SRC_BTNS >> 4][INPUT_BTN_START];
    JoyToKey[INPUT_SRC_BTNS >> 4][INPUT_BTN_START] = irrKey;
    KeyToJoy[oldKey] = 0;
    KeyToJoy[irrKey] = INPUT_BTN_START;
    src[oldKey] = 0;
    src[irrKey] = INPUT_SRC_BTNS >> 4;
}

void Pad::bindSelect(byte irrKey){
    byte oldKey = JoyToKey[INPUT_SRC_BTNS >> 4][INPUT_BTN_SELECT];
    JoyToKey[INPUT_SRC_BTNS >> 4][INPUT_BTN_SELECT] = irrKey;
    KeyToJoy[oldKey] = 0;
    KeyToJoy[irrKey] = INPUT_BTN_SELECT;
    src[oldKey] = 0;
    src[irrKey] = INPUT_SRC_BTNS >> 4;
}

void Pad::bindUp(byte irrKey){
    byte oldKey = JoyToKey[INPUT_SRC_DPAD >> 4][INPUT_UP];
    JoyToKey[INPUT_SRC_DPAD >> 4][INPUT_UP] = irrKey;
    KeyToJoy[oldKey] = 0;
    KeyToJoy[irrKey] = INPUT_UP;
    src[oldKey] = 0;
    src[irrKey] = INPUT_SRC_DPAD >> 4;
}

void Pad::bindDown(byte irrKey){
    byte oldKey = JoyToKey[INPUT_SRC_DPAD >> 4][INPUT_DOWN];
    JoyToKey[INPUT_SRC_DPAD >> 4][INPUT_DOWN] = irrKey;
    KeyToJoy[oldKey] = 0;
    KeyToJoy[irrKey] = INPUT_DOWN;
    src[oldKey] = 0;
    src[irrKey] = INPUT_SRC_DPAD >> 4;
}

void Pad::bindLeft(byte irrKey){
    byte oldKey = JoyToKey[INPUT_SRC_DPAD >> 4][INPUT_LEFT];
    JoyToKey[INPUT_SRC_DPAD >> 4][INPUT_LEFT] = irrKey;
    KeyToJoy[oldKey] = 0;
    KeyToJoy[irrKey] = INPUT_LEFT;
    src[oldKey] = 0;
    src[irrKey] = INPUT_SRC_DPAD >> 4;
}

void Pad::bindRight(byte irrKey){
    byte oldKey = JoyToKey[INPUT_SRC_DPAD >> 4][INPUT_RIGHT];
    JoyToKey[INPUT_SRC_DPAD >> 4][INPUT_RIGHT] = irrKey;
    KeyToJoy[oldKey] = 0;
    KeyToJoy[irrKey] = INPUT_RIGHT;
    src[oldKey] = 0;
    src[irrKey] = INPUT_SRC_DPAD >> 4;
}

bool Pad::OnEvent(const SEvent& event) {
    bool interrupt = false;
    if(event.EventType == EET_KEY_INPUT_EVENT) {

        byte key = event.KeyInput.Key;

        if (event.KeyInput.PressedDown) {
            byte k = mmu->keys[src[key]];
            byte j =  KeyToJoy[key];
            if ((k&j) == 0) {
                mmu->keys[src[key]] |= j;
                interrupt = true;
            }
        } else {
            mmu->keys[src[key]] &= ~(KeyToJoy[key]);
            interrupt = true;
        }
        if (interrupt) {
            gb->mmu->IF |= IR_JOYPAD;
        }
    }
    return interrupt;
}
