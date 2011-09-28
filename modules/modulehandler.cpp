#include "flux_net_irc.hpp"

class CommandMList : public Command
{
public:
  CommandMList():Command("MODLIST", 1, 2)
  {
    this->SetDesc("Lists all loaded modules");
    this->SetSyntax("\37priority\37");
  }
  void Run(CommandSource &source, const std::vector<Flux::string> &params)
  {
    const Flux::string priority = params.size() == 2?params[1]:"";
    int c=0;
    if(priority.empty())
    {
      for(Flux::insensitive_map<module*>::iterator it = Modules.begin(); it != Modules.end(); ++it){
	source.Reply("\2%-16s\2 %s [%s]", it->second->name.c_str(), it->second->GetAuthor().c_str(),
		     ModuleHandler::DecodePriority(it->second->priority).c_str());
	++c;
      }
    }else
    { // There is probably a WAY easier way of doing this but whatever
      for(Flux::insensitive_map<module*>::iterator it = Modules.begin(); it != Modules.end(); ++it){
	if(priority.equals_ci("LAST") || priority == '1'){
	  source.Reply("\2%-16s\2 %s [%s]", it->second->name.c_str(), it->second->GetAuthor().c_str(),
			ModuleHandler::DecodePriority(it->second->priority).c_str());
	  ++c;
	}else if(priority.equals_ci("NORMAL") || priority == '2')
	{
	  source.Reply("\2%-16s\2 %s [%s]", it->second->name.c_str(), it->second->GetAuthor().c_str(),
		     ModuleHandler::DecodePriority(it->second->priority).c_str());
	  ++c;
	}else if(priority.equals_ci("FIRST") || priority == '3')
	{
	  source.Reply("\2%-16s\2 %s [%s]", it->second->name.c_str(), it->second->GetAuthor().c_str(),
		     ModuleHandler::DecodePriority(it->second->priority).c_str());
	  ++c;
	}
      }
    }
    source.Reply("Total of \2%i\2 Modules", c);
    log(LOG_NORMAL, "%s used MODLIST to list all modules%s", source.u->nick.c_str(), 
	priority.empty()?"":Flux::string(" with priority "+priority).c_str());
  }
};

class CommandMLoad : public Command
{
public:
  CommandMLoad():Command("MODLOAD", 1, 2)
  {
    this->SetDesc("Load a module");
    this->SetSyntax("\37name\37");
  }
  void Run(CommandSource &source, const std::vector<Flux::string> &params)
  {
    const Flux::string module = params.size() == 2?params[1]:"";
    if(module.empty())
      this->SendSyntax(source);
    else if(!source.u->IsOwner())
      source.Reply(ACCESS_DENIED);
    else{
      ModErr e = ModuleHandler::LoadModule(module);
      if(e != MOD_ERR_OK)
      {
	source.Reply("Failed to load module %s: %s", module.c_str(), DecodeModErr(e).c_str());
	log(LOG_NORMAL, "%s used LOAD to load %s and failed: %s", source.u->nick.c_str(), module.c_str(), DecodeModErr(e).c_str());
      }else{
	source.Reply("Module \2%s\2 loaded sucessfuly", module.c_str());
	log(LOG_NORMAL, "%s used LOAD to load %s", source.u->nick.c_str(), module.c_str());
      }
    }
  }
};
class CommandMUnload : public Command
{
public:
  CommandMUnload():Command("MODUNLOAD", 1, 2)
  {
    this->SetDesc("Unloads a module");
    this->SetSyntax("\37name\37");
  }
  void Run(CommandSource &source, const std::vector<Flux::string> &params)
  {
    const Flux::string module = params.size() == 2?params[1]:"";
    if(module.empty())
      this->SendSyntax(source);
    else if(!source.u->IsOwner())
      source.Reply(ACCESS_DENIED);
    else{
      bool e = ModuleHandler::Unload(FindModule(module));
      if(!e)
      {
	source.Reply("Failed to unload module %s", module.c_str());
	log(LOG_NORMAL, "%s used UNLOAD to load %s and failed", source.u->nick.c_str(), module.c_str());
      }else{
	source.Reply("Module \2%s\2 unloaded sucessfuly", module.c_str());
	log(LOG_NORMAL, "%s used UNLOAD to unload %s", source.u->nick.c_str(), module.c_str());
      }
      
    }
  }
};

class CommandMInfo : public Command
{
public:
  CommandMInfo():Command("MODINFO", 1, 2)
  {
   this->SetDesc("Provides info on a module");
   this->SetSyntax("\37name\37");
  }
  void Run(CommandSource &source, const std::vector<Flux::string> &params)
  {
    const Flux::string modd = params.size() == 2?params[1]:"";
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
      log(LOG_NORMAL, "%s used MODINFO to show info on module %s", source.u->nick.c_str(), mo->name.c_str());
  }
};

class M_Handler : public module
{
  CommandMList list;
  CommandMLoad load;
  CommandMUnload unload;
  CommandMInfo info;
public:
  M_Handler():module("ModuleHandler", PRIORITY_FIRST)
  {
    this->SetAuthor("Justasic");
    this->SetVersion(VERSION);
    this->AddCommand(&info);
    this->AddCommand(&list);
    this->AddCommand(&load);
    this->AddCommand(&unload);
  }
};
MODULE_HOOK(M_Handler)