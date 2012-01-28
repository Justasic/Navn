/* All code is licensed under GNU General Public License GPL v3 (http://www.gnu.org/licenses/gpl.html) */
#ifndef EXTERN_H
#define EXTERN_H
#include "flux.h"
#include <list> //for std::list
/* Prototypes and external variable declarations only */

#include "windows_navn.h" //Include windows crap

/* #define's */
#define E extern CoreExport
#define BUFSIZE 65535
#define CHANNEL_X_INVALID "Channel \2%s\2 is not a valad channel"
#define welcome_msg "%s has joined. Type '/msg %s help' to see a list of commands."
#define isvalidnick(c) (isalnum(c) || ((c) >= '\x5B' && (c) <= '\x60') || ((c) >= '\x7B' && (c) <= '\x7D') || (c) == '-')
#define isalphibetic(c) (((c) >= '\x41' && (c) <= '\x5A') || ((c) >= '\x61' && (c) <= '\x7A'))
#define isalphibeticnum(c) ( isalnum(c) || ((c) >= '\x41' && (c) <= '\x5A') || ((c) >= '\x61' && (c) <= '\x7A'))
#define ACCESS_DENIED "Access is Denied."
#define SET_SEGV_LOCATION() snprintf(segv_location, sizeof(segv_location), "%s %d %s", __FILE__, __LINE__, __PRETTY_FUNCTION__);
#define CLEAR_SEGV_LOCATION() segv_location[0]='\0';

#ifdef HAVE_SETJMP_H
E jmp_buf sigbuf;
#endif

/* Classes */
class Channel;
class Log;
class User;
class SocketIO;
class Commands;
class Command;
class Oper;
class Thread;
class module;
class IRCProto;
class ModuleHandler;
class INIReader;
class BotConfig;
class IsoHost;
class Clock;
class TextFile;
class XMLFile;
struct CommandSource;

/* Enumorations */
enum LogType
{
  LOG_DEBUG,
  LOG_NORMAL,
  LOG_RAWIO,
  LOG_TERMINAL,
  LOG_SILENT
};
enum CommandType
{
  COMMAND_CHANNEL,
  COMMAND_PRIVATE
};
enum ModErr
{
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
typedef std::map<Flux::string, Command*, ci::less> CommandMap;

/*  Class pointer finder definitions */
E Channel *findchannel(const Flux::string&);
E User *finduser(const Flux::string &nick);
E module *FindModule(const Flux::string &name);
E Command *FindCommand(const Flux::string &name, CommandType);

/* extern's */
E SocketIO *sock;
E IRCProto *ircproto;
E BotConfig *Config;
E module *LastRunModule;
E CommandMap Commandsmap;
E CommandMap ChanCommandMap;
E time_t starttime;
E uint32_t usercnt, maxusercnt;
E Flux::string binary_path, bot_bin, binary_dir, server_name, quitmsg, LastBuf;
E const Flux::string VERSION_LONG;
E Flux::string ForwardResolution(const Flux::string&);
E Flux::string getprogdir(const Flux::string&);
E Flux::string DecodeModErr(ModErr err);
E Flux::string isolate(char begin, char end, const Flux::string &msg);
E Flux::string make_pass();
E Flux::string do_strftime(const time_t &t, bool short_output = false);
E Flux::string duration(const time_t &t);
E Flux::string printfify(const char*, ...);
E Flux::string CondenseString(const Flux::vector&);
E int randint(int x, int y);
E bool IsValidChannel(const Flux::string&);
E bool InTerm();
E bool protocoldebug, IsOper, dev, nofork, quitting, started, nocolor, istempnick;
E Flux::vector SerializeString(const Flux::string&, char);
E Flux::insensitive_map<module*> Modules;
E Flux::insensitive_map<User*> UserNickList;
E Flux::insensitive_map<Channel*> ChanMap;

/* void's */
E void Rehash();
E void QuitUser(User*);
E void Fork();
E void sigact(int);
E void InitSignals();
E void HandleSegfault(module*);
E void restart(const Flux::string&);
E void ListChans(CommandSource &source);
E void ListUsers(CommandSource &source);
E void send_cmd(const char *fmt, ...);
E void process(const Flux::string&);
E void ProcessJoin(CommandSource&, const Flux::string&);
E void ProcessCommands(CommandSource&, std::vector<Flux::string>&);
E void ReadConfig();

/* Char's */
E char segv_location[255];
E char **my_av, **my_envp;

/**************************************************************/
/* This is the only #define allowed at the bottom of the file */
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
	  Log() << "Exception caught: " << modexcept.GetReason(); \
       } \
        _i = safei; \
    } \
} \
else \
      static_cast<void>(0)

#endif