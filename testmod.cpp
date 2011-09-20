#include "flux_net_irc.hpp"

/*This module setup needs serious work!
 * Justasic will work on it but its going to be hard with los's module class.
 */

class dummy : public module
{
public:
  dummy(bool a):module("Dummy_bin", a, PRIORITY_LAST)
  { 
    this->SetDesc("Example Dummy module, in binary form"); 
  }
  
  ModuleReturn run(CommandSource &source, std::vector<Flux::string> &params)
  {
    User *u = source.u;
    Flux::string cmd = params.empty()?"":params[0];
    if(!u){
      return MOD_STOP;
    }
    if(source.message == "moduletest"){
     source.Reply("TEST!");
     printf("YAAY!\n");
    }
   return MOD_RUN; 
  }
};

MODULE_HOOK(dummy)
