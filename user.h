#ifndef USER_H
#define USER_H
#include "privmsg.h"
class User
{
public:
  User(const Flux::string&, const Flux::string&, const Flux::string&, const Flux::string &realname = "", const Flux::string &server ="");
  virtual ~User();
  Flux::string nick, host, realname, ident, fullhost, server;
  void kick(const Flux::string&, const Flux::string&);
  void kill(const Flux::string&);
  void SendWho();
  void SendMessage(const Flux::string&);
  void SendMessage(const char*, ...);
  void SendPrivmsg(const Flux::string&);
  void SendPrivmsg(const char*, ...);
};
extern Flux::map<User *> UserNickList; //this makes the variable global for later use..
class Channel;
typedef Flux::map<Channel*> channel_map;
User *finduser(const Flux::string &nick);
extern bool IsValidChannel(const Flux::string&);
class Channel
{
public:
  Channel(const Flux::string&, time_t ts = time(NULL));
  ~Channel();
  //some day we will hhave a mode manager here :P
  //some day we will also have a user finder for a channel ;P
  Flux::string name;
  Flux::string topic;
  Flux::string topic_setter;
  time_t topic_time;
  time_t creation_time;
  void SendJoin();
  void SendPart();
  void SendPart(const Flux::string&);
  void SendPart(const char*, ...);
  void SendWho();
  void kick(User*, const Flux::string&);
  void kick(User*, const char*, ...);
  void kick(const Flux::string&, const Flux::string&);
  void kick(const Flux::string&, const char*, ...);
  void SetMode(const Flux::string&);
  void SetMode(User *u, const Flux::string &mode);
  void RemoveMode(const Flux::string&);
  void RemoveMode(User *u, const Flux::string &mode);
  void ChangeTopic(const Flux::string&);
  void ChangeTopic(const char *fmt, ...);
  void SendMessage(const Flux::string&);
  void SendMessage(const char*, ...);
  void SendAction(const Flux::string&);
  void SendAction(const char*, ...);
  void SendNotice(const Flux::string&);
  void SendNotice(const char*, ...);
};
Channel *findchannel(const Flux::string&);
void ListChans(CommandSource &source);
struct CommandSource
{
 User *u; /* User name, this will be replaced with above class*/
 Channel *c; /* Channel name, this will be replaced with channel class */
 Flux::string command;
 Flux::string message;
 Flux::string raw;
 std::vector<Flux::string> params;
 
 void Reply(const char *fmt, ...);
 void Reply(const Flux::string&);
};
#endif