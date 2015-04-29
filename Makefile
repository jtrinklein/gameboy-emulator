CXX=g++
CC=g++
CXXFLAGS=-g -Wall -W
LDFLAGS="-stdlib=libstdc++"
LDLIBS=/usr/local/lib
OBJ=libcarts.o cart.o gameboy.o render.o
testOBJ=libcarts.o cart.o cpu_tests.o mock_render.o test_gameboy.o
INCLUDE=-I ./irrlicht-1.8.1/include
testINCLUDE=-I ./ -I ./tests

ifeq ($(OS),Windows_NT)
	CXXFLAGS += -D WIN32
	irrLoc=./irrlicht-1.8.1/lib/Win32-gcc/
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
		CXXFLAGS += -D LINUX
	endif
	ifeq ($(UNAME_S),Darwin)
		CXXFLAGS += -D OSX
		irrLoc=
		irrBuild=xcodebuild -scheme libIrrlicht.a -project irrlicht-1.8.1/source/Irrlicht/MacOSX/MacOSX.xcodeproj build
	endif
endif

all: gameboy cputest endtest

gameboy: clean $(OBJ) libIrrlicht.a
	$(CC) $(CXXFLAGS) -o $@ $(OBJ) -l libIrrlicht.a

cputest: clean $(testOBJ)
	$(CC) $(CXXFLAGS) -o $@ $(testOBJ)

endtest:
	$(CC) $(CXXFLAGS) -o $@ util/endtest.cpp

libIrrlicht.a:
	$(irrBuild)

libcarts.o: 
	$(CC) -g -o $@ -c EMULib/GBCarts.c

render.o:
	$(CC) $(CXXFLAGS) -o $@ $(INCLUDE) -c render.cpp

cart.o: cart.cpp

gameboy.o:
	$(CC) $(CXXFLAGS) -o $@ $(INCLUDE) -c gameboy.cpp

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

