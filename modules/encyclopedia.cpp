#include "flux_net_irc.hpp"

class encyclopedia : public module
{

public:
  encyclopedia(const Flux::string &Name):module(Name)
  {
    this->SetAuthor("Lordofsraam");
    this->SetVersion(VERSION);
    this->SetPriority(PRIORITY_LAST);
    ModuleHandler::Attach(I_OnPrivmsg, this);
  }
  void OnPrivmsg(User *u, Channel *c, const std::vector<Flux::string> &params)
  {
    //std::vector<Flux::string> MessageParams = StringVector(params, ' ');
    Flux::string msg;
    for(unsigned i=0; i < params.size(); ++i)
      msg += params[i]+' ';

    Flux::string cmd = params.empty()?"":params[0];

    if(cmd.equals_ci("!encyclopedia"))
    {
      Flux::string sn = "SEND_NOTHING";

      Flux::string query;
      for(unsigned i=1; i < params.size(); ++i) query += params[i]+' ';

      query = query.replace_all_cs("&","");
      query = query.replace_all_cs("|","");
      query = query.replace_all_cs(">","");
      query = query.replace_all_cs("<","");
      query = query.replace_all_cs("!","");
      query = query.replace_all_cs("(","");
      query = query.replace_all_cs(")","");
      query = query.replace_all_cs("*","");
      query = query.replace_all_cs("{","");
      query = query.replace_all_cs("}","");
      query = query.replace_all_cs("`","");
      query = query.replace_all_cs("\"","");
      query = query.replace_all_cs("\'","");
      query = query.replace_all_cs(".","");

      Flux::string str = "python brain.py "+query;
      Flux::string information = execute(str.c_str());
      information.trim();
      if (information != sn)
      {
	u->SendMessage(information);
      }
    }
  }
};
MODULE_HOOK(encyclopedia)