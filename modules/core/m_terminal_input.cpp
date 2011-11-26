#include "flux_net_irc.hpp"

void ProcessInput(const Flux::string &str)
{
  std::vector<Flux::string> params = StringVector(str, ' ');
  if(params.empty())
    return;

  if(params[0].equals_ci("QUIT")){
	quitting = true;
	send_cmd("%s\n", str.c_str());
  }
  else if(params[0].equals_ci("MSG"))
    send_cmd("PRIVMSG %s\n", str.substr(4).c_str());
  else if(params[0].equals_ci("NICK"))
    send_cmd("NICK %s\n", str.substr(5).c_str());
  else if(params[0].equals_ci("MODRELOAD")){
    if(params.size() >= 2){
      module *m = FindModule(params[1]);
      if(m)
      {
	Log(LOG_TERMINAL) << "Reloading module " << params[1];
	if(!ModuleHandler::Unload(m))
	  Log(LOG_TERMINAL) << "Failed to unload module " << params[1];
	ModErr e = ModuleHandler::LoadModule(params[1]);
	if(e != MOD_ERR_OK)
	{
	  Log(LOG_TERMINAL) << "Failed to reload module " << params[1] << ": " << DecodeModErr(e);
	}
	Log(LOG_TERMINAL) << "Successfuly reloaded " << params[1];
      }else
	Log(LOG_TERMINAL) << "Module " << params[1] << " does not exist!";
    }else
      Log(LOG_TERMINAL) << "Syntax: MODUNLOAD modulename";
  }
  else if(params[0].equals_ci("MODUNLOAD")){
    if(params.size() >= 2)
    {
      module *m = FindModule(params[1]);
      if(m)
      {
	if(!ModuleHandler::Unload(m))
	  Log(LOG_TERMINAL) << "Failed to unload module " << params[1];
      }else
	Log(LOG_TERMINAL) << "Module " << params[1] << " does not exist!";
    }else
      Log(LOG_TERMINAL) << "Syntax: MODUNLOAD modulename";
  }
  else if(params[0].equals_ci("MODLOAD"))
  {
    if(params.size() >= 2)
    {
      ModErr e = ModuleHandler::LoadModule(params[1]);
      if(e != MOD_ERR_OK)
	Log(LOG_TERMINAL) << "Failed to load module " << params[1] << ": " << DecodeModErr(e);
      else
	Log(LOG_TERMINAL) << "Successfuly loaded module " << params[1];
    }
  }
  else if(params[0].equals_ci("MODLIST"))
  {
    const Flux::string priority = params.size() == 2?params[1]:"";
    int c=0;
    if(priority.empty())
    {
      for(Flux::insensitive_map<module*>::iterator it = Modules.begin(); it != Modules.end(); ++it){
	printf("%-16s %s [%s]\n", it->second->name.c_str(), it->second->GetAuthor().c_str(),
	  ModuleHandler::DecodePriority(it->second->GetPriority()).c_str());
	  ++c;
      }
    }else{ // There is probably a WAY easier way of doing this but whatever
      for(Flux::insensitive_map<module*>::iterator it = Modules.begin(); it != Modules.end(); ++it){
	if(priority.equals_ci("LAST") || priority == '1'){
	  printf("%-16s %s [%s]\n", it->second->name.c_str(), it->second->GetAuthor().c_str(),
		      ModuleHandler::DecodePriority(it->second->GetPriority()).c_str());
	  ++c;
	}else if(priority.equals_ci("NORMAL") || priority == '2')
	{
	  printf("%-16s %s [%s]\n", it->second->name.c_str(), it->second->GetAuthor().c_str(),
		      ModuleHandler::DecodePriority(it->second->GetPriority()).c_str());
	  ++c;
	}else if(priority.equals_ci("FIRST") || priority == '3')
	{
	  printf("%-16s %s [%s]\n", it->second->name.c_str(), it->second->GetAuthor().c_str(),
		      ModuleHandler::DecodePriority(it->second->GetPriority()).c_str());
	  ++c;
	}
      }
    }
    printf("Total of %i Modules\n", c);
  }
  else if(params[0].equals_ci("RESTART")){
    Flux::string reason = params.size() > 2?str.substr(8):"No Reason";
    reason.trim();
    restart(reason);
  }
  else
    send_cmd("%s\n", str.c_str());
}

/** \class InputThread
 * This thread allows for user input to be possible, this is activated when the nofork option is specified.
 */
class InputThread : public Thread 
{
public:
  bool exiting;
  InputThread():Thread() { Log() << "Input Thread Initializing."; exiting = false; }
  ~InputThread() { Log() << "Input Thread Exiting."; exiting = true; }
  void ToRun()
  {
    base_string buf;
    while(!exiting)
    {
      Log(LOG_RAWIO) << "Top of Input Loop";
      std::getline(std::cin, buf);
      if(!buf.empty())
	ProcessInput(buf);
    }
    SetExitState();
  }
};

class ModTerminalInput : public module
{
  Thread *t;
public:
  ModTerminalInput(const Flux::string &Name):module(Name)
  {
    this->SetVersion(VERSION);
    this->SetAuthor("Justasic");
    ModuleHandler::Attach(I_OnStart, this);
  }
  ~ModTerminalInput() { if(t) delete t; }
  void OnStart(int, char**)
  {
    if(nofork && InTerm()){
      t = new InputThread();
      t->Start();
    }else
      throw ModuleException("Cannot run m_terminal_input when fork'ed");
  }
  void OnLoad() //This virtual is called when the module is loaded.
  {
    if(!t)
    {
      if(nofork && InTerm()){
	t = new InputThread();
	t->Start();
      }else
	throw ModuleException("Cannot run m_terminal_input when fork'ed");
    }
  }
};

MODULE_HOOK(ModTerminalInput)