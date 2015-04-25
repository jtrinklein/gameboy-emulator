#include "irrlicht.h"
#include "render.h"

using namespace irr;

Render::Render(byte* vram, byte* oam) {
    this->OAM = oam;
    this->VRAM = vram;
    this->bgtile = 0;
    this->init();
    this->scanline = 0;
    this->SCX = 0;
    this->SCY = 0;
    this->bgmap = 0;
    this->bgtile = 0;
    this->clock = 0;
    this->setLCD(0x81);
    this->mode = MODE_OAM_READ;
}

#define PIXEL_WIDTH 160
#define PIXEL_HEIGHT 144
#define WINDOW_SCALE 3

Render::~Render() {
}

void Render::init() {
    this->colors[COLOR_BLACK] = video::SColor(PIXEL_BLACK);
    this->colors[COLOR_DARK_GREY] = video::SColor(PIXEL_DARK_GREY);
    this->colors[COLOR_LIGHT_GREY] = video::SColor(PIXEL_LIGHT_GREY);
    this->colors[COLOR_WHITE] = video::SColor(PIXEL_WHITE);
    
    this->palette[0] = COLOR_WHITE;
    this->palette[1] = COLOR_LIGHT_GREY;
    this->palette[2] = COLOR_DARK_GREY;
    this->palette[3] = COLOR_BLACK;
    
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
    return this->colors[this->palette[colorIdx]];
}

pair Render::getTileRow(byte idx, byte rowIdx) {
    word addr = this->tileBase + (this->bgtile ? idx : (signedbyte)idx)*16 + rowIdx*2;

    pair p;
    p.B.h = this->readVRAM(addr);
    p.B.l = this->readVRAM(addr + 1);
    return p;
}

bool Render::running() {
    return this->device && this->device->run();
}

void Render::useTileset(byte id) {
    if(id) {
        this->tileBase = TILESET_1_BASE_ADDR;
        this->bgtile = 1;
    } else {
        this->tileBase = TILESET_O_BASE_ADDR;
        this->bgtile = 0;
    }
}

void Render::useBgMap(byte id) {
    this->bgmap = id;
    this->mapBase =id ? BGMAP_1_BASE_ADDR : BGMAP_0_BASE_ADDR;
    

}

byte Render::readVRAM(word addr) {
    return this->VRAM[VRAM_BASE_ADDR + addr];
}

#define GRID 0

void Render::updateScreenPixel(u32 pixelX, byte pixelY, video::SColor color) {

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
    word mapoffset = this->mapBase;
        
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

byte Render::getLCD() {
    return this->LCD;
}

void Render::setLCD(byte data) {
    this->LCD = data;
    this->bgEnabled = data & 1;
    this->spritesEnabled = (data >> 1) & 1;
    this->largeSprites = (data >> 2) & 1;
    this->useBgMap((data >> 3) & 1);
    this->useTileset((data >> 4) & 1);
    this->windowEnabled = (data >> 5) & 1;
    this->windowMap = (data >> 6) & 1;
    this->displayEnabled = (data >> 7) & 1;
}

void Render::setPalette(byte data) {
    this->palette[0] = data & 3;
    this->palette[1] = (data >> 1) & 3;
    this->palette[2] = (data >> 2) & 3;
    this->palette[3] = (data >> 3) & 3;
}

#define OAM_CYCLES 80
#define VRAM_CYCLES 172
#define HBLANK_CYCLES 204
#define VBLANK_CYCLES 456


void Render::renderStep(byte cycleDelta) {
    this->clock += cycleDelta;
    
    switch (this->mode) {
        case MODE_OAM_READ:
            if(this->clock >= OAM_CYCLES) {
                this->clock -= OAM_CYCLES;
                this->mode = MODE_VRAM_READ;
            }
            break;
        case MODE_VRAM_READ:
            if(this->clock >= VRAM_CYCLES) {
                this->clock -= VRAM_CYCLES;
                this->mode = MODE_HBLANK;
                this->renderScanline();
            }
            break;
        case MODE_HBLANK:
            if (this->clock >= HBLANK_CYCLES) {
                this->clock -= HBLANK_CYCLES;
                this->scanline++;
                if (this->scanline < PIXEL_HEIGHT) {
                    this->mode = MODE_OAM_READ;
                } else {
                    this->mode = MODE_VBLANK;
                    this->drawScreen();
                }
            }
            break;
        case MODE_VBLANK:
            if(this->clock >= VBLANK_CYCLES) {
                this->clock -= VBLANK_CYCLES;
                this->scanline++;
                if (this->scanline == (PIXEL_HEIGHT + 10)) {
                    this->mode = MODE_OAM_READ;
                    this->scanline = 0;
                }
            }
            break;
    }
}








