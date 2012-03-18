/* Navn IRC bot -- Module handling commands
 * 
 * (C) 2011-2012 Flux-Net
 * Contact us at Development@Azuru.net
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Anope by The Anope Team.
 */

#include "flux_net_irc.hpp"

class CommandMList : public Command
{
public:
  CommandMList(module *m):Command(m, "MODLIST", C_PRIVATE, 0, 1)
  {
    this->SetDesc("Lists all loaded modules");
    this->SetSyntax("\37priority\37");
  }
  void Run(CommandSource &source, const Flux::vector &params)
  {
    const Flux::string priority = params.size() == 2?params[1]:"";
    int c=0;
    if(priority.empty())
    {
      for(Flux::insensitive_map<module*>::iterator it = Modules.begin(); it != Modules.end(); ++it){
	source.Reply("\2%-16s\2 %s [%s]", it->second->name.c_str(), it->second->GetAuthor().c_str(),
		     ModuleHandler::DecodePriority(it->second->GetPriority()).c_str());
	++c;
      }
    }else
    { // There is probably a WAY easier way of doing this but whatever
      for(Flux::insensitive_map<module*>::iterator it = Modules.begin(); it != Modules.end(); ++it){
	if(priority.equals_ci("LAST") || priority == '1'){
	  source.Reply("\2%-16s\2 %s [%s]", it->second->name.c_str(), it->second->GetAuthor().c_str(),
			ModuleHandler::DecodePriority(it->second->GetPriority()).c_str());
	  ++c;
	}else if(priority.equals_ci("NORMAL") || priority == '2')
	{
	  source.Reply("\2%-16s\2 %s [%s]", it->second->name.c_str(), it->second->GetAuthor().c_str(),
		     ModuleHandler::DecodePriority(it->second->GetPriority()).c_str());
	  ++c;
	}else if(priority.equals_ci("FIRST") || priority == '3')
	{
	  source.Reply("\2%-16s\2 %s [%s]", it->second->name.c_str(), it->second->GetAuthor().c_str(),
		     ModuleHandler::DecodePriority(it->second->GetPriority()).c_str());
	  ++c;
	}
      }
    }
    source.Reply("Total of \2%i\2 Modules", c);
    Log(source.u, this) << "to list all module" << (priority.empty()?"":" with priority "+priority);
  }
  bool OnHelp(CommandSource &source, const Flux::string &nill)
  {
    this->SendSyntax(source);
    source.Reply(" ");
    source.Reply("This command displays a list of all modules\n"
		 "or modules by priority and displays who created\n"
		 "the modules and what priority it has in the bot");
    return true;
  }
};

class CommandMLoad : public Command
{
public:
  CommandMLoad(module *m):Command(m, "MODLOAD", C_PRIVATE, 1, 1)
  {
    this->SetDesc("Load a module");
    this->SetSyntax("\37name\37");
  }
  void Run(CommandSource &source, const Flux::vector &params)
  {
    const Flux::string module = params[1];
    if(module.empty())
      this->SendSyntax(source);
    else if(!source.u->IsOwner())
      source.Reply(ACCESS_DENIED);
    else{
      ModErr e = ModuleHandler::LoadModule(module);
      if(e != MOD_ERR_OK)
      {
	source.Reply("Failed to load module %s: %s", module.c_str(), DecodeModErr(e).c_str());
	Log(source.u, this) << "to load " << module << " and failed: " << DecodeModErr(e);
      }else{
	source.Reply("Module \2%s\2 loaded sucessfuly", module.c_str());
	Log(source.u, this) << "to load " << module;
      }
    }
  }
  bool OnHelp(CommandSource &source, const Flux::string &nill)
  {
    this->SendSyntax(source);
    source.Reply(" ");
    source.Reply("This command loads a module into the bot.\n"
		 "If the module fails to load, it will print\n"
		 "an error message, if it crashes, the bot will\n"
		 "automatically notify the owner and unload that module\n"
		 "Note: You must be the bots owner to use this command");
    return true;
  }
};
class CommandMUnload : public Command
{
public:
  CommandMUnload(module *m):Command(m, "MODUNLOAD", C_PRIVATE, 1, 1)
  {
    this->SetDesc("Unloads a module");
    this->SetSyntax("\37name\37");
  }
  void Run(CommandSource &source, const Flux::vector &params)
  {
    const Flux::string module = params[1];
    if(!source.u->IsOwner())
      source.Reply(ACCESS_DENIED);
    else{
      bool e = ModuleHandler::Unload(FindModule(module));
      if(!e)
      {
	source.Reply("Failed to unload module %s", module.c_str());
	Log(source.u, this) << "to unload " << module << " and failed";
      }else{
	source.Reply("Module \2%s\2 unloaded sucessfuly", module.c_str());
	Log(source.u, this) << "to unload " << module;
      }
    }
  }
  bool OnHelp(CommandSource &source, const Flux::string &nill)
  {
    this->SendSyntax(source);
    source.Reply(" ");
    source.Reply("This command unloads a module from the bot\n"
		 "Note: You must be the bots owner to use this command");
    return true;
  }
};

