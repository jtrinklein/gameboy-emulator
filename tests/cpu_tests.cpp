#include <iostream>
#include <iomanip>
#include <functional>
#include "tests.h"
#include "gameboy.h"
#include "render.h"

using namespace std;
unsigned assertFail = 0;
unsigned assertPass = 0;
CPU* cpu;
string dots = "........................................";
#define ID_NEQUAL 1

#define STR(x) #x
#define JOIN(x,y) x##y

#define eql(actual, expected) \
do {\
    if(actual != expected) {\
        assertFail++;\
        cout << "FAIL" << endl;\
        cout << STR(actual) << " != " << STR(expected) << endl;\
        cout << hex << actual << " should equal " << expected << endl;\
    } else {\
        assertPass++;\
    }\
} while(false)

#define fnotset(f) eql((cpu->AF.B.l & f), 0)

#define fset(f) eql((cpu->AF.B.l & f), f)
#define TEST(fn)\
beforeTest();\
do {\
    unsigned oldPass = assertPass, oldFail = assertFail;\
    cout << #fn << dots.substr(0, 40 - getTitleLength( #fn ));\
    fn();\
    if(oldFail == assertFail) {\
        pass++;\
        if (assertPass == oldPass) {\
            cout << "WARN" << endl;\
            cout << "0 assertions run" << endl;\
        } else {\
            cout << "OK" << endl;\
        }\
    } else {\
        fail++;\
    }\
    tests++;\
} while(false)


unsigned long getTitleLength(string title) {
    return title.length();
}

CPU* setup() {
    CPU* c = new CPU();
    c->OAM  = new byte[0x0100];
    c->ZRAM = new byte[0x0100];
    c->WRAM = new byte[0x2000];
    c->ERAM = new byte[0x2000];
    c->VRAM = new byte[0x2000];
    c->ROM  = new byte[0x8000];
    c->gpu = new Render(c->VRAM, c->OAM);
    return c;
}

void cleanup(CPU* cpu) {
    delete cpu->gpu;
    delete [] cpu->ROM;
    delete [] cpu->OAM;
    delete [] cpu->WRAM;
    delete [] cpu->ZRAM;
    delete [] cpu->ERAM;
    delete [] cpu->VRAM;
    delete cpu;
}

void beforeTest() {
    //clear rom
    memset(cpu->ROM, 0, 0x8000);
    
    //reset cpu
    initCPU(cpu);
    
    //move SP to 0xDFFF
    cpu->SP.W = 0xDFFF;
}


byte read(word a) {
    return readMem(cpu, a);
}
void rom(word a, byte v) {
    writeMem(cpu, a, v);
    //cpu->ROM[a] = v;
}

void setFlag(byte flag) {
    cpu->AF.B.l |= flag;
}

void clearFlag(byte flag) {
    cpu->AF.B.l &= ~flag;
}

void runOps(byte opCount) {
    while (opCount-- > 0) {
        byte op = readMem(cpu, cpu->PC.W++);
        execOp(cpu, op);
    }
}
void CALL() {
    byte ops = 1;
    rom(0x100, 0xCD);//CALL a16
    rom(0x101, 0x34);
    rom(0x102, 0x12);//0x1234
    rom(0x103, 0x00);//NOP
    
    runOps(ops);
    
    eql(cpu->PC.W, 0x1234);
    eql(cpu->SP.W, 0xDFFD);
    eql(read(0xDFFE), 0x03);
    eql(read(0xDFFF), 0x01);
}

void CALL_Z() {
    byte ops = 1;
    
    setFlag(FLAG_Z);
    
    rom(0x100, 0xCC);//CALL Z,a16
    rom(0x101, 0x34);
    rom(0x102, 0x12);//0x1234
    
    runOps(ops);
    
    eql(cpu->PC.W, 0x1234);
    eql(cpu->SP.W, 0xDFFD);
    eql(read(0xDFFE), 0x03);
    eql(read(0xDFFF), 0x01);
}


void CALL_Z_NOJUMP() {
    byte ops = 1;
    
    clearFlag(FLAG_Z);
    
    rom(0x100, 0xCC);//CALL Z,a16
    rom(0x101, 0x34);
    rom(0x102, 0x12);//0x1234
    rom(0x103, 0x00);
    
    runOps(ops);
    
    eql(cpu->PC.W, 0x103);
    eql(cpu->SP.W, 0xDFFF);
}

void CALL_NZ() {
    byte ops = 1;
    
    clearFlag(FLAG_Z);
    
    rom(0x100, 0xC4);//CALL NZ,a16
    rom(0x101, 0x34);
    rom(0x102, 0x12);//0x1234
    
    runOps(ops);
    
    eql(cpu->PC.W, 0x1234);
    eql(cpu->SP.W, 0xDFFD);
    eql(read(0xDFFE), 0x03);
    eql(read(0xDFFF), 0x01);
}


