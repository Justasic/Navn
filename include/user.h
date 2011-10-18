/* All code is licensed under GNU General Public License GPL v3 (http://www.gnu.org/licenses/gpl.html) */
#ifndef USER_H
#define USER_H
#include "privmsg.h"
#include "channel.h"
/**
 * \class User "user.h" USER_H
 * \brief A Class for IRC Users
 * This class wraps around IRC Users and includes several useful commands as well as User information.
 */
class User
{
public:
  User(const Flux::string&, const Flux::string&, const Flux::string&, const Flux::string &realname = "", const Flux::string &server ="");
  virtual ~User();
  std::list<Channel*> ChannelList;
  Flux::string nick
  /*!< \property Flux::string nick
  \brief The IRC nickname of the User. */ ,
  host /*!< \property Flux::string host
  \brief The IRC host of the User. */,
  realname /*!< \property Flux::string realname
  \brief The IRC Real Name of the User. */,
  ident /*!< \property Flux::string ident
  \brief The IRC Identification line of the User. */,
  fullhost /*!< \property Flux::string fullhost
  \brief The full IRC host line of the User. */,
  server /*!< \property Flux::string server
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
/**
 * \struct CommandSource "user.h" USER_H
 * \brief A wrapper class for the source of the Command
 * Contains information about where the command came from, as well as a Reply() function to quickly send a PrivMessage to the Command sender.
 */
struct CommandSource
{
 User *u; /*!< \property User *u
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