class CommandMReload : public Command
{
public:
  CommandMReload(module *m):Command(m, "MODRELOAD", C_PRIVATE, 1, 1)
  {
    this->SetDesc("Reloads a module");
    this->SetSyntax("\37name\37");
  }
  void Run(CommandSource &source, const Flux::vector &params)
  {
    const Flux::string module = params[1];
    if(!source.u->IsOwner())
      source.Reply(ACCESS_DENIED);
    else{
      bool err = ModuleHandler::Unload(FindModule(module));
      ModErr err2 = ModuleHandler::LoadModule(module);
      if(!err || err2 != MOD_ERR_OK)
      {
	source.Reply("Failed to reload module %s%s", module.c_str(), err2 != MOD_ERR_OK?Flux::string(": "+DecodeModErr(err2)).c_str():"");
	Log(source.u, this) << "to reload " << module << " and failed";
      }else{
	source.Reply("Module \2%s\2 reloaded sucessfuly", module.c_str());
	Log(source.u, this) << "to reload " << module;
      }
    }
  }
  bool OnHelp(CommandSource &source, const Flux::string &nill)
  {
    this->SendSyntax(source);
    source.Reply(" ");
    source.Reply("This command reloads the module, it does\n"
		 "the same as modunload and modload just in\n"
		 "one command to be run which is much faster");
    return true;
  }
};

class CommandMInfo : public Command
{
public:
  CommandMInfo(module *m):Command(m, "MODINFO", C_PRIVATE, 1, 1)
  {
   this->SetDesc("Provides info on a module");
   this->SetSyntax("\37name\37");
  }
  void Run(CommandSource &source, const Flux::vector &params)
  {
    const Flux::string modd = params[1];
    if(modd.empty()){
      this->SendSyntax(source);
      return;
    }
      module *mo = FindModule(modd);
      if(!mo){
	source.Reply("Module \2%s\2 is not loaded", modd.c_str());
	return;
      }
      source.Reply("******** \2%s\2 Info ********", mo->name.c_str());
      source.Reply("Module: \2%s\2 Version: \2%s\2 Author: \2%s\2\nLoaded: \2%s\2", mo->filename.c_str(), mo->GetVersion().c_str(), mo->GetAuthor().c_str(), do_strftime(mo->GetLoadTime()).c_str());
      Flux::string cmds;
      for(CommandMap::iterator it = Commandsmap.begin(); it != Commandsmap.end(); ++it)
	if((it->second->mod == mo)){ //For /msg commands
	  cmds += it->second->name+" ";
	}
	cmds.trim();
      for(CommandMap::iterator it = ChanCommandMap.begin(); it != ChanCommandMap.end(); ++it)
	if((it->second->mod == mo)){ //For Channel Commands
	  cmds += it->second->name+" ";
	}
      cmds.trim();
      if(cmds.empty())
	source.Reply("Adds no commands");
      else
	source.Reply("Adds commands: \2%s\2", cmds.c_str());
	source.Reply("******** End Info ********");
      Log(source.u, this) << "to show info on module " << mo->name;
  }
  bool OnHelp(CommandSource &source, const Flux::string &nill)
  {
    this->SendSyntax(source);
    source.Reply(" ");
    source.Reply("This command displays module information\n"
		 "based off the author of the module, its\n"
		 "priority, how many commands it adds, the\n"
		 "version of the module and when it was loaded");
    return true;
  }
};

class M_Handler : public module
{
  CommandMList list;
  CommandMLoad load;
  CommandMReload reload;
  CommandMUnload unload;
  CommandMInfo info;
  Flux::vector CurrentModuleList;
public:
  M_Handler(const Flux::string &Name):module(Name), list(this), load(this), reload(this), unload(this), info(this)
  {
    this->SetAuthor("Justasic");
    this->SetVersion(VERSION);
    this->SetPriority(PRIORITY_FIRST);
    Implementation i[] = { I_OnStart, I_OnReload };
    ModuleHandler::Attach(i, this, sizeof(i)/sizeof(Implementation));
  }
  void OnStart(int, char**)
  {
    CurrentModuleList = SerializeString(Config->Modules, ',');
    for(unsigned i=0; i < CurrentModuleList.size(); ++i){
      CurrentModuleList[i].trim();
      printf("[%i] '%s'\n", i, CurrentModuleList[i].c_str());
    }
  }
  void OnReload()
  {
    Flux::vector updatedmodlist = SerializeString(Config->Modules, ',');
    for(Flux::vector::iterator it; it != updatedmodlist.end(); ++it)
    {
      (*it).trim();
      for(Flux::vector::iterator it2; it2 != CurrentModuleList.end(); ++it2)
      {
	if((*it) != (*it2)){
	  Log(LOG_TERMINAL) << "(*it) != (*it2)";
	  ModuleHandler::Unload(FindModule(*it2));
	  CurrentModuleList.erase(it2);
	}else if((*it2) != (*it)){
	  Log(LOG_TERMINAL) << "(*it2) != (*it)";
	  ModuleHandler::LoadModule(*it);
	  CurrentModuleList.push_back(*it);
	}
      }
    }
  }
};
MODULE_HOOK(M_Handler)