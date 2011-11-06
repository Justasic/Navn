#include "flux_net_irc.hpp"
#include "xmlfile.h"
XMLFile * xf = new XMLFile("myxml.xml");

class QuickReply : public module
{
public:
  QuickReply(const Flux::string &Name):module(Name)
  {
    this->SetVersion("1.0");
    this->SetAuthor("Lordofsraam");
    this->SetPriority(PRIORITY_LAST);
    Implementation i[] = { I_OnPrivmsg, I_OnNotice };
    ModuleHandler::Attach(i, this, sizeof(i)/sizeof(Implementation));
  }
  void OnPrivmsg(User *u, Channel *c, const std::vector<Flux::string> &params)
  {
    Flux::string msg;
    for(unsigned i=0; i < params.size(); ++i)
      msg += params[i] +' ';
    if (msg.search_ci("!rehash"))
    {
      u->SendMessage("Rehashing xml file...");
      xf = new XMLFile("myxml.xml");
      u->SendMessage("XML file has been rehashed.");
    }
    if (msg.search_ci(xf->Tags["trigger"].Attributes["word"].Value))
    {
      c->SendMessage(xf->Tags["trigger"].Content);
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