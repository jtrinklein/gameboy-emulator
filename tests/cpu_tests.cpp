#include <iostream>
#include <iomanip>
#include <functional>
#include "tests.h"
#include "gameboy.h"
#include "render.h"
#include "mmu.h"
#include "cart.h"

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

CPU* testsetup() {
    CPU* c = new CPU();
    c->mmu = new MMU();
    c->mmu->rom = new byte[0x8000];
    c->mmu->cart = new Cart("TEST");
    c->gpu = new GPU(c->mmu);
    return c;
}

void testcleanup(CPU* cpu) {
    delete cpu->gpu;
    delete cpu->mmu;
    delete cpu;
}

void beforeTest() {
    //clear rom
    memset(cpu->mmu->rom, 0, 0x8000);

    //reset cpu
    initCPU(cpu);

    //move SP to 0xDFFF
    cpu->SP.W = 0xDFFF;
}


byte read(word a) {
    return readMem(cpu, a);
}
void rom(word a, byte v) {
    //writeMem(cpu, a, v);
    cpu->mmu->rom[a] = v;
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

void INC_xHL() {
    byte ops = 1;

    cpu->HL.W = 0x1266;
    cpu->AF.B.l = 0x00;

    rom(0x100, 0x34);// INC (HL)
    rom(0x1266, 0x06);// (HL)

    runOps(ops);

    eql(read(cpu->HL.W), 0x07);
    fnotset(FLAG_N);
    fnotset(FLAG_Z);
    fnotset(FLAG_H);
}
void INC_xHL_HSET() {
    byte ops = 1;

    cpu->HL.W = 0x1266;
    cpu->AF.B.l = 0x00;

    rom(0x100, 0x34);// INC (HL)
    rom(0x1266, 0x0F);// (HL)

    runOps(ops);

    eql(read(cpu->HL.W), 0x10);
    fnotset(FLAG_N);
    fnotset(FLAG_Z);
    fset(FLAG_H);
}

void INC_xHL_NOHSET() {
    byte ops = 1;

    cpu->HL.W = 0x1266;
    cpu->AF.B.l = 0x00;

    rom(0x100, 0x34);// INC (HL)
    rom(0x1266, 0x10);// (HL)

    runOps(ops);

    eql(read(cpu->HL.W), 0x11);
    fnotset(FLAG_N);
    fnotset(FLAG_Z);
    fnotset(FLAG_H);
}

void INC_xHL_ZSET() {
    byte ops = 1;

    cpu->HL.W = 0x1266;
    cpu->AF.B.l = 0x00;

    rom(0x100, 0x34);// INC (HL)
    rom(0x1266, 0xFF);// (HL)

    runOps(ops);

    eql(read(cpu->HL.W), 0x00);
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
void DEC_B() {
    byte ops = 1;
    cpu->BC.B.h = 0x06;
    cpu->AF.B.l = 0x00;

    rom(0x100, 0x05);// DEC B

    runOps(ops);

    eql(cpu->BC.B.h, 0x05);
    fset(FLAG_N);
    fnotset(FLAG_Z);
    fnotset(FLAG_H);
}
void DEC_B_HSET() {
    byte ops = 1;
    cpu->BC.B.h = 0x10;
    cpu->AF.B.l = 0x00;

    rom(0x100, 0x05);// DEC B

    runOps(ops);

    eql(cpu->BC.B.h, 0x0F);
    fset(FLAG_N);
    fnotset(FLAG_Z);
    fset(FLAG_H);
}
void DEC_B_HSET2() {
    byte ops = 1;
    cpu->BC.B.h = 0x30;
    cpu->AF.B.l = 0x00;

    rom(0x100, 0x05);// DEC B

    runOps(ops);

    eql(cpu->BC.B.h, 0x2F);
    fset(FLAG_N);
    fnotset(FLAG_Z);
    fset(FLAG_H);
}

void DEC_B_ZSET() {
    byte ops = 1;
    cpu->BC.B.h = 0x01;
    cpu->AF.B.l = 0x00;

    rom(0x100, 0x05);// DEC B

    runOps(ops);

    eql(cpu->BC.B.h, 0x00);
    fset(FLAG_N);
    fset(FLAG_Z);
    fnotset(FLAG_H);
}
void DEC_D() {
    byte ops = 1;
    cpu->DE.B.h = 0x07;
    cpu->AF.B.l = 0x00;

    rom(0x100, 0x15);// DEC D

    runOps(ops);

    eql(cpu->DE.B.h, 0x06);
    fset(FLAG_N);
    fnotset(FLAG_Z);
    fnotset(FLAG_H);
}
void DEC_D_HSET() {
    byte ops = 1;
    cpu->DE.B.h = 0x10;
    cpu->AF.B.l = 0x00;

    rom(0x100, 0x15);// DEC D

    runOps(ops);

    eql(cpu->DE.B.h, 0x0F);
    fset(FLAG_N);
    fnotset(FLAG_Z);
    fset(FLAG_H);
}

void DEC_D_ZSET() {
    byte ops = 1;
    cpu->DE.B.h = 0x01;
    cpu->AF.B.l = 0x00;

    rom(0x100, 0x15);// DEC D

    runOps(ops);

    eql(cpu->DE.B.h, 0x00);
    fset(FLAG_N);
    fset(FLAG_Z);
    fnotset(FLAG_H);
}
void DEC_H() {
    byte ops = 1;
    cpu->HL.B.h = 0x06;
    cpu->AF.B.l = 0x00;

    rom(0x100, 0x25);// DEC H

    runOps(ops);

    eql(cpu->HL.B.h, 0x05);
    fset(FLAG_N);
    fnotset(FLAG_Z);
    fnotset(FLAG_H);
}
void DEC_H_HSET() {
    byte ops = 1;
    cpu->HL.B.h = 0x10;
    cpu->AF.B.l = 0x00;

    rom(0x100, 0x25);// DEC H

    runOps(ops);

    eql(cpu->HL.B.h, 0x0F);
    fset(FLAG_N);
    fnotset(FLAG_Z);
    fset(FLAG_H);
}

void DEC_H_ZSET() {
    byte ops = 1;
    cpu->HL.B.h = 0x01;
    cpu->AF.B.l = 0x00;

    rom(0x100, 0x25);// DEC H

    runOps(ops);

    eql(cpu->HL.B.h, 0x00);
    fset(FLAG_N);
    fset(FLAG_Z);
    fnotset(FLAG_H);
}
void DEC_C() {
    byte ops = 1;
    cpu->BC.B.l = 0x08;
    cpu->AF.B.l = 0x00;

    rom(0x100, 0x0D);// DEC C

    runOps(ops);

    eql(cpu->BC.B.l, 0x07);
    fset(FLAG_N);
    fnotset(FLAG_Z);
    fnotset(FLAG_H);
}
void DEC_C_HSET() {
    byte ops = 1;
    cpu->BC.B.l = 0x10;
    cpu->AF.B.l = 0x00;

    rom(0x100, 0x0D);// DEC C

    runOps(ops);

    eql(cpu->BC.B.l, 0x0F);
    fset(FLAG_N);
    fnotset(FLAG_Z);
    fset(FLAG_H);
}

void DEC_C_ZSET() {
    byte ops = 1;
    cpu->BC.B.l = 0x01;
    cpu->AF.B.l = 0x00;

    rom(0x100, 0x0D);// DEC C

    runOps(ops);

    eql(cpu->BC.B.l, 0x00);
    fset(FLAG_N);
    fset(FLAG_Z);
    fnotset(FLAG_H);
}
void DEC_E() {
    byte ops = 1;
    cpu->DE.B.l = 0x08;
    cpu->AF.B.l = 0x00;

    rom(0x100, 0x1D);// DEC E

    runOps(ops);

    eql(cpu->DE.B.l, 0x07);
    fset(FLAG_N);
    fnotset(FLAG_Z);
    fnotset(FLAG_H);
}
void DEC_E_HSET() {
    byte ops = 1;
    cpu->DE.B.l = 0x10;
    cpu->AF.B.l = 0x00;

    rom(0x100, 0x1D);// DEC E

    runOps(ops);

    eql(cpu->DE.B.l, 0x0F);
    fset(FLAG_N);
    fnotset(FLAG_Z);
    fset(FLAG_H);
}

void DEC_E_ZSET() {
    byte ops = 1;
    cpu->DE.B.l = 0x01;
    cpu->AF.B.l = 0x00;

    rom(0x100, 0x1D);// DEC E

    runOps(ops);

    eql(cpu->DE.B.l, 0x00);
    fset(FLAG_N);
    fset(FLAG_Z);
    fnotset(FLAG_H);
}
void DEC_L() {
    byte ops = 1;
    cpu->HL.B.l = 0x08;
    cpu->AF.B.l = 0x00;

    rom(0x100, 0x2D);// DEC L

    runOps(ops);

    eql(cpu->HL.B.l, 0x07);
    fset(FLAG_N);
    fnotset(FLAG_Z);
    fnotset(FLAG_H);
}
void DEC_L_HSET() {
    byte ops = 1;
    cpu->HL.B.l = 0x10;
    cpu->AF.B.l = 0x00;

    rom(0x100, 0x2D);// DEC L

    runOps(ops);

    eql(cpu->HL.B.l, 0x0F);
    fset(FLAG_N);
    fnotset(FLAG_Z);
    fset(FLAG_H);
}

void DEC_L_ZSET() {
    byte ops = 1;
    cpu->HL.B.l = 0x01;
    cpu->AF.B.l = 0x00;

    rom(0x100, 0x2D);// DEC L

    runOps(ops);

    eql(cpu->HL.B.l, 0x00);
    fset(FLAG_N);
    fset(FLAG_Z);
    fnotset(FLAG_H);
}

void DEC_xHL() {
    byte ops = 1;

    cpu->HL.W = 0x1266;
    cpu->AF.B.l = 0x00;

    rom(0x100, 0x35);// DEC (HL)
    rom(0x1266, 0x08);// (HL)

    runOps(ops);

    eql(read(cpu->HL.W), 0x07);
    fset(FLAG_N);
    fnotset(FLAG_Z);
    fnotset(FLAG_H);
}
void DEC_xHL_HSET() {
    byte ops = 1;

    cpu->HL.W = 0x1266;
    cpu->AF.B.l = 0x00;

    rom(0x100, 0x35);// DEC (HL)
    rom(0x1266, 0x10);// (HL)

    runOps(ops);

    eql(read(cpu->HL.W), 0x0F);
    fset(FLAG_N);
    fnotset(FLAG_Z);
    fset(FLAG_H);
}

void DEC_xHL_ZSET() {
    byte ops = 1;

    cpu->HL.W = 0x1266;
    cpu->AF.B.l = 0x00;

    rom(0x100, 0x35);// DEC (HL)
    rom(0x1266, 0x01);// (HL)

    runOps(ops);

    eql(read(cpu->HL.W), 0x00);
    fset(FLAG_N);
    fset(FLAG_Z);
    fnotset(FLAG_H);
}

void DEC_A() {
    byte ops = 1;
    cpu->AF.B.h = 0x08;
    cpu->AF.B.l = 0x00;

    rom(0x100, 0x3D);// DEC A

    runOps(ops);

    eql(cpu->AF.B.h, 0x07);
    fset(FLAG_N);
    fnotset(FLAG_Z);
    fnotset(FLAG_H);
}
void DEC_A_HSET() {
    byte ops = 1;
    cpu->AF.B.h = 0x10;
    cpu->AF.B.l = 0x00;

    rom(0x100, 0x3D);// DEC A

    runOps(ops);

    eql(cpu->AF.B.h, 0x0F);
    fset(FLAG_N);
    fnotset(FLAG_Z);
    fset(FLAG_H);
}

void DEC_A_ZSET() {
    byte ops = 1;
    cpu->AF.B.h = 0x01;
    cpu->AF.B.l = 0x00;

    rom(0x100, 0x3D);// DEC A

    runOps(ops);

    eql(cpu->AF.B.h, 0x00);
    fset(FLAG_N);
    fset(FLAG_Z);
    fnotset(FLAG_H);
}

void INC_BC() {
    byte ops = 1;
    cpu->BC.W = 0xAB32;

    rom(0x100, 0x03);

    runOps(ops);

    eql(cpu->BC.W, 0xAB33);
}

void INC_DE() {
    byte ops = 1;
    cpu->DE.W = 0xAB32;

    rom(0x100, 0x13);

    runOps(ops);

    eql(cpu->DE.W, 0xAB33);
}

void INC_HL() {
    byte ops = 1;
    cpu->HL.W = 0xAB32;

    rom(0x100, 0x23);

    runOps(ops);

    eql(cpu->HL.W, 0xAB33);
}

void INC_SP() {
    byte ops = 1;
    cpu->SP.W = 0xDFFF;

    rom(0x100, 0x33);

    runOps(ops);

    eql(cpu->SP.W, 0xE000);
}

void DEC_BC() {
    byte ops = 1;
    cpu->BC.W = 0xA001;

    rom(0x100, 0x0B);

    runOps(ops);

    eql(cpu->BC.W, 0xA000);
}

void DEC_DE() {
    byte ops = 1;
    cpu->DE.W = 0xB000;

    rom(0x100, 0x1B);

    runOps(ops);

    eql(cpu->DE.W, 0xAFFF);
}

void DEC_HL() {
    byte ops = 1;
    cpu->HL.W = 0xAB32;

    rom(0x100, 0x2B);

    runOps(ops);

    eql(cpu->HL.W, 0xAB31);
}

void DEC_SP() {
    byte ops = 1;
    cpu->SP.W = 0xDFFF;

    rom(0x100, 0x3B);

    runOps(ops);

    eql(cpu->SP.W, 0xDFFE);
}

void LD_xBC_A() {
    byte ops = 1;
    cpu->AF.B.h = 0xA1;
    cpu->BC.W = 0x4BC1;

    rom(0x100, 0x02);//LD (BC), A
    rom(0x4BC1, 0x00);

    runOps(ops);

    eql(read(0x4BC1), 0xA1);
}
void LD_xDE_A() {
    byte ops = 1;
    cpu->AF.B.h = 0xA1;
    cpu->DE.W = 0x4BC1;

    rom(0x100, 0x12);//LD (DE), A
    rom(0x4BC1, 0x00);

    runOps(ops);

    eql(read(0x4BC1), 0xA1);
}
void LDI_xHL_A() {
    byte ops = 1;
    cpu->AF.B.h = 0xA1;
    cpu->HL.W = 0x4BC1;

    rom(0x100, 0x22);//LDI HL, A
    rom(0x4BC1, 0x00);

    runOps(ops);

    eql(cpu->HL.W, 0x4BC2);
    eql(read(0x4BC1), 0xA1);
}
void LDD_xHL_A() {
    byte ops = 1;
    cpu->AF.B.h = 0xA1;
    cpu->HL.W = 0x4BC1;

    rom(0x100, 0x32);//LDI HL, A
    rom(0x4BC1, 0x00);

    runOps(ops);

    eql(cpu->HL.W, 0x4BC0);
    eql(read(0x4BC1), 0xA1);
}
void LD_xHL_B() { cpu->BC.B.h = 0xA1; cpu->HL.W = 0x4BC1; rom(0x100, 0x70); rom(0x4BC1, 0x00); runOps(1); eql(read(0x4BC1), 0xA1); }
void LD_xHL_C() { cpu->BC.B.l = 0xA1; cpu->HL.W = 0x4BC1; rom(0x100, 0x71); rom(0x4BC1, 0x00); runOps(1); eql(read(0x4BC1), 0xA1); }
void LD_xHL_D() { cpu->DE.B.h = 0xA1; cpu->HL.W = 0x4BC1; rom(0x100, 0x72); rom(0x4BC1, 0x00); runOps(1); eql(read(0x4BC1), 0xA1); }
void LD_xHL_E() { cpu->DE.B.l = 0xA1; cpu->HL.W = 0x4BC1; rom(0x100, 0x73); rom(0x4BC1, 0x00); runOps(1); eql(read(0x4BC1), 0xA1); }
void LD_xHL_H() { cpu->HL.W = 0x4BC1; rom(0x100, 0x74); rom(0x4BC1, 0x00); runOps(1); eql(read(0x4BC1), cpu->HL.B.h); }
void LD_xHL_L() { cpu->HL.W = 0x4BC1; rom(0x100, 0x75); rom(0x4BC1, 0x00); runOps(1); eql(read(0x4BC1), cpu->HL.B.l); }
void LD_xHL_A() { cpu->AF.B.h = 0xA1; cpu->HL.W = 0x4BC1; rom(0x100, 0x77); rom(0x4BC1, 0x00); runOps(1); eql(read(0x4BC1), 0xA1); }

void LD_A_xBC() {
    byte ops = 1;
    cpu->AF.B.h = 0xA1;
    cpu->BC.W = 0x4BC1;

    rom(0x100, 0x0A);//LD A, (BC)
    rom(0x4BC1, 0x3D);

    runOps(ops);

    eql(cpu->AF.B.h, 0x3D);
}
void LD_A_xDE() {
    byte ops = 1;
    cpu->AF.B.h = 0xA1;
    cpu->DE.W = 0x4BC1;

    rom(0x100, 0x1A);//LD A, (DE)
    rom(0x4BC1, 0x3D);

    runOps(ops);

    eql(cpu->AF.B.h, 0x3D);
}
void LDI_A_xHL() {
    byte ops = 1;
    cpu->AF.B.h = 0xA1;
    cpu->HL.W = 0x4BC1;

    rom(0x100, 0x2A);//LDI A, (HL)
    rom(0x4BC1, 0x3D);

    runOps(ops);

    eql(cpu->HL.W, 0x4BC2);
    eql(cpu->AF.B.h, 0x3D);
}
void LDD_A_xHL() {
    byte ops = 1;
    cpu->AF.B.h = 0xA1;
    cpu->HL.W = 0x4BC1;

    rom(0x100, 0x3A);//LDD A, (HL)
    rom(0x4BC1, 0x3D);

    runOps(ops);

    eql(cpu->HL.W, 0x4BC0);
    eql(cpu->AF.B.h, 0x3D);
}

void LD_xNN_SP() {
    byte ops = 1;
    cpu->SP.W = 0x1234;

    rom(0x100, 0x08);//LD (nn) SP
    rom(0x101, 0x01);
    rom(0x102, 0x20);// 0xAECF
    rom(0x2001, 0xFF);
    rom(0x2002, 0xFF);
    runOps(ops);

    eql(read(0x2001), 0x12);
    eql(read(0x2002), 0x34);
}

void RLA() {
    byte ops = 1;
    cpu->AF.B.l = 0x00;
    cpu->AF.B.h = 0x78; //01111000 b
    
    rom(0x100, 0x17);//RLA
    
    runOps(ops);
    
    eql(cpu->AF.B.h, 0xF0);//11110000 b
    fnotset(FLAG_H);
    fnotset(FLAG_N);
    fnotset(FLAG_Z);
    fnotset(FLAG_C);
}

void RLA_C_SET() {
    byte ops = 1;
    cpu->AF.B.l = 0x00;
    cpu->AF.B.h = 0xF0; //11110000 b

    rom(0x100, 0x17);//RLA

    runOps(ops);

    eql(cpu->AF.B.h, 0xE0);//11100000 b
    fnotset(FLAG_H);
    fnotset(FLAG_N);
    fnotset(FLAG_Z);
    fset(FLAG_C);
}

void RLA_C_PRESET() {
    byte ops = 1;
    cpu->AF.B.l = FLAG_C;
    cpu->AF.B.h = 0x41; //01000001 b

    rom(0x100, 0x17);//RLA

    runOps(ops);

    eql(cpu->AF.B.h, 0x83);//10000011 b
    fnotset(FLAG_H);
    fnotset(FLAG_N);
    fnotset(FLAG_Z);
    fnotset(FLAG_C);
}


void RLCA() {
    byte ops = 1;
    cpu->AF.B.l = 0x00;
    cpu->AF.B.h = 0x78;//00011110 b

    rom(0x100, 0x07);//RRCA

    runOps(ops);

    eql(cpu->AF.B.h, 0xF0); //00001111 b
    fnotset(FLAG_H);
    fnotset(FLAG_N);
    fnotset(FLAG_Z);
    fnotset(FLAG_C);
}

void RLCA_C_PRESET() {
    byte ops = 1;
    cpu->AF.B.l = FLAG_C;
    cpu->AF.B.h = 0x78;//01111000 b

    rom(0x100, 0x07);//RLCA

    runOps(ops);

    eql(cpu->AF.B.h, 0xF0); //11110000 b
    fnotset(FLAG_H);
    fnotset(FLAG_N);
    fnotset(FLAG_Z);
    fnotset(FLAG_C);
}

void RLCA_C_SET() {
    byte ops = 1;
    cpu->AF.B.l = 0x00;
    cpu->AF.B.h = 0xBC;//01111001 b

    rom(0x100, 0x07);//RRCA

    runOps(ops);

    eql(cpu->AF.B.h, 0x79); //10111100 b
    fnotset(FLAG_H);
    fnotset(FLAG_N);
    fnotset(FLAG_Z);
    fset(FLAG_C);
}

void RRCA() {
    byte ops = 1;
    cpu->AF.B.l = 0x00;
    cpu->AF.B.h = 0x1E;//00011110 b

    rom(0x100, 0x0F);//RRCA

    runOps(ops);

    eql(cpu->AF.B.h, 0x0F); //00001111 b
    fnotset(FLAG_H);
    fnotset(FLAG_N);
    fnotset(FLAG_Z);
    fnotset(FLAG_C);
}

void RRCA_C_PRESET() {
    byte ops = 1;
    cpu->AF.B.l = FLAG_C;
    cpu->AF.B.h = 0x1E;//00011110 b

    rom(0x100, 0x0F);//RRCA

    runOps(ops);

    eql(cpu->AF.B.h, 0x0F); //00001111 b
    fnotset(FLAG_H);
    fnotset(FLAG_N);
    fnotset(FLAG_Z);
    fnotset(FLAG_C);
}

void RRCA_C_SET() {
    byte ops = 1;
    cpu->AF.B.l = 0x00;
    cpu->AF.B.h = 0x31;//00110001 b

    rom(0x100, 0x0F);//RRCA

    runOps(ops);

    eql(cpu->AF.B.h, 0x98); //10011000 b
    fnotset(FLAG_H);
    fnotset(FLAG_N);
    fnotset(FLAG_Z);
    fset(FLAG_C);
}

void RRA() {
    byte ops = 1;
    cpu->AF.B.l = 0x00;
    cpu->AF.B.h = 0x12;//00010010 b

    rom(0x100, 0x1F);//RRA

    runOps(ops);

    eql(cpu->AF.B.h, 0x09); //00001001 b
    fnotset(FLAG_H);
    fnotset(FLAG_N);
    fnotset(FLAG_Z);
    fnotset(FLAG_C);
}

void RRA_C_PRESET() {
    byte ops = 1;
    cpu->AF.B.l = FLAG_C;
    cpu->AF.B.h = 0x02;//00000010 b

    rom(0x100, 0x1F);//RRA

    runOps(ops);

    eql(cpu->AF.B.h, 0x81); //10000001 b
    fnotset(FLAG_H);
    fnotset(FLAG_N);
    fnotset(FLAG_Z);
    fnotset(FLAG_C);
}

void RRA_C_SET() {
    byte ops = 1;
    cpu->AF.B.l = 0x00;
    cpu->AF.B.h = 0x31;//00110001 b

    rom(0x100, 0x1F);//RRA

    runOps(ops);

    eql(cpu->AF.B.h, 0x18); //10011000 b
    fnotset(FLAG_H);
    fnotset(FLAG_N);
    fnotset(FLAG_Z);
    fset(FLAG_C);
}

void DAA_L_LT_9() {
    byte ops = 1;
    cpu->AF.B.l = 0x00;
    cpu->AF.B.h = 0x01;
    
    rom(0x100, 0x27);//DAA
    
    runOps(ops);
    
    eql(cpu->AF.B.h, 0x01);
    fnotset(FLAG_H);
    fnotset(FLAG_N);
    fnotset(FLAG_Z);
    fnotset(FLAG_C);
}

void DAA_L_GT_9() {
    byte ops = 1;
    cpu->AF.B.l = 0x00;
    cpu->AF.B.h = 0x0C;
    
    rom(0x100, 0x27);//DAA
    
    runOps(ops);
    
    eql(cpu->AF.B.h, 0x12);
    fnotset(FLAG_H);
    fnotset(FLAG_N);
    fnotset(FLAG_Z);
    fnotset(FLAG_C);
}

void DAA_H_PRESET() {
    byte ops = 1;
    cpu->AF.B.l = FLAG_H;
    cpu->AF.B.h = 0x01;
    
    rom(0x100, 0x27);//DAA
    
    runOps(ops);
    
    eql(cpu->AF.B.h, 0x07);
    fnotset(FLAG_H);
    fnotset(FLAG_N);
    fnotset(FLAG_Z);
    fnotset(FLAG_C);
}

void DAA_L_GT_9_AND_H_PRESET() {
    byte ops = 1;
    cpu->AF.B.l = FLAG_H;
    cpu->AF.B.h = 0x0C;
    
    rom(0x100, 0x27);//DAA
    
    runOps(ops);
    
    eql(cpu->AF.B.h, 0x12);
    fnotset(FLAG_H);
    fnotset(FLAG_N);
    fnotset(FLAG_Z);
    fnotset(FLAG_C);
}

void DAA_U_LT_9() {
    byte ops = 1;
    cpu->AF.B.l = 0x00;
    cpu->AF.B.h = 0x10;
    
    rom(0x100, 0x27);//DAA
    
    runOps(ops);
    
    eql(cpu->AF.B.h, 0x10);
    fnotset(FLAG_H);
    fnotset(FLAG_N);
    fnotset(FLAG_Z);
    fnotset(FLAG_C);
}

void DAA_U_GT_9() {
    byte ops = 1;
    cpu->AF.B.l = 0x00;
    cpu->AF.B.h = 0xC0;
    
    rom(0x100, 0x27);//DAA
    
    runOps(ops);
    
    eql(cpu->AF.B.h, 0x20);
    fnotset(FLAG_H);
    fnotset(FLAG_N);
    fnotset(FLAG_Z);
    fset(FLAG_C);
}

void DAA_C_PRESET() {
    byte ops = 1;
    cpu->AF.B.l = FLAG_C;
    cpu->AF.B.h = 0x10;
    
    rom(0x100, 0x27);//DAA
    
    runOps(ops);
    
    eql(cpu->AF.B.h, 0x70);
    fnotset(FLAG_H);
    fnotset(FLAG_N);
    fnotset(FLAG_Z);
    fnotset(FLAG_C);
}

void DAA_U_GT_9_AND_C_PRESET() {
    byte ops = 1;
    cpu->AF.B.l = FLAG_C;
    cpu->AF.B.h = 0xC0;
    
    rom(0x100, 0x27);//DAA
    
    runOps(ops);
    
    eql(cpu->AF.B.h, 0x20);
    fnotset(FLAG_H);
    fnotset(FLAG_N);
    fnotset(FLAG_Z);
    fset(FLAG_C);
}

void DAA_Z_SET() {
    byte ops = 1;
    cpu->AF.B.l = FLAG_C;
    cpu->AF.B.h = 0xA0;
    
    rom(0x100, 0x27);//DAA
    
    runOps(ops);
    
    eql(cpu->AF.B.h, 0x00);
    fnotset(FLAG_H);
    fnotset(FLAG_N);
    fset(FLAG_Z);
    fset(FLAG_C);
}

void SCF() {
    byte ops = 1;
    cpu->AF.B.l = 0x00;
    
    rom(0x100, 0x37);//SCF
    
    runOps(ops);
    
    fnotset(FLAG_H);
    fnotset(FLAG_N);
    fnotset(FLAG_Z);
    fset(FLAG_C);
}

void SCF_Z_PRESET() {
    byte ops = 1;
    cpu->AF.B.l = FLAG_Z;
    
    rom(0x100, 0x37);//SCF
    
    runOps(ops);
    
    fnotset(FLAG_H);
    fnotset(FLAG_N);
    fset(FLAG_Z);
    fset(FLAG_C);
}

void SCF_NHZ_PRESET() {
    byte ops = 1;
    cpu->AF.B.l = ~FLAG_C;
    
    rom(0x100, 0x37);//SCF
    
    runOps(ops);
    
    fnotset(FLAG_H);
    fnotset(FLAG_N);
    fset(FLAG_Z);
    fset(FLAG_C);
}

void LD_B_B() {
    byte ops = 1;
    cpu->BC.B.h = 0x12;
    
    rom(0x100, 0x40);
    
    runOps(ops);
    
    eql(cpu->BC.B.h, 0x12);
}

void LD_B_C() {
    byte ops = 1;
    cpu->BC.B.l = 0x12;
    
    rom(0x100, 0x41);
    
    runOps(ops);
    
    eql(cpu->BC.B.h, cpu->BC.B.l);
}

void LD_B_D() {
    byte ops = 1;
    cpu->DE.B.h = 0x12;
    
    rom(0x100, 0x42);
    
    runOps(ops);
    
    eql(cpu->BC.B.h, cpu->DE.B.h);
}

void LD_B_E() {
    byte ops = 1;
    cpu->DE.B.l = 0x12;
    
    rom(0x100, 0x43);
    
    runOps(ops);
    
    eql(cpu->BC.B.h, cpu->DE.B.l);
}

void LD_B_H() {
    byte ops = 1;
    cpu->HL.B.h = 0x12;
    
    rom(0x100, 0x44);
    
    runOps(ops);
    
    eql(cpu->BC.B.h, cpu->HL.B.h);
}

void LD_B_L() {
    byte ops = 1;
    cpu->HL.B.l = 0x12;
    
    rom(0x100, 0x45);
    
    runOps(ops);
    
    eql(cpu->BC.B.h, cpu->HL.B.l);
}

void LD_B_A() {
    byte ops = 1;
    cpu->AF.B.h = 0x12;
    
    rom(0x100, 0x47);
    
    runOps(ops);
    
    eql(cpu->BC.B.h, cpu->AF.B.h);
}

void LD_B_xHL() {
    byte ops = 1;
    cpu->HL.W = 0x2134;
    
    rom(0x100, 0x46);
    rom(0x2134, 0x3C);
    
    runOps(ops);
    
    eql(cpu->BC.B.h, 0x3C);
}

void LD_D_B() {
    byte ops = 1;
    cpu->BC.B.h = 0x12;
    
    rom(0x100, 0x50);
    
    runOps(ops);
    
    eql(cpu->DE.B.h, cpu->BC.B.h);
}

void LD_D_C() {
    byte ops = 1;
    cpu->BC.B.l = 0x12;
    
    rom(0x100, 0x51);
    
    runOps(ops);
    
    eql(cpu->DE.B.h, cpu->BC.B.l);
}

void LD_D_E() {
    byte ops = 1;
    cpu->DE.B.l = 0x12;
    
    rom(0x100, 0x53);
    
    runOps(ops);
    
    eql(cpu->DE.B.h, cpu->DE.B.l);
}

void LD_D_H() {
    byte ops = 1;
    cpu->HL.B.h = 0x12;
    
    rom(0x100, 0x54);
    
    runOps(ops);
    
    eql(cpu->DE.B.h, cpu->HL.B.h);
}

void LD_D_L() {
    byte ops = 1;
    cpu->HL.B.l = 0x12;
    
    rom(0x100, 0x55);
    
    runOps(ops);
    
    eql(cpu->DE.B.h, cpu->HL.B.l);
}

void LD_D_A() {
    byte ops = 1;
    cpu->AF.B.h = 0x12;
    
    rom(0x100, 0x57);
    
    runOps(ops);
    
    eql(cpu->DE.B.h, cpu->AF.B.h);
}

void LD_D_xHL() {
    byte ops = 1;
    cpu->HL.W = 0x2134;
    
    rom(0x100, 0x56);
    rom(0x2134, 0x3C);
    
    runOps(ops);
    
    eql(cpu->DE.B.h, 0x3C);
}
void LD_D_D() {
    byte ops = 1;
    cpu->DE.B.h = 0x12;
    
    rom(0x100, 0x52);
    
    runOps(ops);
    
    eql(cpu->DE.B.h, 0x12);
}
void LD_H_B() {
    byte ops = 1;
    cpu->BC.B.h = 0x12;
    
    rom(0x100, 0x60);
    
    runOps(ops);
    
    eql(cpu->HL.B.h, cpu->BC.B.h);
}

void LD_H_C() {
    byte ops = 1;
    cpu->BC.B.l = 0x12;
    
    rom(0x100, 0x61);
    
    runOps(ops);
    
    eql(cpu->HL.B.h, cpu->BC.B.l);
}

void LD_H_D() {
    byte ops = 1;
    cpu->DE.B.h = 0x12;
    
    rom(0x100, 0x62);
    
    runOps(ops);
    
    eql(cpu->HL.B.h, cpu->DE.B.h);
}

void LD_H_E() {
    byte ops = 1;
    cpu->DE.B.l = 0x12;
    
    rom(0x100, 0x63);
    
    runOps(ops);
    
    eql(cpu->HL.B.h, cpu->DE.B.l);
}

void LD_H_L() {
    byte ops = 1;
    cpu->HL.B.l = 0x12;
    
    rom(0x100, 0x65);
    
    runOps(ops);
    
    eql(cpu->HL.B.h, cpu->HL.B.l);
}

void LD_H_A() {
    byte ops = 1;
    cpu->AF.B.h = 0x12;
    
    rom(0x100, 0x67);
    
    runOps(ops);
    
    eql(cpu->HL.B.h, cpu->AF.B.h);
}

void LD_H_xHL() {
    byte ops = 1;
    cpu->HL.W = 0x2134;
    
    rom(0x100, 0x66);
    rom(0x2134, 0x3C);
    
    runOps(ops);
    
    eql(cpu->HL.B.h, 0x3C);
}
void LD_H_H() {
    byte ops = 1;
    cpu->HL.B.h = 0x12;
    
    rom(0x100, 0x64);
    
    runOps(ops);
    
    eql(cpu->HL.B.h, 0x12);
}


void LD_A_B() {
    byte ops = 1;
    cpu->BC.B.h = 0x12;
    
    rom(0x100, 0x78);
    
    runOps(ops);
    
    eql(cpu->AF.B.h, cpu->BC.B.h);
}

void LD_A_C() {
    byte ops = 1;
    cpu->BC.B.l = 0x12;
    
    rom(0x100, 0x79);
    
    runOps(ops);
    
    eql(cpu->AF.B.h, cpu->BC.B.l);
}

void LD_A_D() {
    byte ops = 1;
    cpu->DE.B.h = 0x12;
    
    rom(0x100, 0x7A);
    
    runOps(ops);
    
    eql(cpu->AF.B.h, cpu->DE.B.h);
}

void LD_A_E() {
    byte ops = 1;
    cpu->DE.B.l = 0x12;
    
    rom(0x100, 0x7B);
    
    runOps(ops);
    
    eql(cpu->AF.B.h, cpu->DE.B.l);
}

void LD_A_H() {
    byte ops = 1;
    cpu->HL.B.h = 0x12;
    
    rom(0x100, 0x7C);
    
    runOps(ops);
    
    eql(cpu->AF.B.h, cpu->HL.B.h);
}

void LD_A_L() {
    byte ops = 1;
    cpu->HL.B.l = 0x12;
    
    rom(0x100, 0x7D);
    
    runOps(ops);
    
    eql(cpu->AF.B.h, cpu->HL.B.l);
}

void LD_A_xHL() {
    byte ops = 1;
    cpu->HL.W = 0x2134;
    
    rom(0x100, 0x7E);
    rom(0x2134, 0x3C);
    
    runOps(ops);
    
    eql(cpu->AF.B.h, 0x3C);
}
void LD_A_A() {
    byte ops = 1;
    cpu->AF.B.h = 0x12;
    
    rom(0x100, 0x7F);
    
    runOps(ops);
    
    eql(cpu->AF.B.h, 0x12);
}

//LD_C_x
void LD_C_B() {   cpu->BC.B.h = 0x12; rom(0x100, 0x48);                    runOps(1); eql(cpu->BC.B.l, cpu->BC.B.h); }
void LD_C_C() {   cpu->BC.B.l = 0x12; rom(0x100, 0x49);                    runOps(1); eql(cpu->BC.B.l, cpu->BC.B.l); }
void LD_C_D() {   cpu->DE.B.h = 0x12; rom(0x100, 0x4A);                    runOps(1); eql(cpu->BC.B.l, cpu->DE.B.h); }
void LD_C_E() {   cpu->DE.B.l = 0x12; rom(0x100, 0x4B);                    runOps(1); eql(cpu->BC.B.l, cpu->DE.B.l); }
void LD_C_H() {   cpu->HL.B.h = 0x12; rom(0x100, 0x4C);                    runOps(1); eql(cpu->BC.B.l, cpu->HL.B.h); }
void LD_C_L() {   cpu->HL.B.l = 0x12; rom(0x100, 0x4D);                    runOps(1); eql(cpu->BC.B.l, cpu->HL.B.l); }
void LD_C_xHL() { cpu->HL.W = 0x2134; rom(0x100, 0x4E); rom(0x2134, 0x3C); runOps(1); eql(cpu->BC.B.l, 0x3C); }
void LD_C_A() {   cpu->AF.B.h = 0x12; rom(0x100, 0x4F);                    runOps(1); eql(cpu->BC.B.l, cpu->AF.B.h); }



//LD_E_x
void LD_E_B() {   cpu->BC.B.h = 0x12; rom(0x100, 0x58);                    runOps(1); eql(cpu->DE.B.l, cpu->BC.B.h); }
void LD_E_C() {   cpu->BC.B.l = 0x12; rom(0x100, 0x59);                    runOps(1); eql(cpu->DE.B.l, cpu->BC.B.l); }
void LD_E_D() {   cpu->DE.B.h = 0x12; rom(0x100, 0x5A);                    runOps(1); eql(cpu->DE.B.l, cpu->DE.B.h); }
void LD_E_E() {   cpu->DE.B.l = 0x12; rom(0x100, 0x5B);                    runOps(1); eql(cpu->DE.B.l, cpu->DE.B.l); }
void LD_E_H() {   cpu->HL.B.h = 0x12; rom(0x100, 0x5C);                    runOps(1); eql(cpu->DE.B.l, cpu->HL.B.h); }
void LD_E_L() {   cpu->HL.B.l = 0x12; rom(0x100, 0x5D);                    runOps(1); eql(cpu->DE.B.l, cpu->HL.B.l); }
void LD_E_xHL() { cpu->HL.W = 0x2134; rom(0x100, 0x5E); rom(0x2134, 0x3C); runOps(1); eql(cpu->DE.B.l, 0x3C); }
void LD_E_A() {   cpu->AF.B.h = 0x12; rom(0x100, 0x5F);                    runOps(1); eql(cpu->DE.B.l, cpu->AF.B.h); }


//LD_L_x
void LD_L_B() {   cpu->BC.B.h = 0x12; rom(0x100, 0x68);                    runOps(1); eql(cpu->HL.B.l, cpu->BC.B.h); }
void LD_L_C() {   cpu->BC.B.l = 0x12; rom(0x100, 0x69);                    runOps(1); eql(cpu->HL.B.l, cpu->BC.B.l); }
void LD_L_D() {   cpu->DE.B.h = 0x12; rom(0x100, 0x6A);                    runOps(1); eql(cpu->HL.B.l, cpu->DE.B.h); }
void LD_L_E() {   cpu->DE.B.l = 0x12; rom(0x100, 0x6B);                    runOps(1); eql(cpu->HL.B.l, cpu->DE.B.l); }
void LD_L_H() {   cpu->HL.B.h = 0x12; rom(0x100, 0x6C);                    runOps(1); eql(cpu->HL.B.l, cpu->HL.B.h); }
void LD_L_L() {   cpu->HL.B.l = 0x12; rom(0x100, 0x6D);                    runOps(1); eql(cpu->HL.B.l, cpu->HL.B.l); }
void LD_L_xHL() { cpu->HL.W = 0x2134; rom(0x100, 0x6E); rom(0x2134, 0x3C); runOps(1); eql(cpu->HL.B.l, 0x3C); }
void LD_L_A() {   cpu->AF.B.h = 0x12; rom(0x100, 0x6F);                    runOps(1); eql(cpu->HL.B.l, cpu->AF.B.h); }

void RunAllCpuTests(void) {
    word tests = 0, pass = 0, fail = 0;
    cpu = testsetup();

    TEST(PUSH_BC); TEST(PUSH_DE); TEST(PUSH_HL); TEST(PUSH_AF);
    TEST(POP_BC); TEST(POP_DE); TEST(POP_HL); TEST(POP_AF);
    TEST(CALL); TEST(CALL_RET);
    TEST(CALL_Z); TEST(CALL_Z_NOJUMP);
    TEST(CALL_NZ); TEST(CALL_NZ_NOJUMP);
    TEST(CALL_C); TEST(CALL_C_NOJUMP);
    TEST(JP);
    TEST(JP_Z_NOJUMP); TEST(JP_Z_JUMP);
    TEST(JP_NZ_NOJUMP); TEST(JP_NZ_JUMP);
    TEST(JP_C_NOJUMP); TEST(JP_C_JUMP);
    TEST(JP_NC_NOJUMP); TEST(JP_NC_JUMP);
    TEST(JP_HL);
    TEST(JR); TEST(JR_NEG);
    TEST(JR_Z); TEST(JR_Z_NOJUMP);
    TEST(JR_NZ); TEST(JR_NZ_NOJUMP);
    TEST(JR_C); TEST(JR_C_NOJUMP);
    TEST(JR_NC); TEST(JR_NC_NOJUMP);
    TEST(RET);
    TEST(LD_BC); TEST(LD_DE); TEST(LD_HL); TEST(LD_SP);
    TEST(LD_B); TEST(LD_D); TEST(LD_H); TEST(LD_xHL); TEST(LD_C); TEST(LD_E); TEST(LD_L); TEST(LD_A);
    TEST(INC_B); TEST(INC_B_HSET); TEST(INC_B_NOHSET); TEST(INC_B_ZSET);
    TEST(INC_C); TEST(INC_C_HSET); TEST(INC_C_NOHSET); TEST(INC_C_ZSET);
    TEST(INC_D); TEST(INC_D_HSET); TEST(INC_D_NOHSET); TEST(INC_D_ZSET);
    TEST(INC_E); TEST(INC_E_HSET); TEST(INC_E_NOHSET); TEST(INC_E_ZSET);
    TEST(INC_H); TEST(INC_H_HSET); TEST(INC_H_NOHSET); TEST(INC_H_ZSET);
    TEST(INC_L); TEST(INC_L_HSET); TEST(INC_L_NOHSET); TEST(INC_L_ZSET);
    TEST(INC_A); TEST(INC_A_HSET); TEST(INC_A_NOHSET); TEST(INC_A_ZSET);
    TEST(INC_xHL); TEST(INC_xHL_HSET); TEST(INC_xHL_NOHSET); TEST(INC_xHL_ZSET);
    TEST(DEC_B); TEST(DEC_B_HSET); TEST(DEC_B_HSET2); TEST(DEC_B_ZSET);
    TEST(DEC_C); TEST(DEC_C_HSET); TEST(DEC_C_ZSET);
    TEST(DEC_D); TEST(DEC_D_HSET); TEST(DEC_D_ZSET);
    TEST(DEC_E); TEST(DEC_E_HSET); TEST(DEC_E_ZSET);
    TEST(DEC_H); TEST(DEC_H_HSET); TEST(DEC_H_ZSET);
    TEST(DEC_L); TEST(DEC_L_HSET); TEST(DEC_L_ZSET);
    TEST(DEC_A); TEST(DEC_A_HSET); TEST(DEC_A_ZSET);
    TEST(DEC_xHL); TEST(DEC_xHL_HSET); TEST(DEC_xHL_ZSET);
    TEST(INC_BC); TEST(INC_DE); TEST(INC_HL); TEST(INC_SP);
    TEST(DEC_BC); TEST(DEC_DE); TEST(DEC_HL); TEST(DEC_SP);
    TEST(LD_xBC_A); TEST(LD_xDE_A); TEST(LDI_xHL_A); TEST(LDD_xHL_A);
    TEST(LD_A_xBC); TEST(LD_A_xDE); TEST(LDI_A_xHL); TEST(LDD_A_xHL);
    TEST(LD_xNN_SP);
    TEST(RLCA); TEST(RLCA_C_SET); TEST(RLCA_C_PRESET);
    TEST(RLA); TEST(RLA_C_SET); TEST(RLA_C_PRESET);
    TEST(RRCA); TEST(RRCA_C_SET); TEST(RRCA_C_PRESET);
    TEST(RRA); TEST(RRA_C_SET); TEST(RRA_C_PRESET);
    TEST(DAA_L_LT_9); TEST(DAA_L_GT_9); TEST(DAA_H_PRESET); TEST(DAA_L_GT_9_AND_H_PRESET);
    TEST(DAA_U_LT_9); TEST(DAA_U_GT_9); TEST(DAA_C_PRESET); TEST(DAA_U_GT_9_AND_C_PRESET); TEST(DAA_Z_SET);
    TEST(SCF); TEST(SCF_Z_PRESET); TEST(SCF_NHZ_PRESET);
    TEST(LD_B_B); TEST(LD_B_C); TEST(LD_B_D); TEST(LD_B_E); TEST(LD_B_H); TEST(LD_B_L); TEST(LD_B_xHL); TEST(LD_B_A);
    TEST(LD_A_B); TEST(LD_A_C); TEST(LD_A_D); TEST(LD_A_E); TEST(LD_A_H); TEST(LD_A_L); TEST(LD_A_xHL); TEST(LD_A_A);
    TEST(LD_H_B); TEST(LD_H_C); TEST(LD_H_D); TEST(LD_H_E); TEST(LD_H_H); TEST(LD_H_L); TEST(LD_H_xHL); TEST(LD_H_A);
    TEST(LD_D_B); TEST(LD_D_C); TEST(LD_D_D); TEST(LD_D_E); TEST(LD_D_H); TEST(LD_D_L); TEST(LD_D_xHL); TEST(LD_D_A);
    TEST(LD_C_B); TEST(LD_C_C); TEST(LD_C_D); TEST(LD_C_E); TEST(LD_C_H); TEST(LD_C_L); TEST(LD_C_xHL); TEST(LD_C_A);
    TEST(LD_E_B); TEST(LD_E_C); TEST(LD_E_D); TEST(LD_E_E); TEST(LD_E_H); TEST(LD_E_L); TEST(LD_E_xHL); TEST(LD_E_A);
    TEST(LD_L_B); TEST(LD_L_C); TEST(LD_L_D); TEST(LD_L_E); TEST(LD_L_H); TEST(LD_L_L); TEST(LD_L_xHL); TEST(LD_L_A);
    TEST(LD_xHL_B); TEST(LD_xHL_D); TEST(LD_xHL_H); TEST(LD_xHL_C); TEST(LD_xHL_E); TEST(LD_xHL_L); TEST(LD_xHL_A);

    
    testcleanup(cpu);
    cout << dec;
    cout << "ran:  " << setw(3) << tests << setw(8) << "(" << setw(4) << assertPass+assertFail << " assertions)" << endl;
    cout << "pass: " << setw(3) << pass <<  setw(8) << "(" << setw(4) << assertPass << " assertions)" << endl;
    cout << "fail: " << setw(3) << fail <<  setw(8) << "(" << setw(4) << assertFail << " assertions)" << endl;
}



int main(void) {
    RunAllCpuTests();
    return 0;
}
