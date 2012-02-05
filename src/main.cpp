/* All code is licensed under GNU General Public License GPL v3 (http://www.gnu.org/licenses/gpl.html) */
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
 * If you want a really detailed history go look at our svn commit log in googlecode.
 * \section dev Further Development
 * Navn is always growing and getting better. If you would like to help, contact Justasic or Lordofsraam.
 * \subsection mods Module Development
 * If you think the system stuff is too complicated, but you would still like to code for Navn to make more special
 * feature available, we have made it as easy as possible to make modules.
 * See the examples tab for an example module.
 */
#include "flux_net_irc.hpp"

bool SocketIO::Read(const Flux::string &buf) const
{
  if(buf.search_ci("ERROR :Closing link:")){
    FOREACH_MOD(I_OnSocketError, OnSocketError(buf));
    throw SocketException(buf.c_str());
    return false;
  }
  process(buf); /* Process the buffer for navn */
  return true;
}

uint32_t startcount, loopcount;
void Connect()
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
  if(sock){
    SocketIO *s = sock;
    delete s;
  }
  FOREACH_MOD(I_OnPreConnect, OnPreConnect(Config->Server, Config->Port));
  sock = new SocketIO(Config->Server, Config->Port);
  sock->Connect();
  if(ircproto){
    ircproto->user(Config->Ident, Config->Realname);
    ircproto->nick(Config->BotNick);
  }
  FOREACH_MOD(I_OnPostConnect, OnPostConnect(sock));
}

int main (int argcx, char** argvx, char *envp[])
{
  SET_SEGV_LOCATION();
  startcount = 0;
  try
  {
    startup(argcx, argvx, envp);
    SocketStart:
    try { Connect(); }
    catch(SocketException &e){
      if(startcount >= 3)
	throw CoreException(e.description().c_str());
      Log(LOG_DEBUG) << "Socket Exception Caught: " << e.description();
      goto SocketStart;
    }
    if(!sock)
      goto SocketStart;
    ircproto = new IRCProto();
    time_t last_check = time(NULL);
    
    //Set the username and nick
    ircproto->user(Config->Ident, Config->Realname);
    ircproto->nick(Config->BotNick);
    FOREACH_MOD(I_OnPostConnect, OnPostConnect(sock));
    
    while(!quitting){
      Log(LOG_RAWIO) << "Top of main loop";
      if(++loopcount >= 50)
	raise(SIGSEGV); //prevent loop bombs, we raise a segfault because the segfault handler will handle it better
      
      /* Process the socket engine */
      try { sock->Process(); }
      catch(SocketException &exc)
      {
	Log() << "Socket Exception: " << exc.description();
	try { Connect(); }
	catch(SocketException &ex){
	  Log() << "Socket Exception: " << ex.description();
	  throw CoreException(ex.description());
	}
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
    FOREACH_MOD(I_OnShutdown, OnShutdown());
    ModuleHandler::UnloadAll();
    ModuleHandler::SanitizeRuntime();
    Log(LOG_TERMINAL) << "\033[0m";
  }//try ends here
  catch(const CoreException& e){
    /* we reset the terminal colors, this should be removed as it makes more issues than it is cool */
    Log(LOG_TERMINAL) << "\033[0m";
    if(!Config)
      Log(LOG_TERMINAL) << "Core Exception Caught: " << e.GetReason();
    else
      Log() << "Core Exception Caught: " << e.GetReason();
    return EXIT_FAILURE;
  }
  catch(std::exception &ex)
  {
    Log(LOG_TERMINAL) << "\033[0m";
    if(!Config)
      Log(LOG_TERMINAL) << "Standard Exception Caught: " << ex.what();
    else
      Log() << "Standard Exception Caught: " << ex.what();
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

