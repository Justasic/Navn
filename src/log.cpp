/* Navn IRC bot -- Logging classes
 * 
 * (C) 2011-2012 Flux-Net
 * Contact us at Dev@Flux-Net.net
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Anope by The Anope Team.
 */

#include "log.h"

Flux::string NoTermColor(const Flux::string &ret){
  Flux::string str;
  bool in_term_color = false;
  for(unsigned i=0; i < ret.length(); ++i){
    char c = ret[i];
    if(in_term_color){
      if(c == 'm')
	in_term_color = false;
      continue;
    }
    if(c == '\033'){
      in_term_color = true;
      continue;
    }
    if(!in_term_color)
      str += c;
  }
  return str;
}

Flux::string Log::TimeStamp()
{
 char tbuf[256];
  time_t t;

  if (time(&t) < 0)
	  throw CoreException("time() failed");
  tm tm = *localtime(&t);
#if HAVE_GETTIMEOFDAY
  if (protocoldebug)
  {
    char *s;
    struct timeval tv;
    gettimeofday(&tv, NULL);
    strftime(tbuf, sizeof(tbuf) - 1, "[%b %d %H:%M:%S", &tm);
    s = tbuf + strlen(tbuf);
    s += snprintf(s, sizeof(tbuf) - (s - tbuf), ".%06d", static_cast<int>(tv.tv_usec));
    strftime(s, sizeof(tbuf) - (s - tbuf) - 1, " %Y]", &tm);
  }
  else
#endif
    strftime(tbuf, sizeof(tbuf) - 1, "[%b %d %H:%M:%S %Y]", &tm);
    
  return Flux::Sanitize(tbuf);
}

Log::Log(LogType t) : type(t), u(NULL), c(NULL) {}
Log::Log(LogType t, User *user):type(t), u(user), c(NULL) { if(!u) throw CoreException("No user argument in Log()"); }
Log::Log(User *user): type(LOG_NORMAL), u(user), c(NULL) { if(!u) throw CoreException("No user argument in Log()"); }
Log::Log(User *user, Command *command):type(LOG_NORMAL), u(user), c(command) { 
  if(!u) throw CoreException("No user argument in Log()");
  if(!c) throw CoreException("No command argument in Log()");
}

Log::Log(LogType t, User *user, Command *command): type(t), u(user), c(command) { 
  if(!u) throw CoreException("No user argument in Log()"); 
  if(!c) throw CoreException("No command argument in Log()");
}

Log::~Log()
{
  Flux::string message = Flux::Sanitize(this->buffer.str()), raw = this->buffer.str();
  if(this->u && !this->c)
   message = this->u->nick + " " + message;
  if(this->u && this->c)
    message = this->u->nick + " used " + this->c->name + " " + message;
  
  if((type == LOG_RAWIO || type == LOG_DEBUG) && protocoldebug && InTerm())
    std::cout << TimeStamp() << " " << (nocolor?NoTermColor(message):message) << std::endl;
  else if(type == LOG_NORMAL && nofork && InTerm())
    std::cout << TimeStamp() << " " << (nocolor?NoTermColor(message):message) << std::endl;
  else if(type == LOG_DEBUG && dev && nofork && InTerm())
    std::cout << TimeStamp() << " " << (nocolor?NoTermColor(message):message) << std::endl;
  else if(type == LOG_TERMINAL && InTerm()){
    std::cout << (nocolor?NoTermColor(raw):raw) << std::endl;
    return;
  }else if(type == LOG_SILENT){} // ignore the terminal if its log silent

  EventResult result;
  FOREACH_RESULT(I_OnLog, OnLog(this), result);
  if(result != EVENT_CONTINUE)
    return;

  try
    {
      log.open(Config->LogFile.c_str(), std::fstream::out | std::fstream::app);
      if(!log.is_open())
	throw LogException(Config->LogFile.empty()?Flux::string("Cannot fild Log File.").c_str():
			    Flux::string("Failed to open Log File"+Config->LogFile+": "+strerror(errno)).c_str());
	
      log << TimeStamp() << " " << NoTermColor(message) << std::endl;
      if(log.is_open())
	log.close();
    }
    catch (LogException &e) { if(!InTerm()) std::cerr << "Log Exception Caught: " << e.GetReason() << std::endl; }
}