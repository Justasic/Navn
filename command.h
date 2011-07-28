#ifndef command_h
#define command_h
#include "includes.h"
/**
 *\file  command.h
 *\brief Contains the command class header.
 */
class Command
{
public:
  SocketIO *s;
  Command(SocketIO *sock);
  void raw(const char *fmt, ...);
  void kick(Flux::string, Flux::string, const char *fmt, ...);
  void topic(Flux::string, const char *fmt, ...);
  void part(Flux::string, const char *fmt, ...);
  void kick(Flux::string, Flux::string, Flux::string);
  void mode(Flux::string, Flux::string, Flux::string);
  void topic(Flux::string, Flux::string);
  void part(Flux::string, Flux::string);
  void oper(Flux::string, Flux::string);
  void mode(Flux::string, Flux::string);
  void nick(Flux::string);
  void part(Flux::string);
  void join(Flux::string);
  void whois(Flux::string);
  
};

#endif