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
class PingTimer:public Timer
{
public:
  PingTimer():Timer(30, time(NULL), true) {}
  void Tick(time_t){
    Send->raw("PING :%i\n", time(NULL)); 
  }
};
class Ping_pong:public module{
  PingTimer pingtimer;
public:
  Ping_pong(bool a):module("Ping", a, PRIORITY_FIRST){ 
    this->SetDesc("Sends a ping to the server, keeping the bot connected");
    //Implementation i[] = { I_OnPrivmsg };
  }
  ModuleReturn run(CommandSource &source, std::vector<Flux::string> &params){
    if (source.command == "PING"){
      Send->raw("PONG :%i", time(NULL));
    } 
     /*for some Undernet connections */
    /*if(reply->said("NOTICE AUTH :*** Ident broken or disabled, to continue to connect you must type")){
      Send->s->send("PASS "+strip(reply->params(16))+nl);
    }*/
    for(unsigned i=0; params.size() > i; i++)
      printf("mparams %i: %s\n", i, params[i].c_str());
    if(source.command == "ERROR"){
      restart("Killed");
    }
    return MOD_RUN;
  }
};
/**
 * @}
 */
#endif
