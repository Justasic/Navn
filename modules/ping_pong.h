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

void ping_pong(SocketIO *sock, irc_string *reply, Flux::string re){
  
  if (reply->said("PING :")){
    sock->send("PONG "+strip(re.substr(6,-1))+nl);
  }
  /* Fix for some Undernet connections */
  if(reply->said("NOTICE AUTH :*** Ident broken or disabled, to continue to connect you must type")){
   sock->send("PASS "+strip(reply->params(17))+nl);
  }
  if(reply->said("ERROR :Closing link:")){
   throw SocketException(raw);
  }
}
/**
 * @}
 */
#endif
