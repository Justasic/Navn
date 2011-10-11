CXX = c++
EXECUTABLE = @PACKAGE@
PERL = $(srcdir)/run-cc.pl
CFLAGS = -Wall -ansi -pipe -Wshadow -fPIC -pedantic -g -Wl,--export-dynamic $(LIBS) -I$(srcdir)/include -I$(srcdir)
LIBS = -lpthread -ldl -lnsl
MODFLAGS = -fPIC -g $(LIBS) -shared -Wl,-soname,
all: navn testmod m_ping m_join help modulehandler m_system searcher m_dns about_me \
	ctcp encyclopedia weather world_clock da_goat channel_logger m_terminal_input
	@echo " "
	@echo "Build complete. Run './$(EXECUTABLE)' to execute the binary."
	
navn:
	@echo "This bot was created in C++ by Lordofsraam and Justasic from Flux-Net"
	@echo "Navn Version @VERSION@-with-netflix-and-chicken"
	@echo " "
	@$(PERL) $(CXX) $(CFLAGS) -c main.cpp
	@$(PERL) $(CXX) $(CFLAGS) -c misc.cpp
	@$(PERL) $(CXX) $(CFLAGS) -c module.cpp
	@$(PERL) $(CXX) $(CFLAGS) -c segfault.cpp
	@$(PERL) $(CXX) $(CFLAGS) -c Socket.cpp
	@$(PERL) $(CXX) $(CFLAGS) -c Sepstream.cpp
	@$(PERL) $(CXX) $(CFLAGS) -c process.cpp
	@$(PERL) $(CXX) $(CFLAGS) -c thread.cpp
	@$(PERL) $(CXX) $(CFLAGS) -c privmsg.cpp
	@$(PERL) $(CXX) $(CFLAGS) -c thread.cpp
	@$(PERL) $(CXX) $(CFLAGS) -c timers.cpp
	@$(PERL) $(CXX) $(CFLAGS) -c user.cpp
	@$(PERL) $(CXX) $(CFLAGS) -c command.cpp
	@$(PERL) $(CXX) $(CFLAGS) -c channel.cpp
	@$(PERL) $(CXX) $(CFLAGS) -c INIReader.cpp
	@$(PERL) $(CXX) $(CFLAGS) -c log.cpp
	@$(PERL) $(CXX) $(CFLAGS)  main.o misc.o log.o segfault.o module.o Socket.o Sepstream.o privmsg.o command.o channel.o thread.o timers.o user.o INIReader.o process.o -o $(EXECUTABLE) -rdynamic
testmod:
	@$(PERL) $(CXX) $(CFLAGS) -c modules/testmod.cpp
	@$(PERL) $(CXX) $(MODFLAGS)modules/testmod.so -o modules/testmod.so testmod.o
m_ping:
	@$(PERL) $(CXX) $(CFLAGS) -c modules/m_ping.cpp
	@$(PERL) $(CXX) $(MODFLAGS)modules/m_ping.so -o modules/m_ping.so m_ping.o
m_join:
	@$(PERL) $(CXX) $(CFLAGS) -c modules/m_join.cpp
	@$(PERL) $(CXX) $(MODFLAGS)modules/m_join.so -o modules/m_join.so m_join.o
help:
	@$(PERL) $(CXX) $(CFLAGS) -c modules/help.cpp
	@$(PERL) $(CXX) $(MODFLAGS)modules/help.so -o modules/help.so help.o
modulehandler:
	@$(PERL) $(CXX) $(CFLAGS) -c modules/modulehandler.cpp
	@$(PERL) $(CXX) $(MODFLAGS)modules/modulehandler.so -o modules/modulehandler.so modulehandler.o
m_system:
	@$(PERL) $(CXX) $(CFLAGS) -c modules/m_system.cpp
	@$(PERL) $(CXX) $(MODFLAGS)modules/m_system.so -o modules/m_system.so m_system.o
searcher:
	@$(PERL) $(CXX) $(CFLAGS) -c modules/searcher.cpp
	@$(PERL) $(CXX) $(MODFLAGS)modules/searcher.so -o modules/searcher.so searcher.o
m_dns:
	@$(PERL) $(CXX) $(CFLAGS) -c modules/m_dns.cpp
	@$(PERL) $(CXX) $(MODFLAGS)modules/m_dns.so -o modules/m_dns.so m_dns.o
about_me:
	@$(PERL) $(CXX) $(CFLAGS) -c modules/about_me.cpp
	@$(PERL) $(CXX) $(MODFLAGS)modules/about_me.so -o modules/about_me.so about_me.o
ctcp:
	@$(PERL) $(CXX) $(CFLAGS) -c modules/ctcp.cpp
	@$(PERL) $(CXX) $(MODFLAGS)modules/ctcp.so -o modules/ctcp.so ctcp.o
encyclopedia:
	@$(PERL) $(CXX) $(CFLAGS) -c modules/encyclopedia.cpp
	@$(PERL) $(CXX) $(MODFLAGS)modules/encyclopedia.so -o modules/encyclopedia.so encyclopedia.o
weather:
	@$(PERL) $(CXX) $(CFLAGS) -c modules/weather.cpp
	@$(PERL) $(CXX) $(MODFLAGS)modules/weather.so -o modules/weather.so weather.o
world_clock:
	@$(PERL) $(CXX) $(CFLAGS) -c modules/world_clock.cpp
	@$(PERL) $(CXX) $(MODFLAGS)modules/world_clock.so -o modules/world_clock.so world_clock.o
da_goat:
	@$(PERL) $(CXX) $(CFLAGS) -c modules/da_goat.cpp
	@$(PERL) $(CXX) $(MODFLAGS)modules/da_goat.so -o modules/da_goat.so da_goat.o
channel_logger:
	@$(PERL) $(CXX) $(CFLAGS) -c modules/channel_logger.cpp
	@$(PERL) $(CXX) $(MODFLAGS)modules/channel_logger.so -o modules/channel_logger.so channel_logger.o
m_terminal_input:
	@$(PERL) $(CXX) $(CFLAGS) -c modules/m_terminal_input.cpp
	@$(PERL) $(CXX) $(MODFLAGS)modules/m_terminal_input.so -o modules/m_terminal_input.so m_terminal_input.o


clean:
	rm -f *.o *.so $(EXECUTABLE) config.h
	@cd $(srcdir)/include
	rm -f *.o *.so
	@cd ..
