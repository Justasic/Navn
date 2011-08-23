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
  std::fstream log;
  try{
  log.open(chanlogfile.c_str(), std::fstream::in | std::fstream::out | std::fstream::app);
  if(!log.is_open())
     throw LogException("Failed to open log file.");
  }catch (LogException &e){
   std::cerr << "Log Exception Caught: " << e.GetReason() << endl;
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
class Chanlog:public module{
public:
  Chanlog(bool a):module("Channel Logger", a, PRIORITY_LAST){ this->SetDesc("Logs what is said in the channel"); }
ModuleReturn run(CommandSource &source, std::vector<Flux::string> &params){
  Flux::string cmd = params.empty()?"":params[0], msg = source.message;
  User *u = source.u;
  if(source.command == "PRIVMSG" && source.c == LogChannel){
    Flux::string nolog = params.size() == 2?params[1]:"";
     if(nolog == "#nl"){return MOD_STOP;}else{
       if(nolog == "\001ACTION"){
         msg = msg.erase(0,7);
         msg.erase(msg.size()-1, 1);
         CLog("*** %s %s", u->nick.c_str(), Flux::Sanitize(msg).c_str());
       }else{
        CLog("<%s> %s", u->nick.c_str(), msg.c_str());
       }
     }
  }
   if(source.command == "NOTICE" && source.c == LogChannel){
     Flux::string nolog = params.size() == 2?params[1]:"";
     if(nolog =="#nl"){ return MOD_STOP; }else{
      CLog("-Notice- %s: %s", u->nick.c_str(), Flux::Sanitize(source.message).c_str());
     }
  }
   if(source.command == "PART" && source.c == LogChannel){
     CLog("*** %s has parted %s", u->nick.c_str(), LogChannel.c_str());
  }
   if(source.command == "JOIN" && source.c == LogChannel){
     CLog("*** %s has joined %s", u->nick.c_str(), LogChannel.c_str());
  }
   if(source.command == "QUIT" && source.c == LogChannel){
     CLog("*** %s has quit", u->nick.c_str(), LogChannel.c_str());
  }
   if(source.command == "MODE" && source.c == LogChannel){
     if(params.size() < 4)
       return MOD_STOP;
     CLog("*** %s sets mode %s %s", u->nick.c_str(), params[3].c_str(), params[4].c_str());
  }
  return MOD_RUN;
}
};
/**
 * @}
 */
#endif
