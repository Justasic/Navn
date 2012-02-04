#ifndef PING_PONG_H
#define PING_PONG_H
#include "../includes.h"
#include "../flux_net_irc.hpp"
/**
 * \file ping_pong.h Header file holding the \a Ping-Pong function.
 * \author Lordofsraam. Polished by Justasic.
 */

/**
 * \defgroup Ping_PongM Ping Pong Module
 * This is the module for the world clock function.
 * This module is pretty basic, it replies to the Ping request from the server
 * and will soon have a timed ping function of its own
 * For a better description see the function description.
 * \section commands Commands associated with this module.
 * \subsection none None
 * This module has no user interactive commands.
 * @{
 */

/**
 * \fn void ping_pong(Socket &sock, irc_string *reply, Flux::string re)
 * \brief Replies to IRC PING
 * Replies to the server's PING request to keep the bot connected
 */
using namespace std;
using namespace flux_net_irc;
#define pingwait 30
time_t endwait = time(NULL) + pingwait;
//bool ppto = false;
Flux::string servername;
//pthread thread1;
void ping_pong(Socket &sock, irc_string *reply, Flux::string re){
  
  unsigned long seconds = 30;
  timer t;
  t.start();
  
  if (reply->said("PING :")){
    sock << "PONG "+strip(re.substr(6,-1));
    servername = re.substr(6,-1);
  }
  if(reply->said("ERROR :Closing link:")){
   throw SocketException(raw);
  }
    if(t.elapsedTime() >= seconds){
      time_t nerp = time(NULL);
      sock << "PING " << stringify(nerp) << nl; 
     }else if(quitting){
       shutdown(sock, "quittin.");
     }
  
  // This is still a very experemental Ping timer to give ping timeouts when something happens
  /*
  if (time(NULL) > endwait){
   Flux::string pingtimestamp = stringify(time(NULL));
   sock << strip("PING "+ strip(servername));
   endwait = time(NULL) + pingwait;
   if(reply->said(" PONG ")){
    //ping-timeout function still needs to be programmed..
    cout << "woop woop we're still connected :P" << endl;
   }
  }
  if(reply->said("!endwait")){
	cout << "Endwait: " << endwait << endl;
	cout << "Time(): " << time(NULL) << endl;
	cout << "Remaining: " << endwait - time(NULL) << endl;
  }
*/
}
/**
 * @}
 */
#endif
