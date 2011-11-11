/* All code is licensed under GNU General Public License GPL v3 (http://www.gnu.org/licenses/gpl.html) */
#ifndef IRCPROTO_H
#define IRCPROTO_H
#include "includes.h"
#include "command.h"

class IRCProto
{
public:
  Oper *o;
  IRCProto();
  void privmsg(const Flux::string &where, const char *fmt, ...);
  void privmsg(const Flux::string &where, const Flux::string &msg);
  void notice(const Flux::string &where, const char *fmt, ...);
  void notice(const Flux::string &where, const Flux::string &msg);
  void action(const Flux::string &where, const char *fmt, ...);
  void action(const Flux::string &where, const Flux::string &msg);
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
  void away(const Flux::string&);
};
#endif