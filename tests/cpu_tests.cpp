#include <iostream>
#include <map>
#include <functional>
#include "tests.h"
#include "gameboy.h"
#include "render.h"

using namespace std;
CPU* cpu;
string dots = "........................................";
#define ID_NEQUAL 1

#define STR(x) #x
#define JOIN(x,y) x##y

#define eql(actual, expected) \
    if(actual == expected) {\
        return;\
    }\
    cout << "FAIL" << endl;\
    cout << STR(actual) << endl;\
    cout << hex << actual << " should equal " << expected << endl;\
    throw ID_NEQUAL

#define fset(f) eql((cpu->AF.B.h & f), f)
#define TEST(fn)\
beforeTest();\
try {\
    cout << #fn << dots.substr(0, 40 - getTitleLength( #fn ));\
    fn();\
    pass++;\
    cout << "OK" << endl;\
} catch(...) {\
    fail++;\
}\
tests++


int getTitleLength(string title) {
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
    
    runOps(ops);
    
    eql(cpu->PC.W, 0x1234);
    eql(cpu->SP.W, 0xDFFE);
}

void CALL_Z() {
    byte ops = 1;
    
    setFlag(FLAG_Z);
    
    rom(0x100, 0xCC);//CALL Z,a16
    rom(0x101, 0x34);
    rom(0x102, 0x12);//0x1234
    
    runOps(ops);
    
    eql(cpu->PC.W, 0x1234);
    eql(cpu->SP.W, 0xDFFE);
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
    eql(cpu->SP.W, 0xDFFE);
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
    eql(cpu->SP.W, 0xDFFE);
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
    eql(cpu->SP.W, 0xDFFE);
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
    eql(cpu->SP.W, 0xDFFE);
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
    
    cpu->BC.W = 0x1C80;

    rom(0x100, 0xC5); //PUSH bc
    rom(0x101, 0x00); //NOP
    rom(0xDFFF, 0x00);
    rom(0xDFFE, 0x00);
    
    runOps(ops);
    
    eql(cpu->PC.W, 0x101);
    eql(cpu->SP.W, 0xDFFD);
    eql(read(0xDFFF), 0x80);
    eql(read(0xDFFE), 0x0C);
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
    eql(read(0xDFFF), 0x80);
    eql(read(0xDFFE), 0x0C);
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
    eql(read(0xDFFF), 0x80);
    eql(read(0xDFFE), 0x0C);
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

    //eql(cpu->PC.W, 0x1FF);
}

void RunAllCpuTests(void) {
    word tests = 0, pass = 0, fail = 0;
    cpu = setup();
    
    TEST(PUSH_BC);
    TEST(PUSH_DE);
    TEST(PUSH_HL);
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
    
    cleanup(cpu);
    cout << dec;
    cout << "ran:  " << tests << endl;
    cout << "pass: " << pass << endl;
    cout << "fail: " << fail << endl;
}

#ifdef BUILD_TEST

int main(void) {
    RunAllCpuTests();
    return 0;
}
#endif
