#include "flux_net_irc.hpp"

class encyclopedia : public module
{

private:
  Flux::string query;
  void Sanitize(Flux::string &victim)
  {
    victim = victim.replace_all_cs("&","");
    victim = victim.replace_all_cs("|","");
    victim = victim.replace_all_cs(">","");
    victim = victim.replace_all_cs("<","");
    victim = victim.replace_all_cs("!","");
    victim = victim.replace_all_cs("(","");
    victim = victim.replace_all_cs(")","");
    victim = victim.replace_all_cs("*","");
    victim = victim.replace_all_cs("{","");
    victim = victim.replace_all_cs("}","");
    victim = victim.replace_all_cs("`","");
    victim = victim.replace_all_cs("\"","");
    victim = victim.replace_all_cs("\'","");
    victim = victim.replace_all_cs(".","");
    victim = victim.replace_all_cs("?","");
  }
  void Brain(User *u, Flux::string q)
  {
    Sanitize(q);
    Flux::string str = "python brain.py "+q;
    Flux::string information = execute(str.c_str());
    information.trim();
    u->SendMessage(information);
  }
  void SetQuery(unsigned n, const std::vector<Flux::string> &params)
  {
    query.clear();
    if (n < params.size())
    {
      for(unsigned i=n; i < params.size(); ++i)
      {
	query += params[i]+' ';
      }
    }
  }

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
      SetQuery(1, params);
      Brain(u,query);
    }
    if(msg.search(Config->BotNick+", what do you know about "))
    {
      SetQuery(6, params);
      Brain(u, query);
    }
    else if(msg.search(Config->BotNick+", what is a ") ^ msg.search(Config->BotNick+", what is the") ^ msg.search(Config->BotNick+", tell me about ") ^ msg.search(Config->BotNick+", who are the ") ^ msg.search(Config->BotNick+", what is an "))
    {
      SetQuery(4, params);
      Brain(u, query);
    }
    else if(msg.search(Config->BotNick+", what is ") ^ msg.search(Config->BotNick+", what are ") ^ msg.search(Config->BotNick+", who is ") ^ msg.search(Config->BotNick+", what's a ") ^ msg.search(Config->BotNick+", what's an "))
    {
      SetQuery(3, params);
      Brain(u, query);
    }
    else if(msg.search(Config->BotNick+", tell me what you know about "))
    {
      SetQuery(7, params);
      Brain(u, query);
    }
  }
};
MODULE_HOOK(encyclopedia)