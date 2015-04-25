#ifndef __GAMEBOY_H__
#define __GAMEBOY_H__
#ifdef __cplusplus
extern "C" {
#endif
                               /* interruptCheck() may return:      */
#define INT_RST00   0x00C7     /* RST 00h                    */
#define INT_RST08   0x00CF     /* RST 08h                    */
#define INT_RST10   0x00D7     /* RST 10h                    */
#define INT_RST18   0x00DF     /* RST 18h                    */
#define INT_RST20   0x00E7     /* RST 20h                    */
#define INT_RST28   0x00EF     /* RST 28h                    */
#define INT_RST30   0x00F7     /* RST 30h                    */
#define INT_RST38   0x00FF     /* RST 38h                    */
#define INT_IRQ     INT_RST38  /* Default IRQ opcode is 0xFF */
#define INT_NMI     0xFFFD     /* Non-maskable interrupt     */
#define INT_NONE    0xFFFF     /* No interrupt required      */
#define INT_QUIT    0xFFFE     /* Exit the emulation         */

                               /* Bits in Z80 F register:    */
#define FLAG_Z      0x80       /* 1: Result is zero          */
#define FLAG_N      0x40       /* 1: Subtraction occured     */
#define FLAG_H      0x20       /* 1: Halfcarry               */
#define FLAG_C      0x10       /* 1: Carry/Borrow occured    */

#define IF_HALT     0x01       /* 1: halt enabled            */
#define IF_STOP     0x02       /* 1: stop enabled            */
#define IF_IE       0x04       /* 1: interrupts enabled      */


typedef unsigned char byte;
typedef unsigned short word;
typedef signed char signedbyte;

typedef union {
    struct { byte h,l;} B;
    word W;
} pair;
class Render;
typedef struct {
    byte *ROM, *ZRAM, *WRAM, *ERAM, *VRAM, *OAM, *BIOS;
    Render *gpu;
    bool inBios;
    
    pair AF, BC, DE, HL, SP, PC;
    byte opCycles;
    
    int IPeriod,ICycles;
    word IRequest;      // set to adress of pending IRQ
    byte IAutoReset;    // set to 1 to automatically reset IRequest
    byte IF; // Interrupt flag register
} CPU;

const int DEFAULT_INTERRUPT_PERIOD = 175782; // from 2.6GHz processor
const int INITIAL_PC = 0;

bool execOp(CPU* cpu, Render* gpu, byte I);
void runGB(CPU* gb);

word interruptCheck(CPU* gb);
void go();
#ifdef __cplusplus
}
#endif
#endif
