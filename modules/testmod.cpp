#include "flux_net_irc.hpp"

/*This module setup needs serious work!
 * Justasic will work on it but its going to be hard with los's module class.
 */
class commanddummy : public Command
{
public:
  commanddummy() : Command("ModuleTest", 1, 1)
  {
   this->SetDesc("Test for the modules");
   this->SetSyntax("\2ModuleTest\2");
  }
  void Run(CommandSource &source, const std::vector<Flux::string> &params)
  {
    source.Reply("YAY!");
  }
  
};
class dummy : public module
{
  commanddummy cmddmy;
public:
  dummy():module("Dummy_bin", PRIORITY_LAST)
  { 
    this->AddCommand(&cmddmy);
    this->SetAuthor("Justasic");
    Implementation i[] = { I_OnReload, I_OnPrivmsg };
    ModuleHandler::Attach(i, this, sizeof(i) / sizeof(Implementation));
  }
  void OnReload(bool)
  {
   printf("RELOAD! YAAY!\n"); 
  }
  void OnPrivmsg(User *u, Channel *c, const std::vector<Flux::string> &params)
  {
    Flux::string msg;
    for(unsigned i=0; i < params.size(); ++i){
      msg += params[i];
      msg.AddSpace();
    }
   printf("Channel message: %s\n", msg.c_str()); 
  }
  
};

MODULE_HOOK(dummy)