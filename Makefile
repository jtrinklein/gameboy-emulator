CXX=g++
CC=g++
CXXFLAGS=-g -Wall -W -Werror 
LDFLAGS="-stdlib=libstdc++"
LDLIBS=/usr/local/lib
OBJ=libcarts.o cart.o main.o

gbemu: clean $(OBJ)
	$(CC) $(CXXFLAGS) -o $@ $(OBJ)

libcarts.o: 
	$(CC) -g -o $@ -c EMULib/GBCarts.c

cart.o: cart.cpp

main.o: main.cpp

.PHONY: clean
clean:
	-rm libcarts.o cart.o main.o gbemu

