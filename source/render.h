#ifndef __RENDER_H__
#define __RENDER_H__

#ifdef BUILD_TEST
#define U32 unsigned int
#define COLOR U32
#define PIMAGE void*
#define PTEXTURE void*
#define DEVICE void*
#define DRIVER void*
#else
#include "irrlicht.h"
#endif

#include "types.h"

#ifndef DRIVER
#define DRIVER irr::video::IVideoDriver*
#endif

#ifndef COLOR
#define COLOR irr::video::SColor
#endif

#ifndef DEVICE
#define DEVICE irr::IrrlichtDevice*
#endif

#ifndef U32
#define U32 irr::u32
#endif

#ifndef PU32
#define PU32 U32*
#endif

#ifndef PIMAGE
#define PIMAGE irr::video::IImage*
#endif

#ifndef PTEXTURE
#define PTEXTURE irr::video::ITexture*
#endif

#define TILE_COLUMN_COUNT 20
#define TILE_ROW_COUNT 18

#define PIXEL_BLACK 0xFF000000
#define PIXEL_DARK_GREY 0xFF555555
#define PIXEL_LIGHT_GREY 0xFFAAAAAA
#define PIXEL_WHITE 0xFFFFFFFF

#define COLOR_BLACK 3
#define COLOR_DARK_GREY 2
#define COLOR_LIGHT_GREY 1
#define COLOR_WHITE 0

#define TILESET_0_BASE_ADDR 0x1000
#define TILESET_1_BASE_ADDR 0x0000
#define BGMAP_1_BASE_ADDR 0x1C00
#define BGMAP_0_BASE_ADDR 0x1800
#define VRAM_BASE_ADDR 0x0

#define MODE_OAM_READ 0
#define MODE_VRAM_READ 1
#define MODE_HBLANK 2
#define MODE_VBLANK 3

class Render {
public:
    Render(MMU* mmu);
    ~Render();
    bool running();
    void renderStep(byte cycleDelta);
    byte SCY, SCX, scanline;
    byte getLCD();
    void setLCD(byte data);
    void setPalette(byte data);
private:
    word mapBase, tileBase, clock;
    
    byte largeSprites, spritesEnabled, bgEnabled, windowEnabled, windowMap, displayEnabled;
    byte mode, LCD;
    
    byte *OAM, *VRAM;
    
    COLOR colors[4];
    byte palette[4];
    
    DRIVER driver;
    DEVICE device;
    
    PU32 texture;
    PIMAGE screenImage;
    PTEXTURE screenTexture;
    
    void useTileset(byte id);
    void useBgMap(byte id);
    void renderScanline();
    void drawScreen();
    void updateScreenImage();
    void updateScreenPixel(U32 pixelX, byte pixelY, COLOR color);
    byte readVRAM(word addr);
    COLOR getPixelColor(pair tile, byte pixelIdx);
    
    void updateImageFromTiles();
    void init();
    void resetTileData();
    byte getTileIdx(word addr);
    pair getTileRow(byte tileIdx, byte rowIdx);
    
    byte bgtile; // tileset 0 or 1
    
    byte bgmap; // background map 0 or 1

};

#endif

