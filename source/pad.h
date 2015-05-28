#ifndef __PAD_H__
#define __PAD_H__

#include "irrlicht.h"
#include "types.h"

class Pad : public irr::IEventReceiver {
public:
    Pad(Gameboy* m);
    ~Pad();
    bool OnEvent(const irr::SEvent& event);
    MMU* mmu;

    void bindA(byte irrKey);
    void bindB(byte irrKey);
    void bindStart(byte irrKey);
    void bindSelect(byte irrKey);

    void bindUp(byte irrKey);
    void bindDown(byte irrKey);
    void bindLeft(byte irrKey);
    void bindRight(byte irrKey);

    Gameboy* gb;
    byte JoyToKey[3][9];
    byte KeyToJoy[0xFF];
    byte src[0xFF];
};

#endif
