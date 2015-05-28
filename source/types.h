#ifndef __GBTYPES_H__
#define __GBTYPES_H__

/* interruptCheck() may return:      */
#define INT_RST00   0xC7     /* RST 00h                    */
#define INT_RST08   0xCF     /* RST 08h                    */
#define INT_RST10   0xD7     /* RST 10h                    */
#define INT_RST18   0xDF     /* RST 18h                    */
#define INT_RST20   0xE7     /* RST 20h                    */
#define INT_RST28   0xEF     /* RST 28h                    */
#define INT_RST30   0xF7     /* RST 30h                    */
#define INT_RST38   0xFF     /* RST 38h                    */
#define INT_IRQ     INT_RST38  /* Default IRQ opcode is 0xFF */
#define INT_NMI     0xFFFD     /* Non-maskable interrupt     */
#define INT_NONE    0xFFFF     /* No interrupt required      */
#define INT_QUIT    0xFFFE     /* Exit the emulation         */

#define IRV_VBLANK  0x0040
#define IRV_LCDC    0x0048
#define IRV_TIMER   0x0050
#define IRV_SERIAL  0x0058
#define IRV_JOYPAD  0x0060

/* Bits in Z80 F register:    */
#define FLAG_Z      0x80       /* 1: Result is zero          */
#define FLAG_N      0x40       /* 1: Subtraction occured     */
#define FLAG_H      0x20       /* 1: Halfcarry               */
#define FLAG_C      0x10       /* 1: Carry/Borrow occured    */

#define RS_RUN      0x00
#define RS_HALT     0x01       /* 1: halt enabled            */
#define RS_STOP     0x02       /* 1: stop enabled            */

#define IE_ENABLED  0xFF       /* 1: interrupts enabled      */
#define IE_DISABLED 0x00

#define IR_VBLANK   0x01
#define IR_LCD      0x02
#define IR_TIMER    0x04
#define IR_SERIAL   0x08
#define IR_JOYPAD   0x10

#define INPUT_SRC_MASK   0x30
#define INPUT_SRC_DPAD   0x20
#define INPUT_SRC_BTNS   0x10

#define INPUT_RIGHT      0x01
#define INPUT_LEFT       0x02
#define INPUT_UP         0x04
#define INPUT_DOWN       0x08

#define INPUT_BTN_A      0x01
#define INPUT_BTN_B      0x02
#define INPUT_BTN_SELECT 0x04
#define INPUT_BTN_START  0x08


#define LCD_COINCIDENCE                 0x04
#define LCD_LYC_IRQ_ENABLED             0x40
#define STAT_MODE_HBLANK_IRQ_ENABLED    0x08
#define STAT_MODE_VBLANK_IRQ_ENABLED    0x10
#define STAT_MODE_VRAM_OAM_IRQ_ENABLED  0x20

#define DEF_ROM_OFFSET 0x4000
#define DEF_RAM_OFFSET 0x0000

typedef unsigned char byte;
typedef unsigned short word;
typedef signed char signedbyte;

typedef union {
    struct { byte l,h;} B;
    word W;
} pair;

class Gameboy;
class GPU;
class MMU;
class CPU;
class Cart;
class Pad;
class APU;

#endif

