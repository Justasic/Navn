/**
 *\file  main.cpp 
 *\brief Contains the main() function.
 */
#include "flux_net_irc.hpp"

/*Include your modules here*/
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
//#include "modules/Flux_Services.h"
/***************************/

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

      /*************MODULES**************/
      ping_pong(sock, reply, rply);
      searcher(sock, reply, rply);
      system_m(sock, reply, rply);
      help_m(sock, reply);
      dummy(sock, reply);
      Da_Goat(sock, reply);
      about_me(sock, reply);
      dns_m(sock, reply, rply);
      world_clock(sock, reply);
      ctcp_m(sock, reply);
      Chanlog(reply);
      weather(sock, rply);
      TinyURL(sock, reply);
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
