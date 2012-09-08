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
char **my_av, **my_envp;
bool nofork = false, dev = false, protocoldebug = false, IsOper = false, quitting = false;
bool started = false, nocolor = false, istempnick = false, memdebug = false;
Flux::string binary_path, bot_bin, binary_dir, quitmsg;
const Flux::string password = make_pass();
char segv_location[255];
time_t starttime = 0;
iSupport isupport;
unsigned int startcount, loopcount;
class ConnectionSocket;
class ReconnectTimer;

// Global Pointers
IRCProto *ircproto;
ConnectionSocket *sock;
BotConfig *Config;
Module *LastRunModule;
extern ReconnectTimer *timer;

class ReconnectTimer : public Timer
{
public:
    ReconnectTimer() : Timer(Config->ReconnectTime)
    {
	Log() << "Reconnecting to server in " << Config->ReconnectTime << " seconds.";
	timer = this;
    }

    ~ReconnectTimer()
    {
	timer = NULL;
    }

    void Tick(time_t);
};

ReconnectTimer *timer;

class ConnectionSocket : public TCPSocket
{
public:
    // Connect to a specific address
    ConnectionSocket() : TCPSocket(Config->Server, static_cast<short>(Config->Port))
    {
	Log() << "Connecting to server '" << Config->Server << ":" << Config->Port << "' -> " << this->resolver->GetIP();
	FOREACH_MOD(I_OnPreConnect, OnPreConnect(Config->Server, Config->Port));
    }

    ~ConnectionSocket()
    {
	new ReconnectTimer();
	sock = NULL;
    }

    void ProcessError()
    {
	// TODO: This.
    }

    void ConnectNotify()
    {
	if(ircproto)
	    ircproto->introduce_client(Config->BotNick, Config->Ident, Config->Realname);

	FOREACH_MOD(I_OnPostConnect, OnPostConnect(sock));
    }

    // What was actually read from the socket.
    bool Read(const Flux::string &received)
    {
	process(received);
	return true;
    }
};

void ReconnectTimer::Tick(time_t)
{
    try
    {
	sock = new ConnectionSocket();
	sock->Connect();
    }
    catch(const SocketException &ex)
    {
	ex.Print();
    }

}

/** \fn void send_cmd(const char *fmt, ...)
 * \brief Sends something directly out the socket after being processed by vsnprintf
 * \param char* a string of what to send to the server including printf style format
 * \param va_list all the variables to be replaced with the printf style variables
 */
void send_cmd(const char *fmt, ...)
{
    char buffer[BUFSIZE] = "";
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    if(sock)
	sock->Send(buffer);
    else
    {
	Log(LOG_WARN) << "Attempted to send \"" << buffer << "\" to the server but no socket exists!";
	// Something is fucko, don't use the socket and try again.
	if(sock)
	    delete sock;

	new ReconnectTimer();
    }
    va_end(args);
}

/**
 * \fn int main (int argcx, char** argvx, char *envp[])
 * \brief Main Entry point for the bot
 * \param argc the number of args provided by the system
 * \param argv the args in a c-string array provided by the system
 * \param envp[] Environment variables (deprecated, only used to re-execute the bot)
 */
int main (int argcx, char** argvx, char *envp[])
{
    SET_SEGV_LOCATION();
    startcount = 0;
    try
    {
	startup(argcx, argvx, envp);
	try
	{
	    sock = new ConnectionSocket();
	    sock->Connect();
	} catch(const SocketException &ex)
	{
	    ex.Print();
	}
	//Connect();

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

	    if(!sock && !timer)
		new ReconnectTimer();

	    /* Process the socket engine */
	    SocketHandler::ProcessSockets();

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
    catch(const CoreException& ex)
    {
	/* we reset the terminal colors, this should be removed as it makes more issues than it is cool */
	//Log(LOG_CRITICAL) << "\033[0mCore Exception Caught: " << e.GetReason();
	ex.Print();
	return EXIT_FAILURE;
    }
    catch(std::exception &ex)
    {
	Log(LOG_CRITICAL) << "\033[0mStandard Exception Caught: " << ex.what();
	return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

