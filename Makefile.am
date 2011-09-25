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
	@$(PERL) $(CXX) $(CFLAGS) -c modules/testmod.cpp
	@$(PERL) $(CXX) $(MODFLAGS)modules/testmod.so -o modules/testmod.so testmod.o
	@$(PERL) $(CXX) $(CFLAGS) -c modules/m_ping.cpp
	@$(PERL) $(CXX) $(MODFLAGS)modules/m_ping.so -o modules/m_ping.so m_ping.o
	@$(PERL) $(CXX) $(CFLAGS) -c modules/m_join.cpp
	@$(PERL) $(CXX) $(MODFLAGS)modules/m_join.so -o modules/m_join.so m_join.o
	@$(PERL) $(CXX) $(CFLAGS) -c modules/help.cpp
	@$(PERL) $(CXX) $(MODFLAGS)modules/help.so -o modules/help.so help.o
	@echo " "
	@echo "Build complete. Run './$(EXECUTABLE)' to execute the binary."

clean:
	rm -f *.o *.so $(EXECUTABLE) config.h