void CALL_NZ_NOJUMP() {
    byte ops = 1;
    
    setFlag(FLAG_Z);
    
    rom(0x100, 0xC4);//CALL NZ,a16
    rom(0x101, 0x34);
    rom(0x102, 0x12);//0x1234
    rom(0x103, 0x00);
    
    runOps(ops);
    
    eql(cpu->PC.W, 0x103);
    eql(cpu->SP.W, 0xDFFF);
}

void CALL_C() {
    byte ops = 1;
    
    setFlag(FLAG_C);
    
    rom(0x100, 0xDC);//CALL C,a16
    rom(0x101, 0x34);
    rom(0x102, 0x12);//0x1234
    
    runOps(ops);
    
    eql(cpu->PC.W, 0x1234);
    eql(cpu->SP.W, 0xDFFD);
    eql(read(0xDFFE), 0x03);
    eql(read(0xDFFF), 0x01);
}


void CALL_C_NOJUMP() {
    byte ops = 1;
    
    clearFlag(FLAG_C);
    
    rom(0x100, 0xDC);//CALL C,a16
    rom(0x101, 0x34);
    rom(0x102, 0x12);//0x1234
    rom(0x103, 0x00);
    
    runOps(ops);
    
    eql(cpu->PC.W, 0x103);
    eql(cpu->SP.W, 0xDFFF);
}

void CALL_NC() {
    byte ops = 1;
    
    clearFlag(FLAG_C);
    
    rom(0x100, 0xD4);//CALL NC,a16
    rom(0x101, 0x34);
    rom(0x102, 0x12);//0x1234
    
    runOps(ops);
    
    eql(cpu->PC.W, 0x1234);
    eql(cpu->SP.W, 0xDFFD);
    eql(read(0xDFFE), 0x03);
    eql(read(0xDFFF), 0x01);
}

void CALL_NC_NOJUMP() {
    byte ops = 1;
    
    setFlag(FLAG_C);
    
    rom(0x100, 0xD4);//CALL NC,a16
    rom(0x101, 0x34);
    rom(0x102, 0x12);//0x1234
    rom(0x103, 0x00);
    
    runOps(ops);
    
    eql(cpu->PC.W, 0x103);
    eql(cpu->SP.W, 0xDFFF);
}

void CALL_RET() {
    byte ops = 2;
    rom(0x100, 0xCD); //CALL
    rom(0x101, 0x34);
    rom(0x102, 0x12); //0x1234
    rom(0x103, 0x00); //NOP
    rom(0x1234, 0xC9);//RET
    
    runOps(ops);
    
    eql(cpu->PC.W, 0x103);
    eql(cpu->SP.W, 0xDFFF);
}


void JP() {
    byte ops = 1;
    rom(0x100, 0xC3); //JP
    rom(0x101, 0x00);
    rom(0x102, 0x7A); //0x7A00
    rom(0x103, 0x00); //NOP
    
    runOps(ops);
    
    eql(cpu->PC.W, (word)0x7A00);
    eql(cpu->SP.W, (word)0xDFFF);
}


void JP_Z_NOJUMP() {
    byte ops = 1;
    
    clearFlag(FLAG_Z);
    
    rom(0x100, 0xCA); //JP Z, a16
    rom(0x101, 0x00);
    rom(0x102, 0x7A); //0x7A00
    rom(0x103, 0x00); //NOP
    
    runOps(ops);
    
    eql(cpu->PC.W, (word)0x103);
}

void JP_Z_JUMP() {
    byte ops = 1;
    
    setFlag(FLAG_Z);
    
    rom(0x100, 0xCA); //JP Z, a16
    rom(0x101, 0x00);
    rom(0x102, 0x7A); //0x7A00
    rom(0x103, 0x00); //NOP
    
    runOps(ops);
    
    eql(cpu->PC.W, (word)0x7A00);
    fset(FLAG_Z);
}

void JP_NZ_JUMP() {
    byte ops = 1;
    
    clearFlag(FLAG_Z);
    
    rom(0x100, 0xC2); //JP NZ, a16
    rom(0x101, 0x00);
    rom(0x102, 0x7A); //0x7A00
    rom(0x103, 0x00); //NOP
    
    runOps(ops);
    
    eql(cpu->PC.W, (word)0x7A00);
}

void JP_NZ_NOJUMP() {
    byte ops = 1;
    
    setFlag(FLAG_Z);
    
    rom(0x100, 0xC2); //JP NZ, a16
    rom(0x101, 0x00);
    rom(0x102, 0x7A); //0x7A00
    rom(0x103, 0x00); //NOP
    
    runOps(ops);
    
    eql(cpu->PC.W, 0x103);
}

