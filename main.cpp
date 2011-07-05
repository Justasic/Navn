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
//#include "modules/module.h"
/**
 * \page tutmod Adding Your Module - Step 1: Including your module.
 * \section tut1 Step 1: Including your module.
 * \subsection tut1a Include your module in main.cpp
 * Include your module in main.cpp using the #inlclude keyword
 * Example:
 *\code
 */

#include "modules/da_goat.h"
#include "modules/dns.h"
#include "modules/ctcp.h"
#include "modules/dummy.h"
#include "modules/ping_pong.h"
#include "modules/searcher.h"
#include "modules/help.h"
#include "modules/about_me.h"
#include "modules/world_clock.h"
#include "modules/system.h"
#include "modules/channel_logger.h"
#include "modules/weather.h"
#include "modules/tinyurl.h"
#include "modules/navn.h"
//#include "modules/Flux_Services.h"

/**http://www.google.com/search?aq=0&oq=c%2B%2B+object+ve&sourceid=chrome&ie=UTF-8&q=c%2B%2B+object+vector
 *\endcode 
 */

string binary_path, services_dir, services_bin;
using namespace std;
using namespace flux_net_irc;

int main (int argcx, char** argvx, char *envp[])
{
  my_av = argvx;
  my_envp = envp;
  try{
  INIReader config("bot.conf");
    if (config.ParseError() < 0) {
       throw ConfigException("Cannot load bot.conf");
    }
  ReadConfig(config);
  }catch(ConfigException &ex){
    cout << "\r\nConfig Exception was caught: \033[22;31m" << ex.GetReason() << "\033[22;37m" << nl;
    log("Config Exception Caught: ", stringify(ex.GetReason()).c_str());
    DoQuit(1);
  }
  startup(argcx, argvx);
  try
  {
    cout << "\033[22;31mStarted with PID \033[22;32m" << getpid() << "\033[22;37m" << nl;
    //Make the socket used to connect to the server
    Socket sock(server,port);
    //Incase there is no connection, say so
    if (!sock.get_address()) throw SocketException("Could not resolve the IRC server.");
    if (!sock.connect()) throw SocketException("Could not connect to the IRC server.");
    //Accept some server replies after connecting
    sock >> rply;
    irc_string* reply = new irc_string(rply);
    //Set the username and nick
    sock << "USER "+usrname+" * * :"+realname+nl;
    sock << setnick(nick);
    
    ping_pong(sock, reply, rply);
    
    sock >> rply;
    string line;
    
    signal(SIGTERM, sigact); //catch any terminal signals if possible
    signal(SIGINT, sigact);
    signal(SIGHUP, sigact);
    
    /*
    struct sigaction act; // to catch a signal sent from the terminal.
    act.sa_handler = sigact;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGINT, &act, 0);
    */


    dummy _dummy("Dummy Module",true);
    
    
    while (!quitting){ //infi loop to stay connected
      if(quitting)
	 shutdown(sock, quitmsg);
      sock >> rply; //keep the connection messages in the rply string.
      irc_string* reply = new irc_string(rply);

      host = reply->host;
      fullhost = reply->usernick+"!"+reply->user+"@"+host;
      chan = reply->channel; //sets the variables.
      unick = reply->usernick;
      msg = reply->message;
      ident = reply->user;
      raw = reply->raw_string;
      
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

      /**       MODULES          */
      ping_pong(sock, reply, rply);
      searcher(sock, reply, rply);
      system_m(sock, reply, rply);
      help_m(sock, reply);
      //dummy(sock, reply);
      Da_Goat(sock, reply);
      about_me(sock, reply);
      dns_m(sock, reply, rply);
      world_clock(sock, reply, rply);
      ctcp_m(sock, reply);
      Chanlog(reply);
      weather(sock, rply);
      TinyURL(sock, reply);
      navn_h(sock, reply);
      
      /*! \endcode */

      for(int i = 0; i < moduleList.size(); i++){
	moduleList[i]->run(sock, rply, reply);
      }  
      //Flux_Services(sock, reply);
      /***********************************/
      delete reply;
    }//while loop ends here
    shutdown(sock, quitmsg);
  }//try ends here
  catch (SocketException& e) //catch any Exceptions sent.
  {
    cout << "\r\nSocket Exception was caught: \033[22;31m" << e.description() << "\033[22;37m" << nl;
    log("Socket Exception Caught: %s", e.description().c_str());
    DoQuit(1);
  }
  catch(CoreException& e){
    cout << "\r\nCore Exception was caught: \033[22;31m" << e.GetReason() << "\033[22;37m" << nl;
    log("Core Exception Caught: ", stringify(e.GetReason()).c_str());
    DoQuit(1);
  }
  return EXIT_SUCCESS;
}
