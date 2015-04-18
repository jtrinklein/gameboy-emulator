#ifndef __RENDER_H__
#define __RENDER_H__

#include "gameboy.h"

struct Tile {
    byte pixels[8];
};

class Render {
    public:
        Render(byte* VRAM);
        ~Render();

        void render();

    private:
        byte* VRAM;

        
}

#endif