void JP_C_NOJUMP() {
    byte ops = 1;
    
    clearFlag(FLAG_C);
    
    rom(0x100, 0xDA); //JP C, a16
    rom(0x101, 0x00);
    rom(0x102, 0x7A); //0x7A00
    rom(0x103, 0x00); //NOP
    
    runOps(ops);
    
    eql(cpu->PC.W, 0x103);
}

void JP_C_JUMP() {
    byte ops = 1;
    
    setFlag(FLAG_C);
    
    rom(0x100, 0xDA); //JP C, a16
    rom(0x101, 0x00);
    rom(0x102, 0x7A); //0x7A00
    rom(0x103, 0x00); //NOP
    
    runOps(ops);
    
    eql(cpu->PC.W, 0x7A00);
}

void JP_NC_JUMP() {
    byte ops = 1;
    
    clearFlag(FLAG_C);
    
    rom(0x100, 0xD2); //JP NC, a16
    rom(0x101, 0x00);
    rom(0x102, 0x7A); //0x7A00
    rom(0x103, 0x00); //NOP
    
    runOps(ops);
    
    eql(cpu->PC.W, (word)0x7A00);
}

void JP_NC_NOJUMP() {
    byte ops = 1;
    
    setFlag(FLAG_C);
    
    rom(0x100, 0xD2); //JP NC, a16
    rom(0x101, 0x00);
    rom(0x102, 0x7A); //0x7A00
    rom(0x103, 0x00); //NOP
    
    runOps(ops);
    
    eql(cpu->PC.W, 0x103);
}

void JP_HL() {
    byte ops = 1;
    cpu->HL.W = 0x1243;
    
    rom(0x100, 0xE9); //JP (HL)
    
    runOps(ops);
    
    eql(cpu->PC.W, 0x1243);
}


void JR() {
    byte ops = 1;
    rom(0x100, 0x18); //JR
    rom(0x101, 0x50); //+0x50
    rom(0x102, 0x00); //NOP
    rom(0x152, 0x00); //NOP
    
    runOps(ops);
    
    eql(cpu->PC.W, 0x152);
}


void JR_NEG() {
    byte ops = 1;
    rom(0xB2, 0x00); //NOP
    rom(0x100, 0x18); //JR
    rom(0x101, (byte)-0x50); //-0x50
    rom(0x102, 0x00); //NOP
    rom(0x0B2, 0x00); //NOP
    
    runOps(ops);
    
    eql(cpu->PC.W, 0xB2);
}

void JR_Z() {
    byte ops = 1;
    setFlag(FLAG_Z);
    rom(0x100, 0x28); //JR z
    rom(0x101, 0x50); //+0x50
    rom(0x102, 0x00); //NOP
    rom(0x152, 0x00); //NOP
    
    runOps(ops);
    
    eql(cpu->PC.W, 0x152);
}


void JR_Z_NOJUMP() {
    byte ops = 1;
    clearFlag(FLAG_Z);
    rom(0x100, 0x28); //JR z
    rom(0x101, 0x50); //+0x50
    rom(0x102, 0x00); //NOP
    rom(0x152, 0x00); //NOP
    
    runOps(ops);
    
    eql(cpu->PC.W, 0x102);
}

void JR_NZ() {
    byte ops = 1;
    clearFlag(FLAG_Z);
    rom(0x100, 0x20); //JR nz
    rom(0x101, 0x54); //+0x54
    rom(0x102, 0x00); //NOP
    rom(0x156, 0x00); //NOP
    
    runOps(ops);
    
    eql(cpu->PC.W, 0x156);
}

void JR_NZ_NOJUMP() {
    byte ops = 1;
    setFlag(FLAG_Z);
    rom(0x100, 0x20); //JR nz
    rom(0x101, 0x54); //+0x54
    rom(0x102, 0x00); //NOP
    rom(0x156, 0x00); //NOP
    
    runOps(ops);
    
    eql(cpu->PC.W, 0x102);
}

void JR_C() {
    byte ops = 1;
    setFlag(FLAG_C);
    rom(0x100, 0x38); //JR c
    rom(0x101, 0x50); //+0x50
    rom(0x102, 0x00); //NOP
    rom(0x152, 0x00); //NOP
    
    runOps(ops);
    
    eql(cpu->PC.W, 0x152);
}


void JR_C_NOJUMP() {
    byte ops = 1;
    clearFlag(FLAG_C);
    rom(0x100, 0x38); //JR c
    rom(0x101, 0x50); //+0x50
    rom(0x102, 0x00); //NOP
    rom(0x152, 0x00); //NOP
    
    runOps(ops);
    
    eql(cpu->PC.W, 0x102);
}

void JR_NC() {
    byte ops = 1;
    clearFlag(FLAG_C);
    rom(0x100, 0x30); //JR nc
    rom(0x101, 0x54); //+0x54
    rom(0x102, 0x00); //NOP
    rom(0x156, 0x00); //NOP
    
    runOps(ops);
    
    eql(cpu->PC.W, 0x156);
}

