/* All code is licensed under GNU General Public License GPL v3 (http://www.gnu.org/licenses/gpl.html) */
#ifndef CHANNEL_H
#define CHANNEL_H
#include "privmsg.h"
class Channel
{
public:
  Channel(const Flux::string&, time_t ts = time(NULL));
  ~Channel();
  Flux::map<User*> UserList;
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
#endif
