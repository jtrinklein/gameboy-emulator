CXX=g++
CC=g++
CXXFLAGS=-g -Wall -W
LDFLAGS="-stdlib=libstdc++"
LDLIBS=/usr/local/lib
OBJ=libcarts.o cart.o gameboy.o
ROBJ=testrender.o render.o
testOBJ=libcarts.o cart.o cpu_tests.o mock_render.o test_gameboy.o

gbemu: clean $(OBJ)
	$(CC) $(CXXFLAGS) -o $@ $(OBJ)

	
libcarts.o: 
	$(CC) -g -o $@ -c EMULib/GBCarts.c

testrender.o: testrender.cpp

render.o: render.cpp

cart.o: cart.cpp

gameboy.o: gameboy.cpp


mock_render.o:
	$(CC) -g -o $@ -D BUILD_TEST=1 -I ./ -I ./tests -c tests/mock_render.cpp

test_gameboy.o:
	$(CC) -g -o $@ -D BUILD_TEST=1 -I ./ -I ./tests -c gameboy.cpp

cpu_tests.o:
	$(CC) -g -o $@ -D BUILD_TEST=1 -I ./ -I ./tests -c tests/cpu_tests.cpp

cputest: clean $(testOBJ)
	$(CC) -g $(CXXFLAGS) -o $@ $(testOBJ)


.PHONY: clean
clean:
	-rm $(OBJ) $(ROBJ) $(testOBJ) testrender gbemu cputest

