CXX = c++
EXECUTABLE = @PACKAGE@
PERL = $(srcdir)/run-cc.pl
CFLAGS = -Wall -ansi -pipe -Wshadow -pedantic -g -Wl,--export-dynamic -ldl -lnsl -I$(srcdir)/include -I$(srcdir)
LIBS = -lpthread -ldl
MODFLAGS = -fPIC -g -lnsl $(LIBS) -shared -Wl,-soname,
all: 
	@echo "This bot was created in C++ by Lordofsraam and Justasic from Flux-Net"
	@echo "Navn Version @VERSION@-with-netflix-and-chicken"
	@echo " "
	@$(PERL) $(CXX) $(CFLAGS) -c main.cpp
	@$(PERL) $(CXX) $(CFLAGS) -c misc.cpp
	@$(PERL) $(CXX) $(CFLAGS) -c module.cpp
	@$(PERL) $(CXX) $(CFLAGS) -c Socket.cpp
	@$(PERL) $(CXX) $(CFLAGS) -c Sepstream.cpp
	@$(PERL) $(CXX) $(CFLAGS) -c process.cpp
	@$(PERL) $(CXX) $(CFLAGS) -c privmsg.cpp
	@$(PERL) $(CXX) $(CFLAGS) -c thread.cpp
	@$(PERL) $(CXX) $(CFLAGS) -c timers.cpp
	@$(PERL) $(CXX) $(CFLAGS) -c user.cpp
	@$(PERL) $(CXX) $(CFLAGS) -c command.cpp
	@$(PERL) $(CXX) $(CFLAGS) -c channel.cpp
	@$(PERL) $(CXX) $(CFLAGS) -c INIReader.cpp
	@$(PERL) $(CXX) $(CFLAGS) $(LIBS) main.o misc.o module.o Socket.o Sepstream.o privmsg.o command.o channel.o thread.o timers.o user.o INIReader.o process.o -o $(EXECUTABLE) -rdynamic
	@$(PERL) $(CXX) $(CFLAGS) -c testmod.cpp
	@$(PERL) $(CXX) $(MODFLAGS)testmod.so -o testmod.so testmod.o
	@$(PERL) $(CXX) $(CFLAGS) -c ping_pong.cpp
	@$(PERL) $(CXX) $(MODFLAGS)ping_pong.so -o ping_pong.so ping_pong.o
	@$(PERL) $(CXX) $(CFLAGS) -c join.cpp
	@$(PERL) $(CXX) $(MODFLAGS)join.so -o join.so join.o
	@echo " "
	@echo "Build complete. Run './$(EXECUTABLE)' to execute the binary."

clean:
	rm -f *.o *.so $(EXECUTABLE) config.h
