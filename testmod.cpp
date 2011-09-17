#include "flux_net_irc.hpp"


/*This module setup needs serious work!
 * Justasic will work on it but its going to be hard with los's module class.
 */

/*class dummy{

public:
  dummy(string n, bool a):module(n,a){}
  
  void run(Socket &sock, string rply, irc_string *reply){
    
    if (reply->said("testing testing")){
      sock << privmsg(reply->channel,"I hear you!");
    }
    
  }
};*/

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
    Channel *c = source.c;
    Flux::string cmd = params.empty()?"":params[0];
    if(!u)
    {
      return MOD_STOP;
    }
    if(source.message == "usertest")
    {
     source.Reply("TEST!");
    }
    if(source.message == "chantest")
    {
     c->SendMessage("TADA!");
    }
    if (source.message == "testing testing")
    {
      source.Reply("I hear you!\nYES I DO!");
    }
    if(source.message == "!startthread")
    {
      //Bacon strips;
      //ThreadHandler::RunThread(strips);
    }
   return MOD_RUN; 
  }
};

MODULE_HOOK(dummy)
