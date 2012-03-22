/* Navn IRC bot -- Channel Logging module
 * 
 * (C) 2011-2012 Azuru
 * Contact us at Development@Azuru.net
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Anope by The Anope Team.
 */
#include "flux_net_irc.hpp"

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

void CLog(const char *fmt, ...)
{
  std::fstream clog;
  try{
    clog.open(Flux::string(Config->LogChannel+".log").c_str(), std::fstream::in | std::fstream::out | std::fstream::app);
    if(!clog.is_open())
      throw LogException("Failed to open log file.");
    
  }catch (LogException &e)
  {
    Log() << "Channel Logger Exception: " << e.GetReason();
  }
  
  va_list args;
  va_start(args, fmt);
  
  time_t t = time(NULL);
  struct tm *tm = localtime(&t);
  
  char buf[512];
  strftime(buf, sizeof(buf) - 1, "[%b %d %H:%M:%S %Y] ", tm);
  clog << buf;
  vsnprintf(buf, sizeof(buf), fmt, args);
  clog << buf << std::endl;
  va_end(args);
  va_end(args);
  clog.close();
}

class Chanlog : public module
{
public:
  Chanlog(const Flux::string &Name):module(Name)
  { 
    this->SetAuthor("Justasic");
    this->SetVersion(VERSION);
    this->SetPriority(PRIORITY_LAST);
    Implementation i[] = { I_OnPart, I_OnPrivmsgChannel, I_OnNoticeChannel, I_OnJoin, I_OnQuit, I_OnJoin, I_OnChannelMode, I_OnChannelOp };
    ModuleHandler::Attach(i, this, sizeof(i)/sizeof(Implementation));
  }
  
  void OnPrivmsgChannel(User *u, Channel *c, const Flux::vector &params)
  {
    Flux::string nolog = params.size() == 2?params[1]:"";
    if(c && c->name != Config->LogChannel)
      return;
    
    Flux::string msg;
    for(unsigned i=0; i < params.size(); ++i)
      msg += params[i]+' ';
    
    msg.trim();
    if(nolog != "#nl" && u)
    {
      if(nolog == "\001ACTION")
      {
	msg = msg.erase(0,8);
	CLog("*** %s %s", u->nick.c_str(), Flux::Sanitize(msg).c_str());
      }else
	CLog("<%s> %s", u->nick.c_str(), msg.c_str());
    }
  }
  
  void OnNoticeChannel(User *u, Channel *c, const Flux::vector &params)
  {
    if(c && c->name != Config->LogChannel)
      return;
    
    Flux::string msg;
    for(unsigned i=0; i < params.size(); ++i)
      msg += params[i]+' ';
      
    msg.trim();
    Flux::string nolog = params.size() == 2?params[1]:"";
    if(nolog != "#nl" && u)
      CLog("-Notice- %s: %s", u->nick.c_str(), Flux::Sanitize(msg).c_str());
  }
  
  void OnPart(User *u, Channel *c, const Flux::string &reason)
  {
     if(c && c->name != Config->LogChannel)
      return;
     
     CLog("*** %s has parted %s (%s)", u->nick.c_str(), c->name.c_str(), reason.c_str());
  }
  
  void OnJoin(User *u, Channel *c)
  {
    if(c && c->name != Config->LogChannel)
      return;
    
    CLog("*** %s has joined %s", u->nick.c_str(), c->name.c_str());
  }
  
  void OnQuit(User *u, const Flux::string &reason)
  {
     CLog("*** %s has quit (%s)", u->nick.c_str(), reason.c_str());
  }
  
  void OnChannelMode(User *u, Channel *c, const Flux::string &mode)
  {
      if(c && c->name != Config->LogChannel)
	return;
      
      CLog("*** %s sets mode %s %s", u->nick.c_str(), c->name.c_str(), mode.c_str());
  }
  
  void OnChannelOp(User *u, Channel *c, const Flux::string &mode, const Flux::string &nick)
  {
    if(c && c->name != Config->LogChannel)
	return;
    
    CLog("*** %s sets mode %s %s %s", u->nick.c_str(), c->name.c_str(), mode.c_str(), nick.c_str());
  }
};
MODULE_HOOK(Chanlog)
/**
 * @}
 */
