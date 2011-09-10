/* All code is licensed under GNU General Public License GPL v3 (http://www.gnu.org/licenses/gpl.html) */
#ifndef PING_PONG_H
#define PING_PONG_H

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
int pings = 0;
bool timeout = false;
class PingTimer:public Timer
{
public:
  PingTimer():Timer(30, time(NULL), true) {}
  void Tick(time_t){
    Send->raw("PING :%i\n", time(NULL));
    if(!timeout)
      timeout = true;
    else if(timeout)
      ++pings;
    else if(pings > 3 && timeout)
      restart("Ping-Timeout");
  }
};
class Ping_pong:public module{
  PingTimer pingtimer;
public:
  Ping_pong(bool a):module("Ping", a, PRIORITY_FIRST){ 
    this->SetDesc("Handles ping pong events");
    /*Implementation i[] = { I_OnPrivmsg };
    ModuleHandler::Attach(i, this);*/
  }
  ModuleReturn run(CommandSource &source, std::vector<Flux::string> &params){
    if (source.command == "PING"){
      Send->raw("PONG :%s", source.params[0].c_str());
    } 
    if(source.command == "PONG"){
     Flux::string ts = params[0];
     int timestamp = atoi(ts.c_str());
     int lag = timestamp-time(NULL);
     timeout = false;
     pings = 0;
     if(protocoldebug)
        printf("%i sec lag (%i - %i)\n", lag, timestamp, (int)time(NULL));
    }
     /*for some Undernet connections */
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
