#ifndef PRIVMSG_H
#define PRIVMSG_H
#include "includes.h"

class Command;
class SendMessage
{
public:
  SocketIO *s;
  Command *command;
  SendMessage(SocketIO *sock, Command *command);
  void privmsg(Flux::string where, const char *fmt, ...);
  void privmsg(Flux::string where, Flux::string msg);
  void notice(Flux::string where, const char *fmt, ...);
  void notice(Flux::string where, Flux::string msg);
  void action(Flux::string where, const char *fmt, ...);
  void action(Flux::string where, Flux::string msg);
  void raw(const char *fmt, ...);
};
  
#endif