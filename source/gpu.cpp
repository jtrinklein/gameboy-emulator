#include <time.h>
#include "irrlicht.h"
#include "gpu.h"
#include "gameboy.h"
#include "mmu.h"
#include "pad.h"

#define PIXEL_WIDTH 160
#define PIXEL_HEIGHT 144
#define WINDOW_SCALE 3

#define NO_LCD 1
using namespace irr;

static u32 transparentColor = PIXEL_WHITE;
#ifdef NO_LCD
static u32 *pixelArray = nullptr;
#endif
GPU::GPU(Gameboy* g) {
    gb = g;
    OAM = gb->mmu->oam;
    VRAM = gb->mmu->vram;
    bgtile = 0;
    init();
    scanline = 0;
    SCX = 0;
    SCY = 0;
    
    bgmap = 0;
    bgtile = 0;
    clock = 0;
    setLCD(0x81);
    mode = MODE_OAM_READ;
}

GPU::~GPU() {
#ifdef NO_LCD
    if (pixelArray != nullptr) {
        delete [] pixelArray;
    }
#endif
}

void GPU::init() {
#ifdef NO_LCD
    if(pixelArray == nullptr) {
        pixelArray = new u32[0x6040];
    }
#endif
    colors[COLOR_BLACK] = video::SColor(PIXEL_BLACK);
    colors[COLOR_DARK_GREY] = video::SColor(PIXEL_DARK_GREY);
    colors[COLOR_LIGHT_GREY] = video::SColor(PIXEL_LIGHT_GREY);
    colors[COLOR_WHITE] = video::SColor(PIXEL_WHITE);
    
    bgPalette[0] = COLOR_WHITE;
    bgPalette[1] = COLOR_LIGHT_GREY;
    bgPalette[2] = COLOR_DARK_GREY;
    bgPalette[3] = COLOR_BLACK;
    
    oamPalette[0][0] = COLOR_WHITE;
    oamPalette[0][1] = COLOR_LIGHT_GREY;
    oamPalette[0][2] = COLOR_DARK_GREY;
    oamPalette[0][3] = COLOR_BLACK;
    
    oamPalette[1][0] = COLOR_WHITE;
    oamPalette[1][1] = COLOR_LIGHT_GREY;
    oamPalette[1][2] = COLOR_DARK_GREY;
    oamPalette[1][3] = COLOR_BLACK;
    
#ifndef NO_LCD
    device = irr::createDevice(video::EDT_OPENGL, core::dimension2d< u32 >(PIXEL_WIDTH*WINDOW_SCALE, PIXEL_HEIGHT*WINDOW_SCALE), 16, false, false, false, gb->pad);
    driver = device->getVideoDriver();
    screenImage = driver->createImage(video::ECF_A8R8G8B8, core::dimension2d< u32 >(PIXEL_WIDTH, PIXEL_HEIGHT));
    screenImage->fill(video::SColor(0xFF, 0xFF, 0xFF, 0xFF));
    screenTexture = driver->addTexture("GB_SCREEN", screenImage);
#endif
}

byte GPU::getTileIdx(word addr) {
    word idxAddr = mapBase + addr;
    return readVRAM(idxAddr);
}

video::SColor GPU::getBgPixelColor(pair tile, byte pixelIdx) {
    //low byte is first pixel section - (low bit)
    //high byte is second pixel section - (high bit)
    // pixel 0 is highest bit
    byte colorIdx = (((tile.B.h >> (7 - pixelIdx)) & 0x01)<< 1) | ((tile.B.l >> (7 - pixelIdx)) & 0x01);
    return colors[bgPalette[colorIdx]];
}

video::SColor GPU::getSpritePixelColor(Sprite s, pair tile, byte pixelIdx) {
    //low byte is first pixel section - (low bit)
    //high byte is second pixel section - (high bit)
    // pixel 0 is highest bit
    byte colorIdx = (((tile.B.h >> (7 - pixelIdx)) & 0x01)<< 1) | ((tile.B.l >> (7 - pixelIdx)) & 0x01);
    return colors[oamPalette[(s.flags & SPRITE_PALETTE)>>4][colorIdx]];
}

