CC=g++
IDIR=include/
CFLAGS=-Wall -ansi -pedantic -g -o 
LDFLAGS=-Wall -ansi -pedantic -g -c -I$(IDIR)
EXTRA=inireader/INIReader.cpp inireader/ini.c
LIBS=-lpthread
EXECUTABLE=navn

all:
	@echo "This bot was created in C++ by Lordofsraam and Justasic from Flux-Net"
	$(CC) $(LDFLAGS) *.cpp
	$(CC) $(CFLAGS) $(EXECUTABLE) *.o $(EXTRA) $(LIBS)

.PHONY : clean
clean:
	rm -f *.o $(EXECUTABLE)