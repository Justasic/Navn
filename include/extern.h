/* Navn IRC bot -- External variable definition header
 *
 * (C) 2011-2012 Azuru
 * Contact us at Development@Azuru.net
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Anope by The Anope Team.
 */
#pragma once
#ifndef EXTERN_H
#define EXTERN_H

#include "flux.h"
#include <list> //for std::list
/* Prototypes and external variable declarations only */

#include "windows_navn.h" //Include windows crap

/* #define's */
#define E extern CoreExport
#define BUFSIZE 65535
#define CHANNEL_X_INVALID "Channel \2%s\2 is not a valid channel"
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
class Module;
class IRCProto;
class ModuleHandler;
class INIReader;
class BotConfig;
class IsoHost;
class Clock;
class TextFile;
class XMLFile;
struct CommandSource;

/* structs */
struct iSupport
{
  // Other options the server might send.
  Flux::map<Flux::string> other;
  // Supported chan modes
  Flux::string ChanModes;
  // Supported chan types
  Flux::string ChanTypes;
  // The IRCd version
  Flux::string IRCdVersion;
  // User modes
  Flux::string UserModes;
  // Network name
  Flux::string Network;
  // Servers hostname
  Flux::string ServerHost;
  // Max away length
  int AwayLen;
  // Max kick length
  int KickLen;
  // Max channel length
  int MaxChannelLen;
  // Max channels you can join
  int MaxChannels;
  // Max number of bans settable
  int MaxBans;
  // Max nickname length
  int NickLen;
  // Max Topic Length
  int TopicLen;
};

/* Enumorations */
enum LogType
{
  LOG_DEBUG,
  LOG_DEVEL,
  LOG_MEMORY,
  LOG_NORMAL,
  LOG_RAWIO,
  LOG_TERMINAL,
  LOG_WARN,
  LOG_CRITICAL,
  LOG_THREAD,
  LOG_SILENT
};

enum EventResult
{
  EVENT_CONTINUE,
  EVENT_STOP
};

