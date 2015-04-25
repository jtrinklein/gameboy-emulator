CXX=g++
CC=g++
CXXFLAGS=-g -Wall -W
LDFLAGS="-stdlib=libstdc++"
LDLIBS=/usr/local/lib
OBJ=libcarts.o cart.o gameboy.o
ROBJ=testrender.o render.o
testOBJ=cpu_test.o render_mock.o

gbemu: clean $(OBJ)
	$(CC) $(CXXFLAGS) -o $@ $(OBJ)

testrender: clean $(ROBJ)
	$(CC) $(CXXFLAGS) -o $@ $(ROBJ) irrlicht-1.8.1/lib/MacOSX/libIrrlicht.a
	
libcarts.o: 
	$(CC) -g -o $@ -c EMULib/GBCarts.c

testrender.o: testrender.cpp

render.o: render.cpp

cart.o: cart.cpp

gameboy.o: gameboy.cpp

cputest: clean cpu_test.o render_mock.o
.PHONY: clean
clean:
	-rm $(OBJ) $(ROBJ) testrender gbemu

