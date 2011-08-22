#ifndef MODULEHANDLER_H
#define MODULEHANDLER_H
#include "../flux_net_irc.hpp"

/**
 * \file modulehandler.h Header file holding the \a module \a handling functions.
 * \author Justasic and Lordofsraam.
 */

/**
 * \defgroup modulehandlerM About Me Module
 * This is the module for Navn's module handling functions. \n
 * For a better description see the function description.
 * \section commands Commands associated with this module.
 * \subsection !flipmod !flipmod module
 * Type \a !flipmod in a channel with a module name and it will enable or disable that module
 * \subsection !modlist !modlist
 * Type \a !modlist in a channel and it lists the compiled modules
 * \subsection !modload !modload module name
 * Type \a !modload in a channel with a module name and it loads the module specified
 * \subsection !modunload !modunload module
 * Type \a !modunload in a channel with a module and it will unload that module
 * 
 * @{
 */

/**
 * \fn class modulehandler(bool a):module("Modulehandler", a, PRIORITY_FIRST){ this->SetDesc("Module Handler"); }
 * \brief Module Handler for navn
 * The Module handler for navn, handles all the module enabling and disabling
 */
class modulehandler:public module
{
public:
  modulehandler(bool a):module("Modulehandler", a, PRIORITY_FIRST){ this->SetDesc("Module Handler"); }
  ModuleReturn run(CommandSource &source, std::vector<Flux::string> &params)
  {
    Flux::string cmd = params.empty()?"":params[0];

    bool request = false;
    Flux::string confPass;
    if (cmd == "modflip")
    {
      Flux::string mod = params.size() == 2 ? params[1]:"";
      if(mod.empty()){
	Send->notice(unick, "Syntax: \2modflip \37module\15");
	return MOD_STOP;
      }
      for (int i = 0; i < (int)moduleList.size(); i++)
      {
	if (mod == moduleList[i]->name && moduleList[i]->priority != PRIORITY_FIRST)
	{
	  moduleList[i]->activated = !moduleList[i]->activated;
	  Send->privmsg(chan, "%s is now %sactivated.", moduleList[i]->name.c_str(), moduleList[i]->activated ? "" : "de-");
	  log("%s used \"flipmob\" to %sactivate module '%s'", unick.c_str(), moduleList[i]->activated ? "" : "de-", moduleList[i]->name.c_str());
	}
      }
    }
    /**************************************************************************************************/
    if(cmd == "modlist")
    {
      Flux::string lpriority = params.size() == 2 ? params[1]:"";
      if(lpriority.empty()){
	goto listall;
      }
      if(lpriority == "low" || lpriority == '1'){
	Send->notice(unick, "Current Low Priority Module list:");
	int count = 0;
	for(int i = 0; i < (int)moduleList.size(); i++)
	{
	  if(moduleList[i]->priority == PRIORITY_LAST){
	    Send->notice(unick, "\2%-16s\2 %s \2[%sactivated]\2.", moduleList[i]->name.c_str(), moduleList[i]->GetDesc().c_str(), moduleList[i]->activated ? "" : "de-");
	    count++;
	  }
	}
	Send->notice(unick, "%i Module%s loaded.", count, count == 1 ? "" : "s");
	log("%s used \"modlist low\" to show %i loaded module%.", unick.c_str(), count, count == 1 ? "" : "s");
      }
      else if(lpriority == "deactivated"){
      Send->notice(unick, "Current Deactivated Modules list:");
      int count = 0;
	for(int i = 0; i < (int)moduleList.size(); i++)
	{
	  if(moduleList[i]->activated == false){
	    Send->notice(unick, "\2%-16s\2 %s \2[%sactivated]\2.", moduleList[i]->name.c_str(), moduleList[i]->GetDesc().c_str(), moduleList[i]->activated ? "" : "de-");
	    count++;
	  }
	}
	Send->notice(unick, "%i Module%s loaded.", count, count == 1 ? "" : "s");
	log("%s used \"modlist low\" to show %i loaded module%.", unick.c_str(), count, count == 1 ? "" : "s");
      }
      else if(lpriority == "activated"){
      Send->notice(unick, "Current Activated Modules list:");
      int count = 0;
	for(int i = 0; i < (int)moduleList.size(); i++)
	{
	  if(moduleList[i]->activated == true){
	    Send->notice(unick, "\2%-16s\2 %s \2[%sactivated]\2.", moduleList[i]->name.c_str(), moduleList[i]->GetDesc().c_str(), moduleList[i]->activated ? "" : "de-");
	    count++;
	  }
	}
	Send->notice(unick, "%i Module%s loaded.", count, count == 1 ? "" : "s");
	log("%s used \"modlist low\" to show %i loaded module%.", unick.c_str(), count, count == 1 ? "" : "s");
      }
      else if(lpriority == "normal" || lpriority == '2'){
	Send->notice(unick, "Current Normal Priority Module list:");
	int count = 0;
	for(int i = 0; i < (int)moduleList.size(); i++)
	{
	  if(moduleList[i]->priority == PRIORITY_DONTCARE){
	    Send->notice(unick, "\2%-16s\2 %s \2[%sactivated]\2.", moduleList[i]->name.c_str(), moduleList[i]->GetDesc().c_str(), moduleList[i]->activated ? "" : "de-");
	    count++;
	  }
	}
	Send->notice(unick, "%i Module%s loaded.", count, count == 1 ? "" : "s");
	log("%s used \"modlist normal\" to show %i loaded module%.", unick.c_str(), count, count == 1 ? "" : "s");
      }
      else if(lpriority == "high" || lpriority == '3'){
	Send->notice(unick, "Current High Priority Module list:");
	int count = 0;
	for(int i = 0; i < (int)moduleList.size(); i++)
	{
	  if(moduleList[i]->priority == PRIORITY_FIRST){
	    Send->notice(unick, "\2%-16s\2 %s \2[%sactivated]\2.", moduleList[i]->name.c_str(), moduleList[i]->GetDesc().c_str(), moduleList[i]->activated ? "" : "de-");
	    count++;
	  }
	}
	Send->notice(unick, "%i Module%s loaded.", count, count == 1 ? "" : "s");
	log("%s used \"modlist high\" to show %i loaded module%.", unick.c_str(), count, count == 1 ? "" : "s");
	
      }else{
	listall:
	int count = 0;
	Send->notice(unick, "Current Module list:");
	for(int i = 0; i < (int)moduleList.size(); i++)
	{
	  Send->notice(unick, "\2%-16s\2 %s \2[%sactivated]\2.", moduleList[i]->name.c_str(), moduleList[i]->GetDesc().c_str(), moduleList[i]->activated ? "" : "de-");
	  count++;
	}
	Send->notice(unick, "%i Module%s loaded.", count, count == 1 ? "" : "s");
	log("%s used \"modlist\" to show %s %i loaded module%.", unick.c_str(), count == 1 ? "one" : "all", count, count == 1 ? "" : "s");
      }
    }
    /***********************************************************************************************************/
    if(cmd == "modunload"){
      Flux::string type = params.size() == 2 ? params[1]:"";
      if(type.empty()){
	Send->notice(unick, "Syntax: \2modunload \37type\15");
      }
      if(type == "all"){
	Send->notice(unick, "Unloading all non-high priority modules.");
	int count = 0;
	for (int i = 0; i < (int)moduleList.size(); i++)
	{
	  if (moduleList[i]->priority < PRIORITY_FIRST) moduleList[i]->activated = false;
	  count++;
	}
	log("%s used \"modunload all\" to unload all non-high priority modules (%i Module%s)", unick.c_str(), count, count == 1 ? "" : "s");
      }
      if(type == "low" || type == '1'){
	Send->notice(unick, "Unloading all low priority modules.");
	int count = 0;
	for (int i = 0; i < (int)moduleList.size(); i++)
	{
	  if (moduleList[i]->priority == PRIORITY_LAST) moduleList[i]->activated = false;
	  count++;
	}
	log("%s used \"modunload low\" to unload all normal priority modules (%i Module%s)", unick.c_str(), count, count == 1 ? "" : "s");
      }
      if(type == "normal" || type == '2'){
	Send->notice(unick, "Unloading all normal priority modules.");
	int count = 0;
	for (int i = 0; i < (int)moduleList.size(); i++)
	{
	  if (moduleList[i]->priority == PRIORITY_DONTCARE) moduleList[i]->activated = false;
	  count++;
	}
	log("%s used \"modunload normal\" to unload all normal priority modules (%i Module%s)", unick.c_str(), count, count == 1 ? "" : "s");
      }
      if((type == "high" || type == '3') && !request){
	request = true;
	confPass = make_pass();
	Send->notice(unick, "\0037WARNING\017: You are requesting an unload of all high priority modules. This cannot be reversed without a restart.");
	Send->notice(unick, "To confirm type \"/msg %s modconfirm %s\"", nick.c_str(), confPass.c_str());
	log("%s requested that all high priority modules be unloaded.", unick.c_str());
      }
    }
    if (cmd == "modconfirm")
    {
      Flux::string pass = params.size() == 2 ? params[1]:"";
      if(pass.empty() || !request){
	Send->notice(unick, "Syntax: \2modconfirm \37password\15");
	Send->notice(unick, "You must have requested to unload all high priority modules first");
	return MOD_STOP;
      }
      Send->notice(unick, "Unloading all high priority modules.");
      int count = 0;
      for (int i = 0; i < (int)moduleList.size(); i++)
	{
	  if (moduleList[i]->priority == PRIORITY_FIRST) moduleList[i]->activated = false;
	  count++;
	}
	log("%s used \"modunload high\" to unload all high priority modules (%i Module%s)", unick.c_str(), count, count == 1 ? "" : "s");
	log("Module's system frozen in current state until next restart, Module Handler disabled!");
    }
    /***********************************************************************************************************/
    if(cmd == "modload"){
      Flux::string priority = params.size() == 2 ? params[1]:"";
      if(priority.empty()){
	Send->notice(unick, "Syntax: \2modload \37priority");
	return MOD_STOP;
      }
      if(priority == "all"){
	Send->notice(unick, "Loading all non-high priority modules.");
	int count = 0;
	for (int i = 0; i < (int)moduleList.size(); i++)
	{
	  if (moduleList[i]->priority < PRIORITY_FIRST) moduleList[i]->activated = true;
	  count++;
	}
	log("%s used \"modload all\" to load all modules (%i Module%s)", unick.c_str(), count, count == 1 ? "" : "s");
      }
      if(priority == '1' || priority == "low"){
	Send->notice(unick, "loading all low priority modules.");
	int count = 0;
	for (int i = 0; i < (int)moduleList.size(); i++)
	{
	  if (moduleList[i]->priority == 1) moduleList[i]->activated = true;
	  count++;
	}
	log("%s used \"modload low\" to load all low priority modules (%i Module%s)", unick.c_str(), count, count == 1 ? "" : "s");
      }
      if(priority == '2' || priority == "normal"){
	Send->notice(unick, "loading all priority 2 modules.");
	int count = 0;
	for (int i = 0; i < (int)moduleList.size(); i++)
	{
	  if (moduleList[i]->priority == 2) moduleList[i]->activated = true;
	  count++;
	}
	log("%s used \"modload normal\" to load all normal priority modules (%i Module%s)", unick.c_str(), count, count == 1 ? "" : "s");
      }
    }
    return MOD_RUN;
  }
};
/**
 * @}
 */
#endif