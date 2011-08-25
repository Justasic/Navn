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
bool timeout = false;
class PingTimer:public Timer
{
public:
  PingTimer():Timer(30, time(NULL), true) {}
  void Tick(time_t){
    Send->raw("PING :%i\n", time(NULL));
    if(!timeout)
      timeout = true;
    else
      restart("Ping-Timeout");
  }
};
class Ping_pong:public module{
  PingTimer pingtimer;
public:
  Ping_pong(bool a):module("Ping", a, PRIORITY_FIRST){ 
    this->SetDesc("Handles ping pong events");
    //Implementation i[] = { I_OnPrivmsg };
  }
  ModuleReturn run(CommandSource &source, std::vector<Flux::string> &params){
    if (source.command == "PING"){
      Send->raw("PONG :%i", time(NULL));
    } 
    if(source.command == "PONG"){
     Flux::string ts = params[0];
     int timestamp = atoi(ts.c_str());
     int lag = timestamp-time(NULL);
     timeout = false;
     printf("%i sec lag (%i - %i)\n", lag, timestamp, (int)time(NULL));
    }
     /*for some Undernet connections */
    /*if(reply->said("NOTICE AUTH :*** Ident broken or disabled, to continue to connect you must type")){
      Send->s->send("PASS "+strip(reply->params(16))+nl);
    }*/
    if(source.command == "ERROR"){
      restart(source.raw);
    }
    return MOD_RUN;
  }
};
/**
 * @}
 */
#endif