pair GPU::getSpriteTileRow(Sprite s, byte rowIdx){
    word addr = tileBase + (s.tile*16) + rowIdx*2;
    pair tile;
    tile.B.l = readVRAM(addr);
    tile.B.h = readVRAM(addr + 1);
    return tile;
}

pair GPU::getTileRow(byte idx, byte rowIdx) {
    word addr = tileBase + (bgtile ? idx : (signedbyte)idx)*16 + rowIdx*2;

    pair p;
    p.B.l = readVRAM(addr);
    p.B.h = readVRAM(addr + 1);
    return p;
}

bool GPU::running() {
#ifdef NO_LCD
    return true;
#else
    return device && device->run();
#endif
}

void GPU::useTileset(byte id) {
    tileBase = id ? TILESET_1_BASE_ADDR : TILESET_0_BASE_ADDR;
    bgtile = id;
}

void GPU::useBgMap(byte id) {
    bgmap = id;
    mapBase =id ? BGMAP_1_BASE_ADDR : BGMAP_0_BASE_ADDR;
    

}

byte GPU::readVRAM(word addr) {
    return VRAM[VRAM_BASE_ADDR + addr];
}

#define GRID 0

void GPU::updateScreenPixel(u32 pixelX, byte pixelY, video::SColor color) {
    texture[pixelX + (u32)pixelY * 160] = color.color;
}

void GPU::renderScanline() {
#ifdef NO_LCD
    texture = pixelArray;
#else
    texture = (u32*)screenTexture->lock();
#endif
    
    // VRAM offset for the tile map
    byte line = scanline + SCY;
    
    // Which line of tiles to use in the map
    byte rowoffset = line >> 3;// y / 8 = y >> 3
    word yTileOffset = rowoffset << 5; // r*32 = r << 5
        
    // Which tile to start with in the map line
    byte xTileOffset = (SCX >> 3) & 0x1F; // (x / 8)%32 = (x >> 3) & 0x1F
    
    // Which line of pixels to use in the tiles
    byte pixelRowIdx = line & 0x07;

    // Where in the tileline to start
    byte x = SCX & 0x07;
    
    byte tileIdx = getTileIdx(yTileOffset + xTileOffset);
    pair tileRow = getTileRow(tileIdx, pixelRowIdx);

    if(bgEnabled) {
        for( u32 px = 0; px < 160; ++px) {
            video::SColor c = getBgPixelColor(tileRow, x);
            updateScreenPixel(px, scanline, c);
            ++x;
            if( x == 8) {
                x = 0;
                xTileOffset = (xTileOffset+1) & 0x1F;
                tileIdx = getTileIdx(yTileOffset + xTileOffset);
                tileRow = getTileRow(tileIdx, pixelRowIdx);
            }
        }
    }
    if(spritesEnabled) {
        for (byte i = 0; i < 40; ++i) {
            Sprite s;
            s.y     = OAM[i*4] - 16;
            s.x     = OAM[i*4 + 1] - 8;
            s.tile  = OAM[i*4 + 2];
            s.flags = OAM[i*4 + 3];

            byte pixelRowIdx = (s.flags & SPRITE_Y_FLIP ? 7-(scanline - s.y) : (scanline - s.y)) & 0x7;
            
            // if sprite falls on scanline
            if(s.y <= scanline && (s.y+8) >= scanline) {
                pair tile = getSpriteTileRow(s, pixelRowIdx);

                for(byte x = 0; x < 8; ++x) {
                    byte pixelIdx = s.flags & SPRITE_X_FLIP ? 7-x : x;
                    byte px = s.x + pixelIdx;

                    bool bgIsTrans = pixelIsTransparent(px, scanline);
                    if (px < PIXEL_WIDTH && (bgIsTrans || s.flags & SPRITE_PRIORITY)) {
                        video::SColor sc = getSpritePixelColor(s, tile, pixelIdx);
                        if (sc.color != colors[bgPalette[0]].color) {
                            updateScreenPixel(px, scanline, sc);
                        }
                    }
                }
                
            }
        }
    }
#ifndef NO_LCD
    screenTexture->unlock();
#endif
}
bool GPU::pixelIsTransparent(byte x, byte y) {
    return texture[x + y * PIXEL_WIDTH] == colors[bgPalette[0]].color;
}

