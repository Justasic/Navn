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
  void quit(const char *fmt, ...);
  void kick(Flux::string, Flux::string, const char *fmt, ...);
  void topic(Flux::string, const char *fmt, ...);
  void part(Flux::string, const char *fmt, ...);
  void kick(Flux::string, Flux::string, Flux::string);
  void mode(Flux::string, Flux::string, Flux::string);
  void topic(Flux::string, Flux::string);
  void part(Flux::string, Flux::string);
  void oper(Flux::string, Flux::string);
  void mode(Flux::string, Flux::string);
  void user(Flux::string, Flux::string);
  void quit(Flux::string);
  void nick(Flux::string);
  void part(Flux::string);
  void join(Flux::string);
  void whois(Flux::string);
  
};
class Oper
{
public:
  SocketIO *s;
  Oper(SocketIO *sock);
  void raw(const char *fmt, ...);
  void samode(Flux::string, Flux::string);
  void samode(Flux::string, Flux::string, Flux::string);
  void sajoin(Flux::string, Flux::string);
  void sapart(Flux::string, Flux::string);
  void sanick(Flux::string, Flux::string);
  void sakick(Flux::string, Flux::string, Flux::string);
  void satopic(Flux::string, Flux::string);
  void satopic(Flux::string, const char *fmt, ...);
  void sahost(Flux::string, Flux::string);
  void saident(Flux::string, Flux::string);
  void kill(Flux::string, Flux::string);
  void saname(Flux::string, Flux::string);
  void saname(Flux::string, const char *fmt, ...);
  void wallops(Flux::string);
  void wallops(const char *fmt, ...);
  void globops(const char *fmt, ...);
  void globops(Flux::string);
  void zline(Flux::string, Flux::string, Flux::string);
  void qline(Flux::string, Flux::string, Flux::string);
  void kline(Flux::string, Flux::string, Flux::string);
  void gline(Flux::string, Flux::string, Flux::string);
};
#endif