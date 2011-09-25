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
      log(LOG_NORMAL, "Connecting to server '%s:%s'", Config->Server.c_str(), Config->Port.c_str());
      sock = new SocketIO(Config->Server, Config->Port);
      sock->Connect();
    }catch(SocketException &e){
      if(startcount >= 3)
	throw CoreException(e.description().c_str());
      if(sock)
	delete sock;
      log(LOG_DEBUG, "Socket Exception Caught: %s", e.description().c_str());
      goto SocketStart;
    }
    if(!sock)
      goto SocketStart;
    Send = new SendMessage();
    time_t last_check = time(NULL);
    
    //Set the username and nick
    Send->command->user(Config->Ident, Config->Realname);
    Send->command->nick(Config->BotNick);
    
    while(!quitting){
      log(LOG_RAWIO, "Top of main loop");
      
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
    log(LOG_TERMINAL, "\033[22;37m");
  }//try ends here
  catch(const CoreException& e){
    if(!Config) //This is so we dont throw logging exceptions..
        log(LOG_TERMINAL, "Core Exception Caught: %s", Flux::stringify(e.GetReason()).c_str());
    else
      log(LOG_NORMAL, "Core Exception Caught: %s", Flux::stringify(e.GetReason()).c_str());
    log(LOG_TERMINAL, "\033[22;37m\n"); /* we reset the terminal colors, this should be removed as it makes more issues than it is cool */
    return EXIT_FAILURE;
  }
  printf("Exiting\n");
  return EXIT_SUCCESS;
}

