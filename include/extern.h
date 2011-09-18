/* All code is licensed under GNU General Public License GPL v3 (http://www.gnu.org/licenses/gpl.html) */
#ifndef EXTERN_H
#define EXTERN_H
#include "flux.h"
/* This file is NOT for functions, it is for defining stuff like classes */

/* #define's */
#define Delete unlink

/* Classes */
class Channel;
class User;
class SocketIO;
class Commands;
class Command;
class Oper;
class Thread;
class module;
class SendMessage;
class ModuleHandler;
class INIReader;
struct CommandSource;

/* Enumorations */
enum LogType
{
  LOG_DEBUG,
  LOG_NORMAL,
  LOG_RAWIO,
  LOG_TERMINAL
};

/*  *pointer definitions */
Channel *findchannel(const Flux::string&);
User *finduser(const Flux::string &nick);
module *FindModule(const Flux::string &name);
Command *FindCommand(const Flux::string &name);

/* extern's */
extern SocketIO *sock;
extern SendMessage *Send;
extern INIReader *config;
extern Flux::string nick, owner_nick, logfile, binary_path, bot_bin, binary_dir;
extern Flux::string strip(const Flux::string &buf);
extern Flux::string getprogdir(const Flux::string&);
extern Flux::string TempFile(const Flux::string&);
extern Flux::string make_pass();
extern int randint(int x, int y);
extern bool IsValidChannel(const Flux::string&);
extern bool protocoldebug, IsOper, dev, nofork, quitting;
extern std::vector<Flux::string> StringVector(const Flux::string&, char);
extern std::vector<module*> moduleList;
extern Flux::insensitive_map<User *> UserNickList;
extern Flux::insensitive_map<Channel*> ChanMap;

/* Typedef's */


/* void's */
void ListChans(CommandSource &source);
void ListUsers(CommandSource &source);
void send_cmd(const char *fmt, ...);
void log(LogType, const char *fmt, ...);
void process(const Flux::string&);
void ProcessJoin(CommandSource&, const Flux::string&);
void ProcessModules(CommandSource&, std::vector<Flux::string>&);

/* char*'s */
extern char **my_av, **my_envp;
#endif