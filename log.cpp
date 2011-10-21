#include "log.h"

Flux::string TimeStamp()
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
  Flux::string message = Flux::Sanitize(this->buffer.str());
  if(this->u && !this->c)
   message = this->u->nick + " " + message;
  if(this->u && this->c)
    message = this->u->nick + " used " + this->c->name + " " + message;
  
  if((type == LOG_RAWIO || type == LOG_DEBUG) && protocoldebug && InTerm())
    std::cout << TimeStamp() << " " << message << std::endl;
  else if(type == LOG_NORMAL && nofork && InTerm())
    std::cout << TimeStamp() << " " << message << std::endl;
  else if(type == LOG_DEBUG && dev && nofork && InTerm())
    std::cout << TimeStamp() << " " << message << std::endl;
  else if(type == LOG_TERMINAL && InTerm()){
    std::cout << this->buffer.str() << std::endl;
    return;
  }else if(type == LOG_SILENT){}
  
  try
    {
      log.open(Config->LogFile.c_str(), std::fstream::out | std::fstream::app);
      if(!log.is_open())
	throw LogException(Config->LogFile.empty()?Flux::string("Cannot fild Log File.").c_str():
			    Flux::string("Failed to open Log File"+Config->LogFile+": "+strerror(errno)).c_str());
	
      log << TimeStamp() << " " << message << std::endl;
      if(log.is_open())
	log.close();
    }
    catch (LogException &e) { if(!InTerm()) std::cerr << "Log Exception Caught: " << e.GetReason() << std::endl; }
}