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

Log::Log(LogType t) : type(t) {}
Log::Log(LogType t, User *user):type(t), u(user) { if(!u) throw CoreException("No user argument in Log()"); }
Log::Log(User *user):u(user) { this->type = LOG_NORMAL; if(!u) throw CoreException("No user argument in Log()"); }
Log::Log(User *user, Command *command):u(user), c(command) { 
  this->type = LOG_NORMAL; 
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
  
  //if(this->u)
   //message = this->u->nick + " " + message;
  //else if(this->u && this->c)
    //message = this->u->nick + " used " + this->c->name + " " + message;
  
  if((type == LOG_RAWIO || type == LOG_DEBUG) && protocoldebug)
    std::cout << TimeStamp() << " " << message << std::endl;
  else if(type == LOG_NORMAL && nofork)
    std::cout << TimeStamp() << " " << message << std::endl;
  else if(type == LOG_DEBUG && dev && nofork)
    std::cout << TimeStamp() << " " << message << std::endl;
  else if(type == LOG_TERMINAL){
    std::cout << this->buffer.str() << std::endl;
    return;
  }
  
  try
    {
      log.open(Config->LogFile.c_str(), std::fstream::out);
      if(!log.is_open())
	throw LogException(Config->LogFile.empty()?Flux::string("Cannot fild Log File.").c_str():
			    Flux::string("Failed to open Log File"+Config->LogFile+": "+strerror(errno)).c_str());
	
      log << TimeStamp() << " " << message << std::endl;
      if(log.is_open())
	log.close();
    }
    catch (LogException &e) { std::cerr << "Log Exception Caught: " << e.GetReason() << std::endl; }
}