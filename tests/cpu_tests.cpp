#include "gameboy.h"
#include <iostream>
#include <map>
#include <functional>

using namespace std;
CPU* cpu;
template<typename T>
bool equals(T a, T b) {
    return a == b;
}

#define STR(x) #x
#define JOIN(x,y) x##y
bool runTest(string title, function<bool(void)> testFn) {
    string dots = "........................................";
    int c = 40 - title.length();
    
    cout << title << dots.substr(0, c);
    bool result = testFn();
    if(result) {
        cout << " ok" << endl;
    } else {
        cout << " FAIL" << endl;
    }
    return result;
}
#define TEST(fn)\
if( !runTest( #fn, fn)) {\
    fail++;\
} else {\
    pass++;\
}\
tests++

void setup(CPU* cpu) {
    cpu->OAM  = new byte[0x0100];
    cpu->ZRAM = new byte[0x0100];
    cpu->WRAM = new byte[0x2000];
    cpu->ERAM = new byte[0x2000];
    cpu->VRAM = new byte[0x2000];
    cpu->ROM  = new byte[0x8000];
}

void cleanup(CPU* cpu) {
    delete [] cpu->ROM;
    delete [] cpu->OAM;
    delete [] cpu->WRAM;
    delete [] cpu->ZRAM;
    delete [] cpu->ERAM;
    delete [] cpu->VRAM;
}

bool CALL_a16() {
    cpu->SP.W = 0xDFFF;
    cpu->PC.W = 0x101;
    cpu->ROM[0x101] = 0x34;
    cpu->ROM[0x102] = 0x12;
    
    
    execOp(cpu, cpu->gpu, 0xCD);
    return equals<word>(cpu->PC.W, 0x1234);
    
}
int main(void) {
    word tests = 0, pass = 0, fail = 0;
    cpu = new CPU();
    setup(cpu);
    TEST(CALL_a16);
    
    cleanup(cpu);
}