void JR_NC_NOJUMP() {
    byte ops = 1;
    setFlag(FLAG_C);
    rom(0x100, 0x30); //JR nc
    rom(0x101, 0x54); //+0x54
    rom(0x102, 0x00); //NOP
    rom(0x156, 0x00); //NOP
    
    runOps(ops);
    
    eql(cpu->PC.W, 0x102);
}
void PUSH_BC() {
    byte ops = 1;
    
    cpu->BC.W = 0x0C80;
    
    rom(0x100, 0xC5); //PUSH bc
    rom(0x101, 0x00); //NOP
    rom(0xDFFF, 0x00);
    rom(0xDFFE, 0x00);
    
    runOps(ops);
    
    eql(cpu->PC.W, 0x101);
    eql(cpu->SP.W, 0xDFFD);
    eql(read(0xDFFE), 0x80); // low bytes written second
    eql(read(0xDFFF), 0x0C); // high bytes written first
}
void PUSH_DE() {
    byte ops = 1;
    
    cpu->DE.W = 0x0C80;
    
    rom(0x100, 0xD5); //PUSH de
    rom(0x101, 0x00); //NOP
    rom(0xDFFF, 0x00);
    rom(0xDFFE, 0x00);
    
    runOps(ops);
    
    eql(cpu->PC.W, 0x101);
    eql(cpu->SP.W, 0xDFFD);
    eql(read(0xDFFE), 0x80);
    eql(read(0xDFFF), 0x0C);
}
void PUSH_HL() {
    byte ops = 1;
    
    cpu->HL.W = 0x8CFF;
    
    rom(0x100, 0xE5); //PUSH HL
    rom(0x101, 0x00); //NOP
    rom(0xDFFF, 0x00);
    rom(0xDFFE, 0x00);
    
    runOps(ops);
    
    eql(cpu->PC.W, 0x101);
    eql(cpu->SP.W, 0xDFFD);
    eql(read(0xDFFE), 0xFF);
    eql(read(0xDFFF), 0x8C);
}
void PUSH_AF() {
    byte ops = 1;
    
    cpu->AF.W = 0x8C1F;
    
    rom(0x100, 0xF5); //PUSH AF
    rom(0x101, 0x00); //NOP
    rom(0xDFFF, 0x00);
    rom(0xDFFE, 0x00);
    
    runOps(ops);
    
    eql(cpu->PC.W, 0x101);
    eql(cpu->SP.W, 0xDFFD);
    eql(read(0xDFFE), 0x1F);
    eql(read(0xDFFF), 0x8C);
}
void POP_BC() {
    byte ops = 1;
    
    cpu->SP.W = 0xDFFD;
    cpu->BC.W = 0x0000;

    rom(0x100, 0xC1); //POP bc
    rom(0x101, 0x00); //NOP
    rom(0xDFFF, 0x80);
    rom(0xDFFE, 0x0C);
    
    runOps(ops);

    eql(cpu->PC.W, 0x101);
    eql(cpu->SP.W, 0xDFFF);
    eql(cpu->BC.W, 0x800C);
}
void POP_DE() {
    byte ops = 1;
    
    cpu->SP.W = 0xDFFD;
    cpu->DE.W = 0x0C80;

    rom(0x100, 0xD1); //POP de
    rom(0x101, 0x00); //NOP
    rom(0xDFFF, 0x80);
    rom(0xDFFE, 0x0C);
    
    runOps(ops);
    
    eql(cpu->PC.W, 0x101);
    eql(cpu->SP.W, 0xDFFF);
    eql(cpu->DE.W, 0x800C);
}
void POP_HL() {
    byte ops = 1;
    
    cpu->SP.W = 0xDFFD;
    cpu->HL.W = 0x0000;

    rom(0x100, 0xE1); //POP HL
    rom(0x101, 0x00); //NOP
    rom(0xDFFF, 0xFF);
    rom(0xDFFE, 0x8C);
    
    runOps(ops);
    
    eql(cpu->PC.W, 0x101);
    eql(cpu->SP.W, 0xDFFF);
    eql(cpu->HL.W, 0xFF8C);
}
void POP_AF() {
    byte ops = 1;
    
    cpu->SP.W = 0xDFFD;
    cpu->AF.W = 0x0000;
    
    rom(0x100, 0xF1); //POP AF
    rom(0x101, 0x00); //NOP
    rom(0xDFFF, 0x31);
    rom(0xDFFE, 0xF6);
    
    runOps(ops);
    
    eql(cpu->PC.W, 0x101);
    eql(cpu->SP.W, 0xDFFF);
    eql(cpu->AF.W, 0x31F6);
}
void RET() {
    byte ops = 1;
    cpu->SP.W = 0xDFFD;
    rom(0xDFFE, 0xFF);
    rom(0xDFFF, 0x01);
    rom(0x100, 0xC9); //RET
    rom(0x101, 0x00);
    rom(0x1FF, 0x00);
    
    runOps(ops);

    eql(cpu->PC.W, 0x1FF);
}

