/* All code is licensed under GNU General Public License GPL v3 (http://www.gnu.org/licenses/gpl.html) */
#ifndef command_h
#define command_h
#include "includes.h"
#include "user.h"
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
  void kick(const Flux::string&, const Flux::string&, const char *fmt, ...);
  void topic(const Flux::string&, const char *fmt, ...);
  void part(const Flux::string&, const char *fmt, ...);
  void kick(const Flux::string&, const Flux::string&, const Flux::string&);
  void mode(const Flux::string&, const Flux::string&, const Flux::string&);
  void topic(const Flux::string&, const Flux::string&);
  void part(const Flux::string&, const Flux::string&);
  void oper(const Flux::string&, const Flux::string&);
  void mode(const Flux::string&, const Flux::string&);
  void user(const Flux::string&, const Flux::string&);
  void who(const Flux::string&);
  void quit(const Flux::string&);
  void nick(const Flux::string&);
  void part(const Flux::string&);
  void join(const Flux::string&);
  void whois(const Flux::string&);
  void names(const Flux::string&);
};
class Oper
{
public:
  SocketIO *s;
  Oper(SocketIO *sock);
  void raw(const char *fmt, ...);
  void samode(const Flux::string&, const Flux::string&);
  void samode(const Flux::string&, const Flux::string&, const Flux::string&);
  void sajoin(const Flux::string&, const Flux::string&);
  void sapart(const Flux::string&, const Flux::string&);
  void sanick(const Flux::string&, const Flux::string&);
  void sakick(const Flux::string&, const Flux::string&, const Flux::string&);
  void satopic(const Flux::string&, const Flux::string&);
  void satopic(const Flux::string&, const char *fmt, ...);
  void sahost(const Flux::string&, const Flux::string&);
  void saident(const Flux::string&, const Flux::string&);
  void kill(const Flux::string&, const Flux::string&);
  void saname(const Flux::string&, const Flux::string&);
  void saname(const Flux::string&, const char *fmt, ...);
  void wallops(const Flux::string&);
  void wallops(const char *fmt, ...);
  void globops(const char *fmt, ...);
  void globops(const Flux::string&);
  void zline(const Flux::string&, const Flux::string&, const Flux::string&);
  void qline(const Flux::string&, const Flux::string&, const Flux::string&);
  void kline(const Flux::string&, const Flux::string&, const Flux::string&);
  void gline(const Flux::string&, const Flux::string&, const Flux::string&);
};
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