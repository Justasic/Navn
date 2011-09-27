/* All code is licensed under GNU General Public License GPL v3 (http://www.gnu.org/licenses/gpl.html) */
#ifndef EXTERN_H
#define EXTERN_H
#include "flux.h"
#include <list> //for std::list
/* Prototypes and external variable declarations only */

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
class BotConfig;
struct CommandSource;

/* Enumorations */
enum LogType
{
  LOG_DEBUG,
  LOG_NORMAL,
  LOG_RAWIO,
  LOG_TERMINAL
};
enum ModErr{
MOD_ERR_OK,
MOD_ERR_MEMORY,
MOD_ERR_PARAMS,
MOD_ERR_EXISTS,
MOD_ERR_NOEXIST,
MOD_ERR_NOLOAD,
MOD_ERR_UNKNOWN,
MOD_ERR_FILE_IO,
MOD_ERR_EXCEPTION
};

/* Typedef's */
typedef std::map<Flux::string, Command *, std::less<ci::string> > CommandMap;

/*  Class pointer finder definitions */
Channel *findchannel(const Flux::string&);
User *finduser(const Flux::string &nick);
module *FindModule(const Flux::string &name);
Command *FindCommand(const Flux::string &name);
Command *FindChanCommand(const Flux::string &name);

/* extern's */
extern SocketIO *sock;
extern SendMessage *Send;
extern BotConfig *Config;
extern CommandMap Commandsmap;
extern CommandMap ChanCommandMap;
extern Flux::string binary_path, bot_bin, binary_dir, server_name;
extern Flux::string strip(const Flux::string &buf);
extern Flux::string getprogdir(const Flux::string&);
extern Flux::string TempFile(const Flux::string&);
extern Flux::string DecodeModErr(ModErr err);
extern Flux::string isolate(char begin, char end, const Flux::string &msg);
extern Flux::string make_pass();
extern Flux::string do_strftime(const time_t &t, bool short_output = false);
extern Flux::string duration(const time_t &t);
extern Flux::string sprintf(const char*, ...);
extern int randint(int x, int y);
extern bool IsValidChannel(const Flux::string&);
extern bool protocoldebug, IsOper, dev, nofork, quitting, started;
extern std::vector<Flux::string> StringVector(const Flux::string&, char);
extern Flux::insensitive_map<module*> Modules;
extern Flux::insensitive_map<User *> UserNickList;
extern Flux::insensitive_map<Channel*> ChanMap;
extern char **my_av, **my_envp;

/* void's */
void ListChans(CommandSource &source);
void ListUsers(CommandSource &source);
void send_cmd(const char *fmt, ...);
void log(LogType, const char *fmt, ...);
void process(const Flux::string&);
void ProcessJoin(CommandSource&, const Flux::string&);
void ProcessCommands(CommandSource&, std::vector<Flux::string>&);
void ReadConfig();

/************************************************************/
/* 	This is the only #define allowed in this file	    */
#define CHANNEL_X_INVALID "Channel \2%s\2 is not a valad channel"
#define welcome_msg "%s has joined. Type '/msg %s help' to see a list of commands."
#define ACCESS_DENIED "Access is Denied."
#define GetCurrentDir getcwd
#define FOREACH_MOD(y, x) \
if(true) \
{ \
    std::vector<module*>::iterator safei; \
    for (std::vector<module*>::iterator _i = ModuleHandler::EventHandlers[y].begin(); _i != ModuleHandler::EventHandlers[y].end(); ) \
    { \
       safei = _i; \
       ++safei; \
       try \
       { \
          (*_i)->x ; \
       } \
       catch (const ModuleException &modexcept) \
       { \
          log(LOG_NORMAL, "Exception caught: %s", modexcept.GetReason()); \
       } \
        _i = safei; \
    } \
} \
else \
      static_cast<void>(0)

#endif