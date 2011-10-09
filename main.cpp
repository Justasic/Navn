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
  process(buf); /* Process the buffer for navn */
  return true;
}

int main (int argcx, char** argvx, char *envp[])
{
  my_av = argvx;
  my_envp = envp;
  try
  {
    startup(argcx, argvx);
    int startcount = 0;
    SocketStart:
    ++startcount;
    try{
      //Make the socket used to connect to the server
      if(Config->Server.empty())
	throw CoreException("No Server Specified.");
      Log() << "Connecting to server '" << Config->Server << ":" << Config->Port << "'";
      FOREACH_MOD(I_OnPreConnect, OnPreConnect(Config->Server, Config->Port));
      sock = new SocketIO(Config->Server, Config->Port);
      sock->Connect();
    }catch(SocketException &e){
      if(startcount >= 3)
	throw CoreException(e.description().c_str());
      if(sock)
	delete sock;
      Log(LOG_DEBUG) << "Socket Exception Caught: " << e.description();
      goto SocketStart;
    }
    if(!sock)
      goto SocketStart;
    Send = new SendMessage();
    time_t last_check = time(NULL);
    
    //Set the username and nick
    Send->command->user(Config->Ident, Config->Realname);
    Send->command->nick(Config->BotNick);
    FOREACH_MOD(I_OnPostConnect, OnPostConnect(sock));
    
    
    while(!quitting){
      Log(LOG_RAWIO) << "Top of main loop";
      
      /* Process the socket engine */
      sock->Process();
      /* Process Timers */
      /***********************************/
      if(time(NULL) - last_check >= 3)
      {
	TimerManager::TickTimers(time(NULL));
	last_check = time(NULL);
      }
      /***********************************/
    }//while loop ends here
    FOREACH_MOD(I_OnShutdown, OnShutdown());
    ModuleHandler::UnloadAll();
    ModuleHandler::SanitizeRuntime();
    Log(LOG_TERMINAL) << "\033[22;37m";
  }//try ends here
  catch(const CoreException& e){
    /* we reset the terminal colors, this should be removed as it makes more issues than it is cool */
    Log(LOG_TERMINAL) << "\033[22;37m";
    Log() << "Core Exception Caught: " << e.GetReason();
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

