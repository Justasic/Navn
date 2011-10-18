/* All code is licensed under GNU General Public License GPL v3 (http://www.gnu.org/licenses/gpl.html) */
#ifndef USER_H
#define USER_H
#include "privmsg.h"
#include "channel.h"
class User
{
public:
  User(const Flux::string&, const Flux::string&, const Flux::string&, const Flux::string &realname = "", const Flux::string &server ="");
  virtual ~User();
  std::list<Channel*> ChannelList;
  Flux::string nick
  /*!< \var Flux::string nick
  \brief The IRC nickname of the User. */ ,
  host /*!< \var Flux::string host
  \brief The IRC host of the User. */,
  realname /*!< \var Flux::string realname
  \brief The IRC Real Name of the User. */,
  ident /*!< \var Flux::string ident
  \brief The IRC Identification line of the User. */,
  fullhost /*!< \var Flux::string fullhost
  \brief The full IRC host line of the User. */,
  server /*!< \var Flux::string server
  \brief The IRC server of the User. */;
  void kick(const Flux::string&, const Flux::string&);
  void kick(Channel*, const Flux::string&);
  void kill(const Flux::string&);
  bool IsOwner();
  void AddChan(Channel*);
  void DelChan(Channel*);
  Channel *FindChan(Channel*);
  void SetNewNick(const Flux::string&);
  void SendWho();
  void SendMessage(const Flux::string&);
  void SendMessage(const char*, ...);
  void SendPrivmsg(const Flux::string&);
  void SendPrivmsg(const char*, ...);
};
/*********************************************************************************/
struct CommandSource
{
 User *u; /*!< \var User *u
  \brief The User who sent the Command. */
 Channel *c; /* Channel name, this will be replaced with channel class */
 Flux::string command;
 Flux::string message;
 Flux::string raw;
 Flux::string raw_source;
 std::vector<Flux::string> params;

 void Reply(const char *fmt, ...);
 void Reply(const Flux::string&);
};
#endif