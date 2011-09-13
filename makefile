CC=g++
IDIR=include/
CFLAGS=-Wall -ansi -pedantic -g -o 
LDFLAGS=-Wall -ansi -pipe -Wshadow -pedantic -g -c -I$(IDIR) -Iinireader/ -I.
EXTRA=inireader/INIReader.cpp inireader/ini.c
LIBS=-lpthread
EXECUTABLE=navn

all:
	@echo "This bot was created in C++ by Lordofsraam and Justasic from Flux-Net"
	@echo "This is probably the lamest make file i have ever seen -Justasic"
	$(CC) $(LDFLAGS) *.cpp
	$(CC) $(CFLAGS) $(EXECUTABLE) *.o $(EXTRA) $(LIBS)
	@echo "Build complete. Run './$(EXECUTABLE)' to execute the binary."

.PHONY : clean
clean:
	rm -f *.o $(EXECUTABLE)