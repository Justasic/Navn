#include "flux_net_irc.hpp"

class encyclopedia : public module
{

public:
  encyclopedia():module("Encyclopedia", PRIORITY_LAST)
  { 
    this->SetAuthor("Lordofsraam");
    this->SetVersion(VERSION);
    ModuleHandler::Attach(I_OnPrivmsg, this);
  }
  void OnPrivmsg(User *u, Channel *c, const std::vector<Flux::string> &params)
  {
    Flux::string msg;
    for(unsigned i=0; i < params.size(); ++i)
      msg += params[i]+' ';
      
    Flux::string sn = "SEND_NOTHING";

    Flux::string cmd = params.empty()?"":params[0];

    Flux::string str = "python brain.py "+msg;
    str = str.replace_all_cs("&","");
    str = str.replace_all_cs("|","");
    Flux::string information = execute(str.c_str());
    information.trim();
    if (information != sn)
    {
      u->SendMessage(information);
    }
  }
};
MODULE_HOOK(encyclopedia)