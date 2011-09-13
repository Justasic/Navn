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
	$(CC) $(LDFLAGS) main.cpp
	$(CC) $(LDFLAGS) misc.cpp
	$(CC) $(LDFLAGS) module.cpp
	$(CC) $(LDFLAGS) Socket.cpp
	$(CC) $(LDFLAGS) Sepstream.cpp
	$(CC) $(LDFLAGS) privmsg.cpp
	$(CC) $(LDFLAGS) command.cpp
	$(CC) $(LDFLAGS) channel.cpp
	$(CC) $(LDFLAGS) thread.cpp
	$(CC) $(LDFLAGS) timers.cpp
	$(CC) $(LDFLAGS) user.cpp
	@echo "Linking $(EXECUTABLE).."
	$(CC) $(CFLAGS) $(EXECUTABLE) *.o $(EXTRA) $(LIBS)
	@echo "Build complete. Run './$(EXECUTABLE)' to execute the binary."

.PHONY : clean
clean:
	rm -f *.o $(EXECUTABLE)