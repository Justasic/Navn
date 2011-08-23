#ifndef command_h
#define command_h
#include "includes.h"
#include "user.h"
class module;
/**
 *\file  command.h
 *\brief Contains the command class header.
 */
class Commands
{
public:
  SocketIO *s;
  Commands(SocketIO *sock);
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
extern Flux::string nick;
class Command;
typedef std::map<Flux::string, Command *, std::less<Flux::string> > CommandMap;
class Command
{
  Flux::string desc;
  std::vector<Flux::string> syntax;
public:
  size_t MaxParams;
  size_t MinParams;
  Flux::string name;
  module *mod;
  Command(const Flux::string &sname, size_t min_params, size_t max_params=0);
  virtual ~Command();
protected:
  void SetDesc(const Flux::string&);
  void SetSyntax(const Flux::string&);
  void SendSyntax(CommandSource&);
  void SendSyntax(CommandSource&, const Flux::string&);
public:
  const Flux::string &GetDesc() const;
  virtual void Execute(CommandSource&, const std::vector<Flux::string> &params)=0;
  virtual bool OnHelp(CommandSource&, const Flux::string&);
  virtual void OnSyntaxError(CommandSource&, const Flux::string&);
};
Command *FindCommand(const Flux::string &name);
#endif