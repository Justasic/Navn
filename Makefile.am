AUTOMAKE_OPTIONS 	= foreign
PERL = $(srcdir)/run-cc.pl
LIBS = -pthread -ldl -lnsl -lrt -lstdc++
INCLUDES = -I$(srcdir)/include -I$(srcdir)
CFLAGS = -Wall -ansi -pipe -Wshadow -fPIC -DPIC  -L. -fno-leading-underscore -pedantic -g -Wl,--export-dynamic $(INCLUDES)
MODFLAGS = -fPIC -g $(LIBS) -shared -Wl,-soname,

navn:
	@echo "This bot was created in C++ by Lordofsraam and Justasic from Flux-Net"
	@echo "Navn Version $(VERSION)-with-netflix-and-chicken"
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
	@$(PERL) $(CXX) $(CFLAGS) -L. main.o misc.o log.o segfault.o module.o Socket.o Sepstream.o privmsg.o command.o channel.o thread.o timers.o user.o INIReader.o process.o -o $(PACKAGE) $(LIBS) -rdynamic

about_me.so:
	@$(PERL) $(CXX) $(CFLAGS) -c modules/about_me.cpp
	@$(PERL) $(CXX) $(LIBS) $(MODFLAGS)modules/about_me.cpp -o modules/about_me.so about_me.o
channel_logger.so:
	@$(PERL) $(CXX) $(CFLAGS) -c modules/channel_logger.cpp
	@$(PERL) $(CXX) $(LIBS) $(MODFLAGS)modules/channel_logger.cpp -o modules/channel_logger.so channel_logger.o
ctcp.so:
	@$(PERL) $(CXX) $(CFLAGS) -c modules/ctcp.cpp
	@$(PERL) $(CXX) $(LIBS) $(MODFLAGS)modules/ctcp.cpp -o modules/ctcp.so ctcp.o
da_goat.so:
	@$(PERL) $(CXX) $(CFLAGS) -c modules/da_goat.cpp
	@$(PERL) $(CXX) $(LIBS) $(MODFLAGS)modules/da_goat.cpp -o modules/da_goat.so da_goat.o
encyclopedia.so:
	@$(PERL) $(CXX) $(CFLAGS) -c modules/encyclopedia.cpp
	@$(PERL) $(CXX) $(LIBS) $(MODFLAGS)modules/encyclopedia.cpp -o modules/encyclopedia.so encyclopedia.o
help.so:
	@$(PERL) $(CXX) $(CFLAGS) -c modules/help.cpp
	@$(PERL) $(CXX) $(LIBS) $(MODFLAGS)modules/help.cpp -o modules/help.so help.o
m_dns.so:
	@$(PERL) $(CXX) $(CFLAGS) -c modules/m_dns.cpp
	@$(PERL) $(CXX) $(LIBS) $(MODFLAGS)modules/m_dns.cpp -o modules/m_dns.so m_dns.o
m_join.so:
	@$(PERL) $(CXX) $(CFLAGS) -c modules/m_join.cpp
	@$(PERL) $(CXX) $(LIBS) $(MODFLAGS)modules/m_join.cpp -o modules/m_join.so m_join.o
modulehandler.so:
	@$(PERL) $(CXX) $(CFLAGS) -c modules/modulehandler.cpp
	@$(PERL) $(CXX) $(LIBS) $(MODFLAGS)modules/modulehandler.cpp -o modules/modulehandler.so modulehandler.o
m_ping.so:
	@$(PERL) $(CXX) $(CFLAGS) -c modules/m_ping.cpp
	@$(PERL) $(CXX) $(LIBS) $(MODFLAGS)modules/m_ping.cpp -o modules/m_ping.so m_ping.o
m_system.so:
	@$(PERL) $(CXX) $(CFLAGS) -c modules/m_system.cpp
	@$(PERL) $(CXX) $(LIBS) $(MODFLAGS)modules/m_system.cpp -o modules/m_system.so m_system.o
m_terminal_input.so:
	@$(PERL) $(CXX) $(CFLAGS) -c modules/m_terminal_input.cpp
	@$(PERL) $(CXX) $(LIBS) $(MODFLAGS)modules/m_terminal_input.cpp -o modules/m_terminal_input.so m_terminal_input.o
searcher.so:
	@$(PERL) $(CXX) $(CFLAGS) -c modules/searcher.cpp
	@$(PERL) $(CXX) $(LIBS) $(MODFLAGS)modules/searcher.cpp -o modules/searcher.so searcher.o
SimpleMod.so:
	@$(PERL) $(CXX) $(CFLAGS) -c modules/SimpleMod.cpp
	@$(PERL) $(CXX) $(LIBS) $(MODFLAGS)modules/SimpleMod.cpp -o modules/SimpleMod.so SimpleMod.o
testmod.so:
	@$(PERL) $(CXX) $(CFLAGS) -c modules/testmod.cpp
	@$(PERL) $(CXX) $(LIBS) $(MODFLAGS)modules/testmod.cpp -o modules/testmod.so testmod.o
weather.so:
	@$(PERL) $(CXX) $(CFLAGS) -c modules/weather.cpp
	@$(PERL) $(CXX) $(LIBS) $(MODFLAGS)modules/weather.cpp -o modules/weather.so weather.o
world_clock.so:
	@$(PERL) $(CXX) $(CFLAGS) -c modules/world_clock.cpp
	@$(PERL) $(CXX) $(LIBS) $(MODFLAGS)modules/world_clock.cpp -o modules/world_clock.so world_clock.o

modules: about_me.so channel_logger.so ctcp.so da_goat.so encyclopedia.so help.so m_dns.so m_join.so modulehandler.so m_ping.so m_system.so m_terminal_input.so searcher.so SimpleMod.so testmod.so weather.so world_clock.so

all-am: navn about_me.so channel_logger.so ctcp.so da_goat.so encyclopedia.so help.so m_dns.so m_join.so modulehandler.so m_ping.so m_system.so m_terminal_input.so searcher.so SimpleMod.so testmod.so weather.so world_clock.so
	@echo " "
	@echo "Build complete. Run \'./$(PACKAGE)\' to execute the binary."
clean:
	rm -f *.o *.so $(PACKAGE) config.h config.status
	rm -f *.o *.so $(srcdir)/modules/*.so
