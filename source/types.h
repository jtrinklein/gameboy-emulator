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

/* Bits in Z80 F register:    */
#define FLAG_Z      0x80       /* 1: Result is zero          */
#define FLAG_N      0x40       /* 1: Subtraction occured     */
#define FLAG_H      0x20       /* 1: Halfcarry               */
#define FLAG_C      0x10       /* 1: Carry/Borrow occured    */

#define RS_RUN      0x00
#define RS_HALT     0x01       /* 1: halt enabled            */
#define RS_STOP     0x02       /* 1: stop enabled            */

#define IE_ENABLED  0xFF       /* 1: interrupts enabled      */

#define IR_VBLANK   0x01
#define IR_LCD      0x02
#define IR_TIMER    0x04
#define IR_SERIAL   0x08
#define IR_JOYPAD   0x10

#define DEF_ROM_OFFSET 0x4000
#define DEF_RAM_OFFSET 0x0000

typedef unsigned char byte;
typedef unsigned short word;
typedef signed char signedbyte;

typedef union {
    struct { byte l,h;} B;
    word W;
} pair;

class Render;
class MMU;
class CPU;
class Cart;

#endif

