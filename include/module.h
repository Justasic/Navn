/* All code is licensed under GNU General Public License GPL v3 (http://www.gnu.org/licenses/gpl.html) */
#ifndef MODULE_H
#define MODULE_H
#include "user.h"
#include "includes.h"
#ifdef HAVE_FCNTL_H
# include <dlfcn.h>
#else
# error dlfcn.h is required by navn to compile modules!
#endif
enum Implementation{
  I_BEGIN,
	I_OnPrivmsg, I_OnModuleLoad, I_OnModuleUnload,
	I_OnRestart, I_OnShutdown, I_OnReload, I_OnCommand,
	I_OnStart, I_OnNumeric, I_OnPreConnect, I_OnPostConnect,
	I_OnCTCP, I_OnQuit, I_OnJoin, I_OnKick, I_OnConnectionError,
	I_OnNotice, I_OnNickChange, I_OnChannelMode, I_OnUserMode,
	I_OnChannelOp, I_OnPart, I_OnInvite, I_OnArgument, I_OnFork,
  I_END
};
enum ModuleReturn{
  MOD_RUN,
  MOD_STOP
};
enum ModulePriority{
  PRIORITY_FIRST,
  PRIORITY_DONTCARE,
  PRIORITY_LAST
};
class module{
  Flux::string author, version;
  time_t loadtime;
  ModulePriority Priority;
protected:
  void SetAuthor(const Flux::string&);
  void SetVersion(const Flux::string&);
  void SetPriority(ModulePriority);
  int AddCommand(Command*);
  int AddChanCommand(Command*);
public:
  int DelChanCommand(Command*);
  int DelCommand(Command*);
  void *handle;
  Flux::string name, filename, filepath;
  Flux::string GetAuthor();
  Flux::string GetVersion();
  ModulePriority GetPriority();
  time_t GetLoadTime();
  module(const Flux::string&);
  
  virtual ~module();
  virtual void OnPrivmsg(User*, const std::vector<Flux::string>&) {}
  virtual void OnPrivmsg(User*, Channel*, const std::vector<Flux::string>&) {}
  virtual void OnNotice(User*, const std::vector<Flux::string>&) {}
  virtual void OnNotice(User*, Channel*, const std::vector<Flux::string>&) {}
  virtual void OnCTCP(const Flux::string&, const std::vector<Flux::string>&) {}
  virtual void OnArgument(int, const Flux::string&) {}
  virtual void OnModuleLoad(module*) {}
  virtual void OnFork(int) {}
  virtual void OnModuleUnload(module*){}
  virtual void OnRestart(const Flux::string&) {}
  virtual void OnShutdown() {}
  virtual void OnNickChange(User*, const Flux::string&) {}
  virtual void OnQuit(User*, const Flux::string&) {}
  virtual void OnJoin(User*, Channel*) {}
  virtual void OnKick(User*, User*, Channel*, const Flux::string&) {}
  virtual void OnNumeric(int) {}
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
};
class ModuleHandler
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
#define MODULE_HOOK(x) \
extern "C" module *ModInit(const Flux::string &name) { return new x(name); } \
extern "C" void Moduninit(x *m) { if(m) delete m; }
#endif