void LD_BC() {
    byte ops = 1;
    cpu->BC.W = 0x0000;
    
    rom(0x100, 0x01);//LD bc
    rom(0x101, 0xF0);
    rom(0x102, 0x80);//0x80F0
    
    runOps(ops);
    
    eql(cpu->PC.W, 0x103);
    eql(cpu->BC.W, 0x80F0);
}
void LD_DE() {
    byte ops = 1;
    cpu->DE.W = 0x0000;
    
    rom(0x100, 0x11);//LD de
    rom(0x101, 0xF0);
    rom(0x102, 0x80);//0x80F0
    
    runOps(ops);
    
    eql(cpu->PC.W, 0x103);
    eql(cpu->DE.W, 0x80F0);
}
void LD_HL() {
    byte ops = 1;
    cpu->HL.W = 0x0000;
    
    rom(0x100, 0x21);//LD hl
    rom(0x101, 0xF0);
    rom(0x102, 0x80);//0x80F0
    
    runOps(ops);
    
    eql(cpu->PC.W, 0x103);
    eql(cpu->HL.W, 0x80F0);
}
void LD_SP() {
    byte ops = 1;
    
    rom(0x100, 0x31);//LD sp
    rom(0x101, 0xF0);
    rom(0x102, 0x80);//0x80F0
    
    runOps(ops);
    
    eql(cpu->PC.W, 0x103);
    eql(cpu->SP.W, 0x80F0);
}

void LD_B() {
    byte ops = 1;
    cpu->BC.W = 0x0000;
    
    rom(0x100, 0x06);//LD b
    rom(0x101, 0xF2);//0xF2
    rom(0x102, 0x00);
    
    runOps(ops);
    
    eql(cpu->PC.W, 0x102);
    eql(cpu->BC.B.h, 0xF2);
}

void LD_D() {
    byte ops = 1;
    cpu->BC.W = 0x0000;
    
    rom(0x100, 0x16);//LD d
    rom(0x101, 0xF2);//0xF2
    rom(0x102, 0x00);
    
    runOps(ops);
    
    eql(cpu->PC.W, 0x102);
    eql(cpu->DE.B.h, 0xF2);
}
void LD_H() {
    byte ops = 1;
    cpu->HL.W = 0x0000;
    
    rom(0x100, 0x26);//LD h
    rom(0x101, 0xF2);//0xF2
    rom(0x102, 0x00);
    
    runOps(ops);
    
    eql(cpu->PC.W, 0x102);
    eql(cpu->HL.B.h, 0xF2);
}
void LD_xHL() {
    byte ops = 1;
    cpu->HL.W = 0x1234;
    
    rom(0x100, 0x36);//LD (hl)
    rom(0x101, 0xF2);//0xF2
    rom(0x1234, 0x00);
    
    runOps(ops);
    
    eql(cpu->PC.W, 0x102);
    eql(read(0x1234), 0xF2);
}

void LD_C() {
    byte ops = 1;
    cpu->BC.W = 0x0000;
    
    rom(0x100, 0x0E);//LD c
    rom(0x101, 0xF2);//0xF2
    rom(0x102, 0x00);
    
    runOps(ops);
    
    eql(cpu->PC.W, 0x102);
    eql(cpu->BC.B.l, 0xF2);
}

void LD_E() {
    byte ops = 1;
    cpu->DE.W = 0x0000;
    
    rom(0x100, 0x1E);//LD e
    rom(0x101, 0xF2);//0xF2
    rom(0x102, 0x00);
    
    runOps(ops);
    
    eql(cpu->PC.W, 0x102);
    eql(cpu->DE.B.l, 0xF2);
}
void LD_L() {
    byte ops = 1;
    cpu->HL.W = 0x0000;
    
    rom(0x100, 0x2E);//LD l
    rom(0x101, 0xF2);//0xF2
    rom(0x102, 0x00);
    
    runOps(ops);
    
    eql(cpu->PC.W, 0x102);
    eql(cpu->HL.B.l, 0xF2);
}
void LD_A() {
    byte ops = 1;
    cpu->HL.W = 0x0000;
    
    rom(0x100, 0x3E);//LD a
    rom(0x101, 0xF2);//0xF2
    rom(0x102, 0x00);
    
    runOps(ops);
    
    eql(cpu->PC.W, 0x102);
    eql(cpu->AF.B.h, 0xF2);
}
void INC_B() {
    byte ops = 1;
    cpu->BC.B.h = 0x06;
    cpu->AF.B.l = 0x00;
    
    rom(0x100, 0x04);// INC B
    
    runOps(ops);
    
    eql(cpu->BC.B.h, 0x07);
    fnotset(FLAG_N);
    fnotset(FLAG_Z);
    fnotset(FLAG_H);
}
void INC_B_HSET() {
    byte ops = 1;
    cpu->BC.B.h = 0x0F;
    cpu->AF.B.l = 0x00;
    
    rom(0x100, 0x04);// INC B
    
    runOps(ops);
    
    eql(cpu->BC.B.h, 0x10);
    fnotset(FLAG_N);
    fnotset(FLAG_Z);
    fset(FLAG_H);
}

