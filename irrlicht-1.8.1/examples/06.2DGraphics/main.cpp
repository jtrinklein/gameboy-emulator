/** Example 006 2D Graphics

This Tutorial shows how to do 2d graphics with the Irrlicht Engine.
It shows how to draw images, keycolor based sprites,
transparent rectangles, and different fonts. You may consider
this useful if you want to make a 2d game with the engine, or if
you want to draw a cool interface or head up display for your 3d game.

As always, I include the header files, use the irr namespace,
and tell the linker to link with the .lib file.
*/
#include "irrlicht.h"
#include "render.h"

#define NOCPU 0
#define TILESET 0

using namespace irr;

void setTile1(byte* ram, pair data, byte idx) {
    word addr = 0x8000 + idx*16;
    for(byte i = 0; i < 8; ++i) {
        ram[addr + 2*i] = data.B.l;
        ram[addr + 2*i +1] = data.B.h;
    }
}
void setTile0(byte* ram, pair data, signedbyte idx) {
    word addr = 0x9000 + idx*16;
    for(byte i = 0; i < 8; ++i) {
        ram[addr + 2*i] = data.B.l;
        ram[addr + 2*i +1] = data.B.h;
    }
}
byte* initRam0() {
    byte* ram = new byte[0xFFFF];
    memset(ram, 0, 0xFFFF);
    pair t;
    
    t.B.l = 0xAA;
    t.B.h = 0xAA;
    setTile0(ram, t, 0x01);
    t.B.l = 0xC3;
    t.B.h = 0xC3;
    setTile0(ram, t, -0x01);
    t.B.l = 0xFF;
    t.B.h = 0xFF;
    setTile0(ram, t, 0x02);
    t.B.l = 0x18;
    t.B.h = 0x18;
    setTile0(ram, t, -0x02);

    
    for (byte i = 0; i < 0x0A; ++i) {
        ram[0x9820+i*2] = 0x01;
        ram[0x9820+i*2+1] = -0x01;
        ram[0x9840+i] = 0x02;
        ram[0x9860+i*2] = -0x02;


    }
    return ram;
}

byte* initRam1() {
    byte* ram = new byte[0xFFFF];
    memset(ram, 0, 0xFFFF);
    pair t;
    
    t.B.l = 0xFF;
    t.B.h = 0x00;
    setTile1(ram, t, 0x01);
    t.B.l = 0x00;
    t.B.h = 0xFF;
    setTile1(ram, t, 0x02);
    t.B.l = 0xFF;
    t.B.h = 0xFF;
    setTile1(ram, t, 0x03);
    t.B.l = 0xC3;
    t.B.h = 0x0A;
    setTile1(ram, t, 0x04);
    
    
    for (byte i = 0; i < 0x0A; ++i) {
        ram[0x9C20+i*2] = 0x01;
        ram[0x9C20+i*2+1] = 0x04;
        ram[0x9C40+i] = 0x02;
        ram[0x9C60+i*2] = 0x03;
        ram[0x9C81+i*2] = 0x03;
    }
    return ram;
}

int main()
{
	// create device
#if NOCPU
#if TILESET
    byte * ram = initRam1();
    Render *r = new Render(ram);
    r->useTileset1();
#else
    byte * ram = initRam0();
    Render *r = new Render(ram);
    r->useTileset0();
#endif


	
	while(r->device->run() && r->driver)
	{
		if (r->device->isWindowActive())
		{
            r->renderStep(8);
		}
	}

	r->device->drop();
    delete r;
    delete [] ram;
#else
    go();
#endif
	return 0;
}


