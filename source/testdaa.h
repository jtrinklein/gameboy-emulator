#ifndef __TESTDAA_H__
#define __TESTDAA_H__
#include <iostream>
void Daa(Gameboy* gb)
{
    byte f = gb->cpu->AF.B.l;
    int a = gb->cpu->AF.B.h;

    if ((f&FLAG_N) == 0)
    {
        if ((f& FLAG_H) || (a & 0xF) > 9)
            a += 0x06;

        if ((f&FLAG_C) || a > 0x9F)
            a += 0x60;
    }
    else
    {
        if ((f&FLAG_H))
            a = (a - 6) & 0xFF;

        if ((f& FLAG_C))
            a -= 0x60;
    }

    f &= ~(FLAG_H | FLAG_Z);

    if ((a & 0x100) == 0x100)
        f |= FLAG_C;

    a &= 0xFF;

    if (a == 0)
        f |= FLAG_Z;

    gb->cpu->AF.B.h = (byte)a;
    gb->cpu->AF.B.l = f;
}
void testDAA(Gameboy *gb) {
#define wbin(x) std::cout << std::hex << std::uppercase << (word)x << " " << std::dec <<\
    ((x>>7) & 1) << ((x>>6) & 1) << ((x>>5) & 1) << ((x>>4) & 1) << \
    ((x>>3) & 1) << ((x>>2) & 1) << ((x>>1) & 1) << (x & 1)
    byte v = 0;
    byte them,me,fMe,fThem;
    word fails = 0,fFails = 0;
    for (byte i = 0; i < 0xF; ) {
        byte f = i << 4;

        gb->cpu->AF.B.l = f;
        gb->cpu->AF.B.h = v;
        gb->cpu->execOp(0x27);
        me = gb->cpu->AF.B.h;
        fMe = gb->cpu->AF.B.l;

        gb->cpu->AF.B.l = f;
        gb->cpu->AF.B.h = v;
        Daa(gb);
        them = gb->cpu->AF.B.h;
        fThem = gb->cpu->AF.B.l;
        if(me != them) {
            std::cout << "value - ";
            std::cout << "A: ";
            wbin(v);
            std::cout << " F: ";
            wbin(f);
            std::cout << " me: ";
            wbin(me);
            std::cout << " them: ";
            wbin(them);
            std::cout << std::endl;
            fails++;
        }
        if (fMe != fThem) {
            std::cout << "flags - ";
            std::cout << "A: ";
            wbin(v);
            std::cout << " F: ";
            wbin(f);
            std::cout << " me: ";
            wbin(fMe);
            std::cout << " them: ";
            wbin(fThem);
            std::cout << std::endl;
            fFails++;
        }
        ++v;
        if (v == 0) {
            i++;
        }
    }
    std::cout << "failed: " << fails;
    if (fails == 0) {
        std::cout << " WEEEEEEEEEEE!!!!!!!!!!";
    }
    std::cout << std::endl;
    std::cout << "F-Failed: " << fFails;
    if (fFails == 0) {
        std::cout << " w00000000000ttt!!";
    }
    std::cout << std::endl;
#undef wbin
}
#endif
