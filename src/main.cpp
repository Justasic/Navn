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
iSupport isupport;
unsigned int loopcount;

// Global Pointers
IRCProto *ircproto;
SocketIO *sock;
BotConfig *Config;
Module *LastRunModule;

// Generic class for the socket processing and such
class Main : public Timer, public SocketIO
{
  bool NeedsReconnect;
  int startcount;
public:
  Main() : Timer(Config->ReconnectTime, time(NULL), true), SocketIO(Config->Server, Config->Port), NeedsReconnect(false), startcount(0)
  {
    Log() << "Connecting to server in " << Config->ReconnectTime << " seconds.";
    sock = this;
    ircproto = new IRCProto();
  }

  // Exiting the application
  ~Main()
  {
    GarbageCollect();
    Log(LOG_TERMINAL) << "Bye!\033[0m";
  }

  // Reconnecting to a server
  void Tick(time_t)
  {
    if(this->NeedsReconnect)
      this->EstablishConnection();
    else
      startcount = 0;
  }

  // Decide if we need to reconnect or not
  inline void SetReconnect(bool status)
  {
    this->NeedsReconnect = status;
  }

  // Run garbage collector for modules
  inline void RunGarbageCollect()
  {
    Log(LOG_DEBUG) << "Doing a garbage run.";
    FOREACH_MOD(I_OnGarbageCollect, OnGarbageCollect());
  }

  // Attempt to connect to our server in the config
  void EstablishConnection()
  {
    try
    {
      if(quitting)
	return;

      startcount++;
      Log() << "Connecting to server '" << this->Server << ":" << this->Port << "'";

      if(Config->Server.empty())
	throw SocketException("No Server Specified.");
      if(Config->Port.empty())
	throw SocketException("No Port Specified.");

      FOREACH_MOD(I_OnPreConnect, OnPreConnect(Config->Server, Config->Port));

      this->Connect();

      if(!ircproto)
      {
	Log(LOG_WARN) << "IRCProto was not created or initialized!!";
	ircproto = new IRCProto();
      }

      ircproto->introduce_client(Config->BotNick, Config->Ident, Config->Realname);

      FOREACH_MOD(I_OnPostConnect, OnPostConnect(sock));
      this->SetReconnect(false);
    }
    catch (SocketException &e)
    {
      Log() << "Socket Exception Caught: " << e.description();
      if(startcount >= Config->ReconnectTries)
	throw CoreException("Cannot connect to server!");
      else
      {
	Log() << "Reconnecting to server in " << Config->ReconnectTime << " seconds.";
	this->SetReconnect(true);
      }
    }
  }
};

/**
 * \fn int main (int argcx, char** argvx, char *envp[])
 * \brief Main Entry point for the bot
 * \param argc the number of args provided by the system
 * \param argv the args in a c-string array provided by the system
 * \param envp[] Environment variables (deprecated)
 */
int main (int argcx, char** argvx, char *envp[])
{
  SET_SEGV_LOCATION();
  unsigned int loopcount = 0;
  time_t last_check = time(NULL);
  try
  {
    startup(argcx, argvx, envp);
    Main *m = new Main();
    m->EstablishConnection();

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
	m->SetReconnect(true);
      }

      /* Process Timers */
      /***********************************/
      if(time(NULL) - last_check >= 3)
      {
	loopcount = 0;
	TimerManager::TickTimers(time(NULL));
	m->RunGarbageCollect();
	last_check = time(NULL);
      }
    } //while loop ends here
    delete m;
  }//try ends here
  catch(const CoreException& e)
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

