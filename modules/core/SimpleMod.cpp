#include "flux_net_irc.hpp"

class CommandSimple : public Command
{
public:
  CommandSimple(module *m):Command(m, "HI", C_PRIVATE)
  {
    this->SetDesc("Says Hi");
  }
  void Run(CommandSource &source, const Flux::vector &params)
  {
    source.Reply("HI!");
  }
  bool OnHelp(CommandSource &source, const Flux::string &nill)
  {
    this->SendSyntax(source);
    source.Reply(" ");
    source.Reply("This is the help for the simple module\n"
		 "This is line 2 of the help in the simple module");
    return true;
  }
};

class SimpleMod : public module
{
  CommandSimple s;
public:
  SimpleMod(const Flux::string &Name):module(Name), s(this)
  {
    this->SetVersion("1.0");
    this->SetAuthor("Justasic & Lordofsraam");
    this->SetPriority(PRIORITY_LAST);
    Implementation i[] = { I_OnPrivmsg, I_OnNotice };
    ModuleHandler::Attach(i, this, sizeof(i)/sizeof(Implementation));
  }
  void OnPrivmsg(User *u, const Flux::vector &params)
  {
    Flux::string msg;
    for(unsigned i=0; i < params.size(); ++i)
      msg += params[i] +' ';
    if(msg.search("I am a very long command"))
    {
      Log(LOG_TERMINAL) << "PRIVMSG: " << msg;
    }
  }
  void OnNotice(User *u, const Flux::vector &params)
  {
    Flux::string msg;
    for(unsigned i=0; i < params.size(); ++i)
      msg += params[i] +' ';
    if(msg.search("I am a very long notice"))
    {
      Log(LOG_TERMINAL) << "NOTICE: " << msg;
    }
  }
};
MODULE_HOOK(SimpleMod)