enum CommandType
{
  C_NULL,
  C_CHANNEL,
  C_PRIVATE
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
E Module *FindModule(const Flux::string &name);
E Command *FindCommand(const Flux::string &name, CommandType);

/* extern's */
E SocketIO *sock;
E IRCProto *ircproto;
E BotConfig *Config;
E iSupport isupport;
E Module *LastRunModule;
E CommandMap Commandsmap;
E CommandMap ChanCommandMap;
E std::vector<Base*> BaseReferences;
E time_t starttime;
E uint32_t usercnt, maxusercnt;
E Flux::string binary_path, bot_bin, binary_dir, quitmsg;
E size_t memused, memobjects, newcalls, deletecalls;
E const Flux::string password;
E const Flux::string VERSION_LONG;
E Flux::string ForwardResolution(const Flux::string&);
E Flux::string getprogdir(const Flux::string&, Flux::string &bin_var1 = bot_bin);
E Flux::string DecodeModErr(ModErr err);
E Flux::string isolate(char begin, char end, const Flux::string &msg);
E Flux::string make_pass();
E Flux::string do_strftime(const time_t &t, bool short_output = false);
E Flux::string duration(const time_t &t);
E Flux::string printfify(const char*, ...);
E int randint(int x, int y);
E bool IsValidChannel(const Flux::string&);
E bool InTerm();
E bool protocoldebug, IsOper, dev, nofork, quitting, started, nocolor, memdebug, istempnick;
// E Flux::vector ParamitizeString(const Flux::string&, char);
E Flux::insensitive_map<Module*> Modules;
E Flux::insensitive_map<User*> UserNickList;
E Flux::insensitive_map<Channel*> ChanMap;

/* void's */
E void Rehash();
E void GarbageCollect();
E void QuitUser(User*);
E void Fork();
E void sigact(int);
E void InitSignals();
E void HandleSegfault(Module*);
E void restart(const Flux::string&);
E void ListChans(CommandSource &source);
E void ListUsers(CommandSource &source);
E void send_cmd(const char *fmt, ...);
E void process(const Flux::string&);
E void ProcessJoin(CommandSource&, const Flux::string&);
E void ProcessCommands(CommandSource&, std::vector<Flux::string>&);
// This will process any command through the core.
E void ProcessCommand(CommandSource &Source, Flux::vector &params2, const Flux::string &receiver,
		      const Flux::string &command);
// E void ReadConfig();

/* Char's */
E char segv_location[255];
E char **my_av, **my_envp;

// This nifty little function deletes and zeros a pointer
template<typename T> void DeleteZero(T *&t)
{
  T *p = t;
  delete p;
  t = NULL;
}

/**************************************************************/
/* This is the only #define allowed at the bottom of the file */
#ifdef HAVE_SETJMP_H
// We have setjmp, try and recover from Segmentation Faults
#define FOREACH_MOD(y, x) \
if(true) \
{ \
    std::vector<Module*>::iterator safei; \
    for (std::vector<Module*>::iterator _i = ModuleHandler::EventHandlers[y].begin(); _i != ModuleHandler::EventHandlers[y].end(); ) \
    { \
       safei = _i; \
       ++safei; \
       try \
       { \
	  SET_SEGV_LOCATION(); \
	  if(setjmp(sigbuf) == 0) \
	  { \
	    LastRunModule = *_i; \
	    (*_i)->x ; \
	  } \
	  else \
	  {\
	    throw ModuleException(printfify("%s failed to run an event. Segmentation fault occured.", LastRunModule->name.c_str())); \
	    for(unsigned _o = 0; _o < Config->Owners.size(); ++_o) \
	    { \
	      User *ou = finduser(Config->Owners[_o]); \
	      if(ou) \
		ou->SendMessage("Module \2%s\2 has crashed when running an event!", LastRunModule->name.c_str()); \
	    } \
	  } \
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

#define FOREACH_RESULT(y, x, v) \
if (true) \
{ \
  std::vector<Module*>::iterator safei; \
  v = EVENT_CONTINUE; \
  for (std::vector<Module*>::iterator _i = ModuleHandler::EventHandlers[y].begin(); _i != ModuleHandler::EventHandlers[y].end();) \
    { \
      safei = _i; \
      ++safei; \
      try \
      { \
	if(setjmp(sigbuf) == 0) \
	{ \
	  SET_SEGV_LOCATION(); \
	  EventResult res = (*_i)->x ; \
	  if (res != EVENT_CONTINUE) \
	  { \
	    v = res; \
	    break; \
	  } \
	} \
	else \
	{ \
	  throw ModuleException(printfify("%s failed to run an event. Segmentation fault occured.", LastRunModule->name.c_str())); \
	  for(unsigned _o = 0; _o < Config->Owners.size(); ++_o) \
	  { \
	    User *ou = finduser(Config->Owners[_o]); \
	    if(ou) \
	      ou->SendMessage("Module \2%s\2 has crashed when running an event!", LastRunModule->name.c_str()); \
	  } \
	} \
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

#else // HAVE_SETJMP_H
// We don't have setjmp
#define FOREACH_MOD(y, x) \
if(true) \
{ \
    EventsVector::iterator safei; \
    for (EventsVector::iterator _i = ModuleHandler::EventHandlers[y].begin(); _i != ModuleHandler::EventHandlers[y].end(); ) \
    { \
       safei = _i; \
       ++safei; \
       try \
       { \
	  SET_SEGV_LOCATION(); \
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

#define FOREACH_RESULT(y, x, v) \
if (true) \
{ \
  EventsVector::iterator safei; \
  v = EVENT_CONTINUE; \
  for (EventsVector::iterator _i = ModuleHandler::EventHandlers[y].begin(); _i != ModuleHandler::EventHandlers[y].end();) \
    { \
      safei = _i; \
      ++safei; \
      try \
      { \
	SET_SEGV_LOCATION(); \
	EventResult res = (*_i)->x ; \
	if (res != EVENT_CONTINUE) { \
	  v = res; \
	  break; \
	} \
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

#endif // HAVE_SETJMP_H


#endif
