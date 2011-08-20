#include "includes.h"

Flux::string strip2(const Flux::string &buf){
	Flux::string newbuf = buf;
	char c = newbuf[newbuf.size() - 1];
	while (c == '\n' || c == '\r'){
		newbuf.erase(newbuf.end() - 1);
		c = newbuf[newbuf.size() - 1];
	}
	return newbuf;
}
template<typename T> inline Flux::string stringify(const T &x){
	std::ostringstream stream;
	if(!(stream << x))
		throw;
	return stream.str();
}
Flux::string isolate(char begin, char end, Flux::string msg){
  Flux::string to_find;
  size_t pos = msg.find(begin);
  pos += 1;
  for (unsigned i = pos; i < msg.length(); i++){
    if (msg.at(i) == end){
      break;
    }else{to_find = to_find+msg.at(i);}
  }
  return to_find;
}
/** 
 * \fn void log(const char *fmt, ...)
 * This is what logs everything that goes on with the bot
 * \param *fmt
 */
void log(const char *fmt, ...){
  std::fstream log;
  Flux::string logmsg;
  try{
  log.open("navn.log", std::fstream::in | std::fstream::out | std::fstream::app);
  if(!log.is_open())
     logmsg = "Failed to open log file: "+stringify(strerror(errno));
     throw LogException(logmsg);
  }catch (LogException &e){
    Flux::string reason = e.GetReason();
    reason.trim();
    if(reason.empty())
      return;
   std::cout << "Log Exception Caught: " << e.GetReason() << "\n";
  }
  va_list args;
  va_start(args, fmt);
  
  time_t t = time(NULL);
  struct tm *tm = localtime(&t);
  
  char buf[512];
  strftime(buf, sizeof(buf) - 1, "[%b %d %H:%M:%S %Y] ", tm);
  log << buf;
  vsnprintf(buf, sizeof(buf), fmt, args);
  log << strip2(buf) << "\n";
  va_end(args);
  va_end(args);
  log.close();
}
std::vector<Flux::string> StringVector(const Flux::string &src, char delim){
 sepstream tok(src, delim);
 Flux::string token;
 std::vector<Flux::string> ret;
 while(tok.GetToken(token))
   ret.push_back(token);
 return ret;
}