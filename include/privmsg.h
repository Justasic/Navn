/* All code is licensed under GNU General Public License GPL v3 (http://www.gnu.org/licenses/gpl.html) */
#ifndef PRIVMSG_H
#define PRIVMSG_H
#include "includes.h"
#include "command.h"
class SendMessage
{
public:
  Commands *command;
  Oper *o;
  SendMessage();
  void privmsg(const Flux::string &where, const char *fmt, ...);
  void privmsg(const Flux::string &where, const Flux::string &msg);
  void notice(const Flux::string &where, const char *fmt, ...);
  void notice(const Flux::string &where, const Flux::string &msg);
  void action(const Flux::string &where, const char *fmt, ...);
  void action(const Flux::string &where, const Flux::string &msg);
};
#endif