void INC_B_NOHSET() {
    byte ops = 1;
    cpu->BC.B.h = 0x10;
    cpu->AF.B.l = 0x00;
    
    rom(0x100, 0x04);// INC B
    
    runOps(ops);
    
    eql(cpu->BC.B.h, 0x11);
    fnotset(FLAG_N);
    fnotset(FLAG_Z);
    fnotset(FLAG_H);
}

void INC_B_ZSET() {
    byte ops = 1;
    cpu->BC.B.h = 0xFF;
    cpu->AF.B.l = 0x00;
    
    rom(0x100, 0x04);// INC B
    
    runOps(ops);
    
    eql(cpu->BC.B.h, 0x00);
    fnotset(FLAG_N);
    fset(FLAG_Z);
    fset(FLAG_H);
}
void INC_D() {
    byte ops = 1;
    cpu->DE.B.h = 0x06;
    cpu->AF.B.l = 0x00;
    
    rom(0x100, 0x14);// INC D
    
    runOps(ops);
    
    eql(cpu->DE.B.h, 0x07);
    fnotset(FLAG_N);
    fnotset(FLAG_Z);
    fnotset(FLAG_H);
}
void INC_D_HSET() {
    byte ops = 1;
    cpu->DE.B.h = 0x0F;
    cpu->AF.B.l = 0x00;
    
    rom(0x100, 0x14);// INC D
    
    runOps(ops);
    
    eql(cpu->DE.B.h, 0x10);
    fnotset(FLAG_N);
    fnotset(FLAG_Z);
    fset(FLAG_H);
}

void INC_D_NOHSET() {
    byte ops = 1;
    cpu->DE.B.h = 0x10;
    cpu->AF.B.l = 0x00;
    
    rom(0x100, 0x14);// INC D
    
    runOps(ops);
    
    eql(cpu->DE.B.h, 0x11);
    fnotset(FLAG_N);
    fnotset(FLAG_Z);
    fnotset(FLAG_H);
}

void INC_D_ZSET() {
    byte ops = 1;
    cpu->DE.B.h = 0xFF;
    cpu->AF.B.l = 0x00;
    
    rom(0x100, 0x14);// INC D
    
    runOps(ops);
    
    eql(cpu->DE.B.h, 0x00);
    fnotset(FLAG_N);
    fset(FLAG_Z);
    fset(FLAG_H);
}
void INC_H() {
    byte ops = 1;
    cpu->HL.B.h = 0x06;
    cpu->AF.B.l = 0x00;
    
    rom(0x100, 0x24);// INC H
    
    runOps(ops);
    
    eql(cpu->HL.B.h, 0x07);
    fnotset(FLAG_N);
    fnotset(FLAG_Z);
    fnotset(FLAG_H);
}
void INC_H_HSET() {
    byte ops = 1;
    cpu->HL.B.h = 0x0F;
    cpu->AF.B.l = 0x00;
    
    rom(0x100, 0x24);// INC H
    
    runOps(ops);
    
    eql(cpu->HL.B.h, 0x10);
    fnotset(FLAG_N);
    fnotset(FLAG_Z);
    fset(FLAG_H);
}

void INC_H_NOHSET() {
    byte ops = 1;
    cpu->HL.B.h = 0x10;
    cpu->AF.B.l = 0x00;
    
    rom(0x100, 0x24);// INC H
    
    runOps(ops);
    
    eql(cpu->HL.B.h, 0x11);
    fnotset(FLAG_N);
    fnotset(FLAG_Z);
    fnotset(FLAG_H);
}

