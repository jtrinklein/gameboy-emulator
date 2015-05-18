CXX=g++
CC=g++
CXXFLAGS=-g -Wall -W -std=c++11
LDFLAGS=-L./lib
LDLIBS=-framework OpenGL -framework AppKit -framework IOKit -framework Carbon -framework Foundation -lGbSndEmu -lIrrlicht
OBJ=libcarts.o cart.o gameboy.o gpu.o pad.o mmu.o cpu.o apu.o main.o
testOBJ=libcarts.o cart.o cpu_tests.o mock_render.o test_gameboy.o
INCLUDE=-I ./irrlicht/include -I ./source -I ./Gb_Snd_Emu/include -I Gb_Snd_Emu/include/gb_apu
testINCLUDE=-I ./source -I ./tests


gameboy: clean $(OBJ)
	$(CC) $(CXXFLAGS) $(LDFLAGS) -o $@ $(INCLUDE) $(OBJ) $(LDLIBS)

main.o:
	$(CC) $(CXXFLAGS) -o $@ $(INCLUDE) -c source/main.cpp

apu.o:
	$(CC) $(CXXFLAGS) -o $@ $(INCLUDE) -c source/apu.cpp

gpu.o:
	$(CC) $(CXXFLAGS) -o $@ $(INCLUDE) -c source/gpu.cpp

mmu.o:
	$(CC) $(CXXFLAGS) -o $@ $(INCLUDE) -c source/mmu.cpp

cpu.o:
	$(CC) $(CXXFLAGS) -o $@ $(INCLUDE) -c source/cpu.cpp

pad.o:
	$(CC) $(CXXFLAGS) -o $@ $(INCLUDE) -c source/pad.cpp

cart.o:
	$(CC) $(CXXFLAGS) -o $@ $(INCLUDE) -c source/cart.cpp

gameboy.o:
	$(CC) $(CXXFLAGS) -o $@ $(INCLUDE) -c source/gameboy.cpp

all: gameboy cputest endtest

cputest: clean $(testOBJ)
	$(CC) $(CXXFLAGS) -o $@ $(testOBJ)

endtest:
	$(CC) $(CXXFLAGS) -o $@ util/endtest.cpp

libcarts.o:
	$(CC) -g -o $@ -c source/GBCarts.c


testrender.o: testrender.cpp

mock_render.o:
	$(CC) $(CXXFLAGS) -o $@ -D BUILD_TEST=1 $(testINCLUDE) -c tests/mock_render.cpp

test_gameboy.o:
	$(CC) $(CXXFLAGS) -o $@ -D BUILD_TEST=1 $(testINCLUDE) -c gameboy.cpp

cpu_tests.o:
	$(CC) $(CXXFLAGS) -o $@ -D BUILD_TEST=1 $(testINCLUDE) -c tests/cpu_tests.cpp


.PHONY: clean
clean:
	-rm $(OBJ) $(ROBJ) $(testOBJ) testrender gbemu cputest
