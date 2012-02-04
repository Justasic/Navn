#ifndef PRIVMSG_H
#define PRIVMSG_H
#include "includes.h"

class Privmsg
{
public:
  void privmsg(Socket &sock, Flux::string send, const char *fmt, ...);
  void privmsg(Socket &sock, Flux::string send, Flux::string msg);
  void notice(Socket &sock, Flux::string send, const char *fmt, ...);
  void notice(Socket &sock, Flux::string send, Flux::string msg);
  void raw(Socket &sock, const char *fmt, ...);
};

class SendMessage
{
private:
    Socket s;
public:
  void SendMessage(Socket&);
  void privmsg(Flux::string where, const char *fmt, ...);
  void privmsg(Flux::string where, Flux::string msg);
  void raw(const char *fmt, ...);
};
  
#endif