/* Navn IRC bot -- Main entry point file
 *
 * (C) 2011-2012 Azuru
 * Contact us at Development@Azuru.net
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Anope by The Anope Team.
 */
/**
 *\file  main.cpp
 *\brief Contains the main() function.
 *\mainpage Documentation for Navn - The C++ IRC Sockets bot by Flux-net
 *\section history Navn's History
 * Navn was started by Lordofsraam because he didn't like depending
 *  on an IRC client to run his scripts for the IRC channel. It really staterted
 * out as a hunt to be able to tell the time in the IRC channel because
 * Justasic lived in a different timezone. From there it evolved to what you see today
 * with the help of Justasic coming in to help with the code and debugging.
 * If you want a really detailed history go look at our svn commit log in googlecode (old)
 * or our Git history on gitorious.
 * \section dev Further Development
 * Navn is always growing and getting better. If you would like to help, contact Justasic or Lordofsraam.
 * \subsection mods Module Development
 * If you think the system stuff is too complicated, but you would still like to code for Navn to make more special
 * feature available, we have made it as easy as possible to make Modules.
 * See the examples tab for an example Module.
 */
#include "modules.h"

#ifdef HAVE_SETJMP_H
jmp_buf sigbuf;
#endif

// Global variables
char **my_av, * *my_envp;
bool nofork = false, dev = false, protocoldebug = false, IsOper = false, quitting = false;
bool started = false, nocolor = false, istempnick = false, memdebug = false;
Flux::string binary_path, bot_bin, binary_dir, quitmsg;
const Flux::string password = make_pass();
char segv_location[255];
time_t starttime = 0;
iSupport isupport;
unsigned int startcount, loopcount;

// Global Pointers
IRCProto *ircproto;
SocketIO *sock;
BotConfig *Config;
Module *LastRunModule;

E void Connect();

class ReconnectTimer : public Timer
{
public:
	ReconnectTimer() : Timer(Config->ReconnectTime)
	{
		Log() << "Reconnecting to server in " << Config->ReconnectTime << " seconds.";
	}
	void Tick(time_t)
	{
		Connect();
	}
};

/**
 * \fn bool SocketIO::Read(const Flux::string &buf) const
 * \brief Read from a socket for the IRC processor
 * \param buffer Raw socket buffer
 */
void Connect()
{
	try
	{
		if(quitting)
			return;

		++startcount;
		Log() << "Connecting to server '" << Config->Server << ":" << Config->Port << "'";

		FOREACH_MOD(I_OnPreConnect, OnPreConnect(Config->Server, Config->Port));

		if(Config->Server.empty())
			throw SocketException("No Server Specified.");

		if(Config->Port.empty())
			throw SocketException("No Port Specified.");

		if(sock)
		{
			delete sock;
			sock = NULL;
		}

		FOREACH_MOD(I_OnPreConnect, OnPreConnect(Config->Server, Config->Port));

		sock = new SocketIO(Config->Server, Config->Port);
		sock->Connect();

		if(!sock)
			throw SocketException("Cannot create socket!");
		else
			startcount = 0;

		if(ircproto)
			ircproto->introduce_client(Config->BotNick, Config->Ident, Config->Realname);

		FOREACH_MOD(I_OnPostConnect, OnPostConnect(sock));
	}
	catch(SocketException &e)
	{
		Log(LOG_DEBUG) << "Socket Exception Caught: " << e.description();

		if(static_cast<int>(startcount) >= Config->ReconnectTries)
			throw CoreException("Cannot connect to server!");
		else
		{
			delete sock;
			sock = NULL;
			new ReconnectTimer();
		}
	}
}

/**
 * \fn int main (int argcx, char** argvx, char *envp[])
 * \brief Main Entry point for the bot
 * \param argc the number of args provided by the system
 * \param argv the args in a c-string array provided by the system
 * \param envp[] Not quite sure what this is lol
 */
int main(int argcx, char **argvx, char *envp[])
{
	SET_SEGV_LOCATION();
	startcount = 0;

	try
	{
		startup(argcx, argvx, envp);
		Connect();

		ircproto = new IRCProto();
		time_t last_check = time(NULL);

		// Introduce ourselves to the IRC server
		ircproto->introduce_client(Config->BotNick, Config->Ident, Config->Realname);

		FOREACH_MOD(I_OnPostConnect, OnPostConnect(sock));

		while(!quitting)
		{
			Log(LOG_RAWIO) << "Top of main loop";

			if(++loopcount >= 50)
				raise(SIGSEGV); //prevent loop bombs, raise a SIGSEGV to handle elsewhere.

			/* Process the socket engine */
			try
			{
				if(sock)
					sock->Process();
				else
					throw SocketException("No socket to read from!");
			}
			catch(SocketException &exc)
			{
				Log() << "Socket Exception: " << exc.description();
				Connect();
			}

			/* Process Timers */
			/***********************************/
			if(time(NULL) - last_check >= 3)
			{
				loopcount = 0;
				TimerManager::TickTimers(time(NULL));
				last_check = time(NULL);
			}

			/***********************************/
		}//while loop ends here

		GarbageCollect();
		Log(LOG_TERMINAL) << "Bye!\033[0m";
	}//try ends here
	catch(const CoreException &e)
	{
		/* we reset the terminal colors, this should be removed as it makes more issues than it is cool */
		Log(LOG_CRITICAL) << "\033[0mCore Exception Caught: " << e.GetReason();
		return EXIT_FAILURE;
	}
	catch(std::exception &ex)
	{
		Log(LOG_CRITICAL) << "\033[0mStandard Exception Caught: " << ex.what();
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

