#ifndef USER_H
#define USER_H
#include "privmsg.h"
class User
{
public:
  User(const Flux::string&, const Flux::string&, const Flux::string&, const Flux::string &realname = "");
  virtual ~User();
  Flux::string nick, host, realname, ident;
  void Kick(const Flux::string&, const Flux::string&);
  void Kill(const Flux::string&);
  void SendMessage(const Flux::string&);
  void Privmsg(const Flux::string&);
};
extern Flux::map<User *> UserList;
User *finduser(const Flux::string &nick);
struct CommandSource
{
 Flux::string u; /* User name, this will be replaced with above class*/
 Flux::string c; /* Channel name, this will be replaced with channel class */
 Flux::string command;
 Flux::string message;
 std::vector<Flux::string> params;
 
 void Reply(const char *fmt, ...);
 void Reply(const Flux::string&);
};
#endif