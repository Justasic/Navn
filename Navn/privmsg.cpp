#include "includes.h"
void Privmsg::privmsg(Socket &sock, Flux::string send, const char *fmt, ...)
{
  va_list args;
  char buffer[4096] = "";
  if(fmt)
  {
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    this->privmsg(sock, send, Flux::string(buffer));
    va_end(args);
  }
}
void Privmsg::privmsg(Socket &s, Flux::string send, Flux::string msg)
{
 sepstream sep(msg, '\n');
 Flux::string token;
 while(sep.GetToken(token))
 {
   this->raw(s, "PRIVMSG %s :%s", send.c_str(), msg.c_str()); 
 }
}
void Privmsg::notice(Socket &sock, Flux::string send, const char *fmt, ...)
{
  va_list args;
  char buffer[4096] = "";
  if(fmt)
  {
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    this->notice(sock, send, Flux::string(buffer));
    va_end(args);
  }
}
void Privmsg::notice(Socket &s, Flux::string send, Flux::string msg)
{
 sepstream sep(msg, '\n');
 Flux::string tok;
 while(sep.GetToken(tok))
 {
   this->raw(s, "NOTICE %s :%s", send.c_str(), msg.c_str());
 }
}
void Privmsg::raw(Socket &s, const char *fmt, ...){
  char buffer[4096] = "";
  va_list args;
  va_start(args, fmt);
  vsnprintf(buffer, sizeof(buffer), fmt, args);
  s.Send(buffer);
  va_end(args);
}

void SendMessage::SendMessage(Socket& sock){
  this->s = sock;
}
void SendMessage::raw(const char *fmt, ...){
  char buffer[4096] = "";
  va_list args;
  va_start(args, fmt);
  vsnprintf(buffer, sizeof(buffer), fmt, args);
  this->s.Send(buffer);
  va_end(args);
}
void SendMessage::privmsg(Flux::string where, const char *fmt, ...){
  va_list args;
  char buffer[4096] = "";
  if(fmt)
  {
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    this->privmsg(where, Flux::string(buffer));
    va_end(args);
  }
}
void SendMessage::privmsg(Flux::string where, Flux::string msg){
   sepstream sep(msg, '\n');
 Flux::string tok;
 while(sep.GetToken(tok))
 {
   this->raw("NOTICE %s :%s", where.c_str(), msg.c_str());
 }
}