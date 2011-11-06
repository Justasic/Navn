#include "flux_net_irc.hpp"

XMLFile * xf = new XMLFile("myxml.xml");

class CommandSimple : public Command
{
public:
  CommandSimple():Command(xf->Tags["trigger"].Attributes["word"].Value)
  {
    this->SetDesc("Pulls from the XML file.");
  }
  void Run(CommandSource &source, const std::vector<Flux::string> &params)
  {
    source.Reply(xf->Tags["trigger"].Content);
  }
};

class QuickReply : public module
{
  CommandSimple s;
public:
  QuickReply(const Flux::string &Name):module(Name)
  {
    this->SetVersion("1.0");
    this->SetAuthor("Lordofsraam");
    this->AddCommand(&s);
    this->SetPriority(PRIORITY_LAST);
    Implementation i[] = { I_OnPrivmsg, I_OnNotice };
    ModuleHandler::Attach(i, this, sizeof(i)/sizeof(Implementation));
  }
  void OnPrivmsg(User *u, const std::vector<Flux::string> &params)
  {
    Flux::string msg;
    for(unsigned i=0; i < params.size(); ++i)
      msg += params[i] +' ';
    if(msg.search_ci("!rehash"))
    {
      u->SendMessage("Rehashing xml file...");
      xf = new XMLFile("myxml.xml");
      u->SendMessage("XML file has been rehashed.");
    }
  }
  void OnNotice(User *u, const std::vector<Flux::string> &params)
  {
    Flux::string msg;
    for(unsigned i=0; i < params.size(); ++i)
      msg += params[i] +' ';
  }
};
MODULE_HOOK(QuickReply)