void GPU::drawScreen() {
#ifndef NO_LCD
    driver->beginScene(true, true, video::SColor(255,33,12,0));
    core::rect<s32> src = core::rect<s32>(0,0,PIXEL_WIDTH,PIXEL_HEIGHT);
    core::rect<s32> dst = core::rect<s32>(0,0,PIXEL_WIDTH*WINDOW_SCALE,PIXEL_HEIGHT*WINDOW_SCALE);
    driver->draw2DImage(screenTexture, dst,src);
    driver->endScene();
#endif
}

byte GPU::getLCD() {
    return LCD;
}

void GPU::setLCD(byte data) {
    LCD = data;
    bgEnabled = data & 1;
    spritesEnabled = (data >> 1) & 1;
    largeSprites = (data >> 2) & 1;
    useBgMap((data >> 3) & 1);
    useTileset((data >> 4) & 1);
    windowEnabled = (data >> 5) & 1;
    windowMap = (data >> 6) & 1;
    displayEnabled = (data >> 7) & 1;
}

void GPU::setBgPalette(byte data) {
    bgPalette[0] = data & 3;
    bgPalette[1] = (data >> 2) & 3;
    bgPalette[2] = (data >> 4) & 3;
    bgPalette[3] = (data >> 6) & 3;
}

void GPU::setOamPalette(byte id, byte data) {
    oamPalette[id][0] = data & 3;
    oamPalette[id][1] = (data >> 2) & 3;
    oamPalette[id][2] = (data >> 4) & 3;
    oamPalette[id][3] = (data >> 6) & 3;
}

#define OAM_CYCLES 80
#define VRAM_CYCLES 172
#define HBLANK_CYCLES 204
#define VBLANK_CYCLES 456

void GPU::checkLYC() {
    if (scanline == gb->mmu->lyc) {
        LCD |= LCD_COINCIDENCE;
        
        if (LCD & LCD_LYC_IRQ_ENABLED) {
            gb->mmu->IF |= IR_LCD;
        }
    } else {
        LCD &= ~LCD_COINCIDENCE;
    }
    
}
void GPU::renderStep(byte cycleDelta) {
    clock += cycleDelta;
    switch (mode) {
        case MODE_OAM_READ:
            if(clock >= OAM_CYCLES) {
                clock = 0;
                mode = MODE_VRAM_READ;
            }
            break;
        case MODE_VRAM_READ:
            if(clock >= VRAM_CYCLES) {
                clock = 0;
                mode = MODE_HBLANK;
                renderScanline();
                if ((gb->mmu->IE & IR_LCD) && (LCD & STAT_MODE_HBLANK_IRQ_ENABLED)) {
                    gb->mmu->IF |= IR_LCD;
                }
            }
            break;
        case MODE_HBLANK:
            if (clock >= HBLANK_CYCLES) {
                clock = 0;
                scanline++;
                checkLYC();
                if (scanline < PIXEL_HEIGHT) {
                    mode = MODE_OAM_READ;
                } else {
                    mode = MODE_VBLANK;
                    drawScreen();
                    
                    gb->mmu->IF |= IR_LCD;
                    
                    if ((gb->mmu->IE & IR_LCD) && (LCD & STAT_MODE_VBLANK_IRQ_ENABLED)) {
                        gb->mmu->IF |= IR_LCD;
                    }
                }
            }
            break;
        case MODE_VBLANK:
            if(clock >= VBLANK_CYCLES) {
                clock = 0;
                scanline++;
                if (scanline == 154) {
                    mode = MODE_OAM_READ;
                    scanline = 0;
                    clock = 0;
                    
                    if ((gb->mmu->IE & IR_LCD) && (LCD & STAT_MODE_VRAM_OAM_IRQ_ENABLED)) {
                        gb->mmu->IF |= IR_LCD;
                    }
                }
            }
            break;
    }
}