void INC_H_ZSET() {
    byte ops = 1;
    cpu->HL.B.h = 0xFF;
    cpu->AF.B.l = 0x00;
    
    rom(0x100, 0x24);// INC H
    
    runOps(ops);
    
    eql(cpu->HL.B.h, 0x00);
    fnotset(FLAG_N);
    fset(FLAG_Z);
    fset(FLAG_H);
}
void INC_C() {
    byte ops = 1;
    cpu->BC.B.l = 0x06;
    cpu->AF.B.l = 0x00;
    
    rom(0x100, 0x0C);// INC C
    
    runOps(ops);
    
    eql(cpu->BC.B.l, 0x07);
    fnotset(FLAG_N);
    fnotset(FLAG_Z);
    fnotset(FLAG_H);
}
void INC_C_HSET() {
    byte ops = 1;
    cpu->BC.B.l = 0x0F;
    cpu->AF.B.l = 0x00;
    
    rom(0x100, 0x0C);// INC C
    
    runOps(ops);
    
    eql(cpu->BC.B.l, 0x10);
    fnotset(FLAG_N);
    fnotset(FLAG_Z);
    fset(FLAG_H);
}

void INC_C_NOHSET() {
    byte ops = 1;
    cpu->BC.B.l = 0x10;
    cpu->AF.B.l = 0x00;
    
    rom(0x100, 0x0C);// INC C
    
    runOps(ops);
    
    eql(cpu->BC.B.l, 0x11);
    fnotset(FLAG_N);
    fnotset(FLAG_Z);
    fnotset(FLAG_H);
}

void INC_C_ZSET() {
    byte ops = 1;
    cpu->BC.B.l = 0xFF;
    cpu->AF.B.l = 0x00;
    
    rom(0x100, 0x0C);// INC C
    
    runOps(ops);
    
    eql(cpu->BC.B.l, 0x00);
    fnotset(FLAG_N);
    fset(FLAG_Z);
    fset(FLAG_H);
}
void INC_E() {
    byte ops = 1;
    cpu->DE.B.l = 0x06;
    cpu->AF.B.l = 0x00;
    
    rom(0x100, 0x1C);// INC E
    
    runOps(ops);
    
    eql(cpu->DE.B.l, 0x07);
    fnotset(FLAG_N);
    fnotset(FLAG_Z);
    fnotset(FLAG_H);
}
void INC_E_HSET() {
    byte ops = 1;
    cpu->DE.B.l = 0x0F;
    cpu->AF.B.l = 0x00;
    
    rom(0x100, 0x1C);// INC E
    
    runOps(ops);
    
    eql(cpu->DE.B.l, 0x10);
    fnotset(FLAG_N);
    fnotset(FLAG_Z);
    fset(FLAG_H);
}

void INC_E_NOHSET() {
    byte ops = 1;
    cpu->DE.B.l = 0x10;
    cpu->AF.B.l = 0x00;
    
    rom(0x100, 0x1C);// INC E
    
    runOps(ops);
    
    eql(cpu->DE.B.l, 0x11);
    fnotset(FLAG_N);
    fnotset(FLAG_Z);
    fnotset(FLAG_H);
}

void INC_E_ZSET() {
    byte ops = 1;
    cpu->DE.B.l = 0xFF;
    cpu->AF.B.l = 0x00;
    
    rom(0x100, 0x1C);// INC E
    
    runOps(ops);
    
    eql(cpu->DE.B.l, 0x00);
    fnotset(FLAG_N);
    fset(FLAG_Z);
    fset(FLAG_H);
}
void INC_L() {
    byte ops = 1;
    cpu->HL.B.l = 0x06;
    cpu->AF.B.l = 0x00;
    
    rom(0x100, 0x2C);// INC L
    
    runOps(ops);
    
    eql(cpu->HL.B.l, 0x07);
    fnotset(FLAG_N);
    fnotset(FLAG_Z);
    fnotset(FLAG_H);
}
void INC_L_HSET() {
    byte ops = 1;
    cpu->HL.B.l = 0x0F;
    cpu->AF.B.l = 0x00;
    
    rom(0x100, 0x2C);// INC L
    
    runOps(ops);
    
    eql(cpu->HL.B.l, 0x10);
    fnotset(FLAG_N);
    fnotset(FLAG_Z);
    fset(FLAG_H);
}

void INC_L_NOHSET() {
    byte ops = 1;
    cpu->HL.B.l = 0x10;
    cpu->AF.B.l = 0x00;
    
    rom(0x100, 0x2C);// INC L
    
    runOps(ops);
    
    eql(cpu->HL.B.l, 0x11);
    fnotset(FLAG_N);
    fnotset(FLAG_Z);
    fnotset(FLAG_H);
}

void INC_L_ZSET() {
    byte ops = 1;
    cpu->HL.B.l = 0xFF;
    cpu->AF.B.l = 0x00;
    
    rom(0x100, 0x2C);// INC L
    
    runOps(ops);
    
    eql(cpu->HL.B.l, 0x00);
    fnotset(FLAG_N);
    fset(FLAG_Z);
    fset(FLAG_H);
}

