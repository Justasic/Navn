#include "flux_net_irc.hpp"

class CommandMList : public Command
{
public:
  CommandMList():Command("MODLIST", 1, 2)
  {
    this->SetDesc("Lists all loaded modules");
    this->SetSyntax("MODLIST \37priority\37");
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
	if(priority.equals_ci("LOW") || priority == '1'){
	  source.Reply("\2%-16s\2 %s [%s]", it->second->name.c_str(), it->second->GetAuthor().c_str(),
			ModuleHandler::DecodePriority(it->second->priority).c_str());
	  ++c;
	}else if(priority.equals_ci("NORMAL") || priority == '2')
	{
	  source.Reply("\2%-16s\2 %s [%s]", it->second->name.c_str(), it->second->GetAuthor().c_str(),
		     ModuleHandler::DecodePriority(it->second->priority).c_str());
	  ++c;
	}else if(priority.equals_ci("LOW") || priority == '3')
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
    this->SetSyntax("MODLOAD \37name\37");
  }
  void Run(CommandSource &source, const std::vector<Flux::string> &params)
  {
    const Flux::string module = params.size() == 2?params[1]:"";
    if(module.empty())
      this->SendSyntax(source);
    else if(!source.u->IsOwner())
      source.Reply(ACCESS_DENIED);
    else{
      ModuleHandler::LoadModule(module);
      log(LOG_NORMAL, "%s used LOAD to unload %s", source.u->nick.c_str(), module.c_str());
    }
  }
};

class CommandMUnload : public Command
{
public:
  CommandMUnload():Command("MODUNLOAD", 1, 2)
  {
    this->SetDesc("Unloads a module");
    this->SetSyntax("MODUNLOAD \37name\37");
  }
  void Run(CommandSource &source, const std::vector<Flux::string> &params)
  {
    const Flux::string module = params.size() == 2?params[1]:"";
    if(module.empty())
      this->SendSyntax(source);
    else if(!source.u->IsOwner())
      source.Reply(ACCESS_DENIED);
    else{
      ModuleHandler::Unload(FindModule(module));
      log(LOG_NORMAL, "%s used UNLOAD to unload %s", source.u->nick.c_str(), module.c_str());
    }
  }
};

class M_Handler : public module
{
  CommandMList list;
  CommandMLoad load;
  CommandMUnload unload;
public:
  M_Handler():module("ModuleHandler", PRIORITY_FIRST)
  {
    this->SetAuthor("Justasic");
    this->AddCommand(&list);
    this->AddCommand(&load);
    this->AddCommand(&unload);
  }
};
MODULE_HOOK(M_Handler)