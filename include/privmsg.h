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
  void privmsg(Flux::string where, const char *fmt, ...);
  void privmsg(Flux::string where, Flux::string msg);
  void notice(Flux::string where, const char *fmt, ...);
  void notice(Flux::string where, Flux::string msg);
  void action(Flux::string where, const char *fmt, ...);
  void action(Flux::string where, Flux::string msg);
};
#endif