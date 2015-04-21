#include "irrlicht.h"
#include "render.h"

using namespace irr;

Render::Render(byte* vram) {
    this->VRAM = vram;
    this->bgtile = 0;
    this->init();
    this->scanline = 0;
    this->SCX = 0;
    this->SCY = 0;
    this->bgmap = 0;
    this->useTileset0();
    
}
#define PIXEL_WIDTH 160
#define PIXEL_HEIGHT 144
#define WINDOW_SCALE 3
Render::~Render() {
}

void Render::init() {
    this->palette[0] = video::SColor(PIXEL_WHITE);
    this->palette[1] = video::SColor(PIXEL_LIGHT_GREY);
    this->palette[2] = video::SColor(PIXEL_DARK_GREY);
    this->palette[3] = video::SColor(PIXEL_BLACK);
    this->device = irr::createDevice(video::EDT_OPENGL, core::dimension2d< u32 >(PIXEL_WIDTH*WINDOW_SCALE, PIXEL_HEIGHT*WINDOW_SCALE));
    this->driver = this->device->getVideoDriver();
    this->screenImage = driver->createImage(video::ECF_A8R8G8B8, core::dimension2d< u32 >(PIXEL_WIDTH, PIXEL_HEIGHT));
    this->screenImage->fill(video::SColor(0xFF, 0xFF, 0xFF, 0xFF));
    this->screenTexture = driver->addTexture("GB_SCREEN", this->screenImage);
}

byte Render::getTileIdx(word addr) {
    return this->readVRAM(addr);
}

video::SColor Render::getPixelColor(pair tile, byte pixelIdx) {
    //high byte is first pixel section - (low bit)
    //low byte is second pixel section - (high bit)
    // pixel 0 is highest bit
    byte colorIdx = (((tile.B.l >> (7 - pixelIdx)) & 0x01)<< 1) | ((tile.B.h >> (7 - pixelIdx)) & 0x01);
    return this->palette[colorIdx];
}

pair Render::getTileRow(byte idx, byte rowIdx) {
    word addr = this->addrBase + (this->bgtile == 1 ? idx : (signedbyte)idx)*16 + rowIdx*2;

    pair p;
    p.B.h = this->readVRAM(addr);
    p.B.l = this->readVRAM(addr + 1);
    return p;
}
void Render::useTileset0() {
    this->addrBase = TILESET_O_BASE_ADDR;
    this->bgtile = 0;
}
void Render::useTileset1() {
    this->addrBase = TILESET_1_BASE_ADDR;
    this->bgtile = 1;
}
byte Render::readVRAM(word addr) {
    return this->VRAM[VRAM_BASE_ADDR + addr];
}

#define GRID 0
void Render::updateScreenPixel(u32 pixelX, byte pixelY, video::SColor color) {
//    this->screenImage->setPixel(pixelX, (u32)pixelY, color);
#if GRID
    if(pixelX%8 == 0 || pixelY%8 == 0) {
        this->texture[pixelX + (u32)pixelY*160] = this->palette[1].color;
    } else {
        this->texture[pixelX + (u32)pixelY*160] = this->palette[2].color;
    }
#else
    this->texture[pixelX + (u32)pixelY * 160] = color.color;
#endif
    
}

void Render::renderScanline() {
    
    this->texture = (u32*)this->screenTexture->lock();
    
    // VRAM offset for the tile map
    word mapoffset = this->bgtile == 1 ? BGMAP_1_BASE_ADDR : BGMAP_0_BASE_ADDR;
        
    // Which line of tiles to use in the map
    byte rowoffset = ((this->scanline + this->SCY) & 0xFF) >> 3;// y / 8 = y >> 3
    mapoffset += rowoffset << 5; // r*32 = r << 5
        
    // Which tile to start with in the map line
    byte lineoffset = (this->SCX >> 3); // x / 8 = x >> 3
    
    // Which line of pixels to use in the tiles
    byte pixelRowIdx = (this->scanline + this->SCY) & 0x07;
        
    // Where in the tileline to start
    byte x = this->SCX & 0x07;
    
    byte tileIdx = this->getTileIdx(mapoffset + lineoffset);
    pair tileRow = this->getTileRow(tileIdx, pixelRowIdx);
    for( u32 px = 0; px < 160; ++px) {
        // If the tile data set in use is #1, the
        // indices are signed; calculate a real tile offset
        //if(this->bgtile == 1) {
        //    tile = (byte)(((signedbyte)tile) + 256);
        //}
        
        
        video::SColor c = this->getPixelColor(tileRow, x);
        this->updateScreenPixel(px, this->scanline, c);
        ++x;
        if( x == 8) {
            x = 0;
            lineoffset = (lineoffset+1) & 0x1F;
            tileIdx = this->getTileIdx(mapoffset + lineoffset);
            tileRow = this->getTileRow(tileIdx, pixelRowIdx);
        }
    }
    this->screenTexture->unlock();
}

void Render::drawScreen() {
    this->driver->beginScene(true, true, video::SColor(255,33,12,0));
    core::rect<s32> src = core::rect<s32>(0,0,PIXEL_WIDTH,PIXEL_HEIGHT);
    core::rect<s32> dst = core::rect<s32>(0,0,PIXEL_WIDTH*WINDOW_SCALE,PIXEL_HEIGHT*WINDOW_SCALE);
    this->driver->enableMaterial2D();
    //this->driver->draw2DImage(this->screenTexture, dst.UpperLeftCorner, dst,0,video::SColor(225,255,255,255),true);
    this->driver->draw2DImage(this->screenTexture, dst,src);
    this->driver->enableMaterial2D(false);
    this->driver->endScene();
}

void Render::renderStep() {
    if(this->scanline < PIXEL_HEIGHT) {
        this->renderScanline();
        this->scanline++;
    } else {
        this->drawScreen();
        this->scanline = 0;
    }
}








