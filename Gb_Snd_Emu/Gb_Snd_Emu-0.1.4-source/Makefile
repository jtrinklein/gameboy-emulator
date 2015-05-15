
CXXFLAGS=-g -I ./ -I ./gb_apu/

SDLINC=`sdl-config --cflags`
SDLFLAGS=`sdl-config --libs` $(SDLINC)


Gb_Snd_Emu: clean Blip_Buffer.o Gb_Apu.o Gb_Oscs.o Multi_Buffer.o
	ar rvs libGbSndEmu.a Blip_Buffer.o Gb_Apu.o Gb_Oscs.o Multi_Buffer.o

Blip_Buffer.o:
	g++ -o $@ -c $(SDLINC) $(CXXFLAGS) ./gb_apu/Blip_Buffer.cpp

Gb_Apu.o:
	g++ -o $@ -c $(SDLINC) $(CXXFLAGS) ./gb_apu/Gb_Apu.cpp

Gb_Oscs.o:
	g++ -o $@ -c $(SDLINC) $(CXXFLAGS) ./gb_apu/Gb_Oscs.cpp

Multi_Buffer.o:
	g++ -o $@ -c $(SDLINC) $(CXXFLAGS) ./gb_apu/Multi_Buffer.cpp


demo: Gb_Snd_Emu
	g++ -o $@ -lGbSndEmu -L./ $(CXXFLAGS) $(SDLFLAGS) Basic_Gb_Apu.cpp Wave_Writer.cpp Sound_Queue.cpp demo.cpp


demo_sdl: Gb_Snd_Emu
	g++ -o $@ -lGbSndEmu -L./ $(CXXFLAGS) $(SDLFLAGS) Basic_Gb_Apu.cpp Sound_Queue.cpp demo_sdl.cpp

.PHONY: clean
clean:
	-rm libGbSndEmu.a *.o demo demo_sdl

all: demo demo_sdl
