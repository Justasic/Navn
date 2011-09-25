/* All code is licensed under GNU General Public License GPL v3 (http://www.gnu.org/licenses/gpl.html) */
#ifndef MODULE_H
#define MODULE_H
#include "user.h"
#include "includes.h"
enum Implementation{
  I_BEGIN,
	I_OnPrivmsg, I_OnModuleLoad, I_OnModuleUnload,
	I_OnRestart, I_OnShutdown, I_OnReload, I_OnCommand,
	I_OnStart,
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
public:
  void *handle;
  Flux::string name, filename;
  ModulePriority priority;
  Flux::string author;
  module(const Flux::string&, ModulePriority);
  int AddCommand(Command *c);
  int DelCommand(Command *c);
  
  virtual ~module();
  
  virtual void OnPrivmsg(const Flux::string&, const std::vector<Flux::string>&) {}
  virtual void OnModuleLoad(module*) {}
  virtual void OnModuleUnload(module*){}
  virtual void OnRestart(const Flux::string&) {}
  virtual void OnShutdown() {}
  virtual void OnReload(bool) {}
  virtual void OnCommand(CommandSource&, const std::vector<Flux::string>&) {}
  virtual void OnStart(int argc, char** argv) {}
};

class ModuleHandler
{
public:
  static std::vector<module *> EventHandlers[I_END];
  static ModErr LoadModule(const Flux::string&);
  static void SanitizeRuntime();
  static void UnloadAll();
  static bool Unload(module*);
  
  static bool Attach(Implementation i, module *mod);
  static bool Detach(Implementation i, module *mod);
private:
  static bool DeleteModule(module*);
};
#endif