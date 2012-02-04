//#include "includes.h"
//#include "flux_net_irc.hpp"
//using namespace std;
//using namespace flux_net_irc;

/*This module setup needs serious work!
 * Justasic will work on it but its going to be hard with los's module class.
 */
/*
class dummy{

public:
  dummy(string n, bool a):module(n,a){}
  
  void run(Socket &sock, string rply, irc_string *reply){
    
    if (reply->said("testing testing")){
      sock << privmsg(reply->channel,"I hear you!");
    }
    
  }
};

class ModDummy : public module
{
 dummy dumdum;
 
public:
  ModDummy():module("testmodule",true);
  this->author = "Justasic";
  
  dumdum.run
  
  
}

MODULE_HOOK(ModDummy)
*/