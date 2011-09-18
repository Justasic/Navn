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
#include <flux_net_irc.hpp>

/**
 * \page tutmod Adding Your Module - Step 1: Including your module.
 * \section tut1 Step 1: Including your module.
 * \subsection tut1a Include your module in main.cpp
 * Include your module in main.cpp using the #inlclude keyword
 * Example:
 *\code
 */
#include "modules/ping_pong.h"
#include "modules/modulehandler.h"
#include "modules/system.h"
#include "modules/navn.h"
#include "modules/weather.h"
#include "modules/world_clock.h"
#include "modules/searcher.h"
#include "modules/help.h"
#include "modules/dns.h"
#include "modules/dummy.h"
#include "modules/ctcp.h"
#include "modules/channel_logger.h"
#include "modules/about_me.h"
#include "modules/da_goat.h"
#include "modules/Flux_Services.h"
#include "modules/encyclopedia.h"
/*====
#include "modules/tinyurl.h"

*/
/**
 *\endcode 
 */
int main (int argcx, char** argvx, char *envp[])
{
  my_av = argvx;
  my_envp = envp;
  try
  {
    startup(argcx, argvx);
    SocketStart:
    try{
      log(LOG_NORMAL, "Started with PID %i", getpid());
      //Make the socket used to connect to the server
      if(server.empty())
	throw CoreException("No Server Specified.");
      sock = new SocketIO(server, port);
	  //Incase there is no connection
      if(!sock->get_address())
	throw SocketException("Could not resolve server");
      if(!sock->connect())
	throw SocketException("Could not create a socket to connect to the IRC server");
      Send = new SendMessage();
    }catch(SocketException &e){
      log(LOG_DEBUG, "Socket Exception Caught: %s", e.description().c_str());
      log(LOG_TERMINAL, "\033[22;37m");
      goto SocketStart;
    }
    if(!sock)
      goto SocketStart;
    time_t last_check = time(NULL);
    
    //Set the username and nick
    Send->command->user(usrname, realname);
    Send->command->nick(nick);

    /**
      * \page tutmod2 Adding Your Module - Step 2: Running your module.
      * \section tut2 Step 2: Running your module.
      * \subsection tut2a Run your main module function in main.cpp
      * Run the main function of your module in main.cpp, inside the while loop. To be able
      * to send things out of the socket, and read things from the socket, your module's
      *  main function should accept \a sock and either \a irc_string reply or the raw string \a rply.
      * Example:
      * \code 
      */
    Ping_pong _Ping(true);
    modulehandler _modulehandler(true);
    system_m _system_m(true);
    navn _navn(true);
    weather _weather(true);
    world_clock _world_clock(true);
    searcher _searcher(true);
    help_m _help_m(true);
    dns_m _dns_m(true);
    dummy _dummy(true);
    ctcp _ctcp(true);
    Chanlog _Chanlog(true);
    about_me _about_me(true);
    Da_Goat _Da_Goat(true);
    Flux_Services _Flux_Services(false);
    encyclopedia _encyclopedia(true);
    /*
    TinyURL _TinyURL(false);
    */
    
    /**       MODULES          */
    /*! \endcode */ 
    Flux::string rply;
    while(!quitting){
      if(protocoldebug)
        log(LOG_RAWIO, "Top of main loop");
      
      /* Process the buffer and modules */
      std::queue<Flux::string> queue = sock->GetBuffer();
      while(!queue.empty()){
	if(queue.empty())
	 break;
	process(queue.front());
	queue.pop();
	sock->popqueue();
      }
      /***********************************/
      if(time(NULL) - last_check >= 3){
	TimerManager::TickTimers(time(NULL));
	last_check = time(NULL);
      }
      /***********************************/
    }//while loop ends here
    delete Send;
    delete sock;
    log(LOG_TERMINAL, "\033[22;37m");
  }//try ends here
  catch(const CoreException& e){
    if(config){
      log(LOG_NORMAL, "Core Exception Caught: %s", Flux::stringify(e.GetReason()).c_str());
    }
    else
      printf("Core Exception Caught: %s", Flux::stringify(e.GetReason()).c_str());
    printf("\033[22;37m\n"); /* we reset the terminal colors, this should be removed as it makes more issues than it is cool */
    exit(1);
  }
  return EXIT_SUCCESS;
}