void INC_A() {
    byte ops = 1;
    cpu->AF.B.h = 0x06;
    cpu->AF.B.l = 0x00;
    
    rom(0x100, 0x3C);// INC A
    
    runOps(ops);
    
    eql(cpu->AF.B.h, 0x07);
    fnotset(FLAG_N);
    fnotset(FLAG_Z);
    fnotset(FLAG_H);
}
void INC_A_HSET() {
    byte ops = 1;
    cpu->AF.B.h = 0x0F;
    cpu->AF.B.l = 0x00;
    
    rom(0x100, 0x3C);// INC A
    
    runOps(ops);
    
    eql(cpu->AF.B.h, 0x10);
    fnotset(FLAG_N);
    fnotset(FLAG_Z);
    fset(FLAG_H);
}

void INC_A_NOHSET() {
    byte ops = 1;
    cpu->AF.B.h = 0x10;
    cpu->AF.B.l = 0x00;
    
    rom(0x100, 0x3C);// INC A
    
    runOps(ops);
    
    eql(cpu->AF.B.h, 0x11);
    fnotset(FLAG_N);
    fnotset(FLAG_Z);
    fnotset(FLAG_H);
}

void INC_A_ZSET() {
    byte ops = 1;
    cpu->AF.B.h = 0xFF;
    cpu->AF.B.l = 0x00;
    
    rom(0x100, 0x3C);// INC A
    
    runOps(ops);
    
    eql(cpu->AF.B.h, 0x00);
    fnotset(FLAG_N);
    fset(FLAG_Z);
    fset(FLAG_H);
}

void RunAllCpuTests(void) {
    word tests = 0, pass = 0, fail = 0;
    cpu = setup();
    
    TEST(PUSH_BC);
    TEST(PUSH_DE);
    TEST(PUSH_HL);
    TEST(PUSH_AF);
    TEST(POP_BC);
    TEST(POP_DE);
    TEST(POP_HL);
    TEST(POP_AF);
    TEST(CALL);
    TEST(CALL_RET);
    TEST(CALL_Z);
    TEST(CALL_Z_NOJUMP);
    TEST(CALL_NZ);
    TEST(CALL_NZ_NOJUMP);
    TEST(CALL_C);
    TEST(CALL_C_NOJUMP);
    TEST(JP);
    TEST(JP_Z_NOJUMP);
    TEST(JP_Z_JUMP);
    TEST(JP_NZ_NOJUMP);
    TEST(JP_NZ_JUMP);
    TEST(JP_C_NOJUMP);
    TEST(JP_C_JUMP);
    TEST(JP_NC_NOJUMP);
    TEST(JP_NC_JUMP);
    TEST(JP_HL);
    TEST(JR);
    TEST(JR_NEG);
    TEST(JR_Z);
    TEST(JR_Z_NOJUMP);
    TEST(JR_NZ);
    TEST(JR_NZ_NOJUMP);
    TEST(JR_C);
    TEST(JR_C_NOJUMP);
    TEST(JR_NC);
    TEST(JR_NC_NOJUMP);
    TEST(RET);
    TEST(LD_BC);
    TEST(LD_DE);
    TEST(LD_HL);
    TEST(LD_SP);
    TEST(LD_B);
    TEST(LD_D);
    TEST(LD_H);
    TEST(LD_xHL);
    TEST(LD_C);
    TEST(LD_E);
    TEST(LD_L);
    TEST(LD_A);
    TEST(INC_B);
    TEST(INC_B_HSET);
    TEST(INC_B_NOHSET);
    TEST(INC_B_ZSET);
    TEST(INC_C);
    TEST(INC_C_HSET);
    TEST(INC_C_NOHSET);
    TEST(INC_C_ZSET);
    TEST(INC_D);
    TEST(INC_D_HSET);
    TEST(INC_D_NOHSET);
    TEST(INC_D_ZSET);
    TEST(INC_E);
    TEST(INC_E_HSET);
    TEST(INC_E_NOHSET);
    TEST(INC_E_ZSET);
    TEST(INC_H);
    TEST(INC_H_HSET);
    TEST(INC_H_NOHSET);
    TEST(INC_H_ZSET);
    TEST(INC_L);
    TEST(INC_L_HSET);
    TEST(INC_L_NOHSET);
    TEST(INC_L_ZSET);
    TEST(INC_A);
    TEST(INC_A_HSET);
    TEST(INC_A_NOHSET);
    TEST(INC_A_ZSET);
    cleanup(cpu);
    cout << dec;
    cout << "ran:  " << setw(3) << tests << setw(8) << "(" << setw(4) << assertPass+assertFail << " assertions)" << endl;
    cout << "pass: " << setw(3) << pass <<  setw(8) << "(" << setw(4) << assertPass << " assertions)" << endl;
    cout << "fail: " << setw(3) << fail <<  setw(8) << "(" << setw(4) << assertFail << " assertions)" << endl;
}

#ifdef BUILD_TEST

int main(void) {
    RunAllCpuTests();
    return 0;
}
#endif
