#ifndef CLOG_H
#define CLOG_H
#include "../includes.h"
#include "../flux_net_irc.hpp"

/**
 * \file channel_logger.h Header file holding the \a ChanLog function.
 * \author Justasic.
 */

/**
 * \defgroup chanlogM Channel Log Module
 * This is the module for the Channel Logger function. \n
 * The channel logger function logs what was said in the channel
 * to a file specified in the config file for navn \n
 * NOTE: Channel name is caps sensitive \n
 * For a better description see the function description.
 * \section commands Commands associated with this module.
 * \subsection none none
 * This module has no user interactive function
 * @{
 */

/**
 * \fn class Chanlog(bool a):module("Channel Logger", a, PRIORITY_LAST){ this->SetDesc("Logs what is said in the channel"); }
 * \fn void CLog(const char *fmt, ...)
 * \brief Logs channel messages
 * Logs what is said in the channel to a specified file.
 */

void CLog(const char *fmt, ...){
  const Flux::string chanlogfile = LogChannel+".log";
  fstream log;
  try{
  log.open(chanlogfile.c_str(), fstream::in | fstream::out | fstream::app);
  if(!log.is_open())
     throw LogException("Failed to open log file.");
  }catch (LogException &e){
   cerr << "Log Exception Caught: " << e.GetReason() << nl;
  }
  va_list args;
  va_start(args, fmt);
  
  time_t t = time(NULL);
  struct tm *tm = localtime(&t);
  
  char buf[512];
  strftime(buf, sizeof(buf) - 1, "[%b %d %H:%M:%S %Y] ", tm);
  log << buf;
  vsnprintf(buf, sizeof(buf), fmt, args);
  log << buf << endl;
  va_end(args);
  va_end(args);
  log.close();
}
class Chanlog:module{
public:
  Chanlog(bool a):module("Channel Logger", a, PRIORITY_LAST){ this->SetDesc("Logs what is said in the channel"); }
ModuleReturn run(SendMessage *Send, Flux::string rply, irc_string *reply){
  if(reply->said("PRIVMSG "+LogChannel+" ")){
     if(reply->said("#nl")){return MOD_STOP;}else{
       if(reply->said("\001ACTION")){
         msg = msg.erase(0,7);
         msg.erase(msg.size()-1, 1);
         CLog("*** %s %s", unick.c_str(), Flux::Sanitize(msg).c_str());
       }else{
        CLog("<%s> %s", unick.c_str(), msg.c_str());
       }
     }
  }
   if(reply->said("NOTICE "+LogChannel+" ")){
     if(reply->said("#nl")){}else{
      CLog("-Notice- %s: %s", unick.c_str(), Flux::Sanitize(reply->message).c_str());
     }
  }
   if(reply->said("PART "+LogChannel)){
     CLog("*** %s has parted %s", unick.c_str(), LogChannel.c_str());
  }
   if(reply->said("JOIN :"+LogChannel)){
     CLog("*** %s has joined %s", unick.c_str(), LogChannel.c_str());
  }
   if(reply->said("QUIT :")){
     CLog("*** %s has quit", unick.c_str(), LogChannel.c_str());
  }
   if(reply->said("MODE "+LogChannel)){
     CLog("*** %s sets mode %s %s", unick.c_str(), reply->params(3).c_str(), reply->params(4).c_str());
  }
  return MOD_RUN;
}
};
/**
 * @}
 */
#endif
