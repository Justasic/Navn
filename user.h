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
#endif