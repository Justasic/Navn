#include "flux_net_irc.hpp"

class CommandSimple : public Command
{
public:
  CommandSimple():Command("HI")
  {
    this->SetDesc("Says Hi");
  }
  void Run(CommandSource &source, const std::vector<Flux::string> &params)
  {
    source.Reply("HI!");
  }
};

class SimpleMod : public module
{
  CommandSimple s;
public:
  SimpleMod(const Flux::string &Name):module(Name)
  {
    this->SetVersion("1.0");
    this->SetAuthor("Justasic & Lordofsraam");
    this->AddCommand(&s);
    this->SetPriority(PRIORITY_LAST);
    Implementation i[] = { I_OnPrivmsg, I_OnNotice } 
    ModuleHandler::Attach(i, this, sizeof(i)/sizeof(Implementation));
  }
  void OnPrivmsg(User *u, const std::vector<Flux::string> &params)
  {
    Flux::string msg;
    for(unsigned i=0; i < params.size(), ++i)
      msg += params[i] +' ';
    if(irc_string::said(msg, "I am a very long command")){
      log(LOG_TERMINAL, "PRIVMSG: %s", msg.c_str());
    }
  }
  void OnNotice(User *u, const std::vector<Flux::string> &params)
  {
    Flux::string msg;
    for(unsigned i=0; i < params.size(), ++i)
      msg += params[i] +' ';
    if(irc_string::said(msg, "I am a very long notice")){
      log(LOG_TERMINAL, "NOTICE: %s", msg.c_str());
    }
  }
};
MODULE_HOOK(SimpleMod)