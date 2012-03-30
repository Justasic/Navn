/* Navn IRC bot -- Module header
 * 
 * (C) 2011-2012 Azuru
 * Contact us at Development@Azuru.net
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Anope by The Anope Team.
 */
#pragma once
#ifndef MODULE_H
#define MODULE_H

#include "user.h"
#include "command.h"
#ifndef _WIN32
# ifdef HAVE_FCNTL_H
#  include <dlfcn.h>
# else
#  error dlfcn.h is required by navn to compile modules!
# endif
#endif
enum Implementation
{
  I_BEGIN,
	I_OnPrivmsg, I_OnModuleLoad, I_OnModuleUnload,
	I_OnRestart, I_OnShutdown, I_OnReload, I_OnCommand,
	I_OnStart, I_OnNumeric, I_OnPreConnect, I_OnPostConnect,
	I_OnCTCP, I_OnQuit, I_OnJoin, I_OnKick, I_OnConnectionError,
	I_OnNotice, I_OnNickChange, I_OnChannelMode, I_OnUserMode,
	I_OnChannelOp, I_OnPart, I_OnInvite, I_OnArgument, I_OnFork,
	I_OnSocketError, I_OnPing, I_OnPong, I_OnPreReceiveMessage,
	I_OnPrivmsgChannel, I_OnNoticeChannel, I_OnLog,
  I_END
};

enum ModulePriority
{
  PRIORITY_FIRST,
  PRIORITY_DONTCARE,
  PRIORITY_LAST
};

class CoreExport module : public Base
{
  Flux::string author, version;
  time_t loadtime;
  ModulePriority Priority;
  bool permanent;
protected:
  void SetAuthor(const Flux::string&);
  void SetVersion(const Flux::string&);
  void SetPriority(ModulePriority);
  void SetPermanent(bool);
public:
  void *handle;
  Flux::string name, filename, filepath;
  Flux::string GetAuthor();
  Flux::string GetVersion();
  bool GetPermanent();
  ModulePriority GetPriority();
  time_t GetLoadTime();
  module(const Flux::string&);
  
  virtual ~module();
  virtual EventResult OnPreReceiveMessage(const Flux::string&) { return EVENT_CONTINUE; }
  virtual void OnPrivmsg(User*, const std::vector<Flux::string>&) {}
  virtual void OnPrivmsgChannel(User*, Channel*, const std::vector<Flux::string>&) {}
  virtual void OnNotice(User*, const std::vector<Flux::string>&) {}
  virtual void OnNoticeChannel(User*, Channel*, const std::vector<Flux::string>&) {}
  virtual void OnCTCP(const Flux::string&, const std::vector<Flux::string>&) {}
  virtual void OnPing(const std::vector<Flux::string>&) {}
  virtual void OnPong(const std::vector<Flux::string>&) {}
  virtual void OnArgument(int, const Flux::string&) {}
  virtual EventResult OnLog(Log*) { return EVENT_CONTINUE; }
  virtual void OnModuleLoad(module*) {}
  virtual void OnFork(int) {}
  virtual void OnSocketError(const Flux::string&) {}
  virtual void OnModuleUnload(module*){}
  virtual void OnRestart(const Flux::string&) {}
  virtual void OnShutdown() {}
  virtual void OnNickChange(User*, const Flux::string&) {}
  virtual void OnQuit(User*, const Flux::string&) {}
  virtual void OnJoin(User*, Channel*) {}
  virtual void OnKick(User*, User*, Channel*, const Flux::string&) {}
  virtual void OnNumeric(int, const std::vector<Flux::string>&) {}
  virtual void OnReload() {}
  virtual void OnCommand(const Flux::string&, const std::vector<Flux::string>&) {}
  virtual void OnStart(int, char**) {}
  virtual void OnChannelMode(User*, Channel*, const Flux::string&) {}
  virtual void OnChannelOp(User*, Channel*, const Flux::string&, const Flux::string&) {}
  virtual void OnPart(User*, Channel*, const Flux::string&) {}
  virtual void OnUserMode(User*, const Flux::string&, const Flux::string&) {}
  virtual void OnPreConnect(const Flux::string&, const Flux::string&) {}
  virtual void OnPostConnect(SocketIO*) {}
  virtual void OnConnectionError(const Flux::string&) {}
  virtual void OnInvite(User *u, const Flux::string&) {}
  virtual void OnLoad() {} // This is NOT to be called by FOREACH_MOD!
};

class CoreExport ModuleHandler
{
public:
  static std::vector<module*> EventHandlers[I_END];
  static ModErr LoadModule(const Flux::string&);
  static Flux::string DecodePriority(ModulePriority);
  static void SanitizeRuntime();
  static void UnloadAll();
  static bool Unload(module*);
  
  static bool Attach(Implementation i, module *mod);
  static void Attach(Implementation *i, module *mod, size_t sz);
  static bool Detach(Implementation i, module *mod);
  static void DetachAll(module*);
private:
  static bool DeleteModule(module*);
};
#endif