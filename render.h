#ifndef __RENDER_H__
#define __RENDER_H__

#include "gameboy.h"
#include "irrlicht.h"


#define TILE_COLUMN_COUNT 20
#define TILE_ROW_COUNT 18

#define PIXEL_BLACK 0xFF000000
#define PIXEL_DARK_GREY 0xFF555555
#define PIXEL_LIGHT_GREY 0xFFAAAAAA
#define PIXEL_WHITE 0xFFFFFFFF

#define TILESET_O_BASE_ADDR 0x1000
#define TILESET_1_BASE_ADDR 0x0000
#define BGMAP_1_BASE_ADDR 0x1C00
#define BGMAP_0_BASE_ADDR 0x1800
#define VRAM_BASE_ADDR 0x8000

class Render {
public:
    Render(byte* VRAM);
    ~Render();
    void renderStep();
    void useTileset0();
    void useTileset1();
    byte SCY,SCX;
    irr::video::IVideoDriver* driver;
    irr::IrrlichtDevice* device;

private:
    irr::u32* texture;
    void renderScanline();
    void drawScreen();
    word addrBase;
    void updateScreenImage();
    void updateScreenPixel(irr::u32 pixelX, byte pixelY, irr::video::SColor color);
    byte readVRAM(word addr);
    irr::video::SColor getPixelColor(pair tile, byte pixelIdx);
    irr::video::SColor palette[4];
    void updateImageFromTiles();
    irr::video::IImage* screenImage;
    irr::video::ITexture* screenTexture;
    void init();
    void resetTileData();
    byte getTileIdx(word addr);
    pair getTileRow(byte tileIdx, byte rowIdx);
    byte bgtile; // tileset 0 or 1
    byte* VRAM;
    bool bgmap;
    byte scanline;
    

    
};

#endif

