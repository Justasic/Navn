#include "flux_net_irc.hpp"

/*This module setup needs serious work!
 * Justasic will work on it but its going to be hard with los's module class.
 */
class CommandJoin : public Command
{
public:
  CommandJoin() : Command("JOIN", 1, 2)
  {
   this->SetDesc("Joins a channel");
   this->SetSyntax("\2JOIN \37channel\37\2");
  }
  void Run(CommandSource &source, const std::vector<Flux::string> &params)
  {
    if(params.size() != 2)
      return;
    User *u = source.u;
    Flux::string chan = params.size() == 2 ? params[1] : "";
    if(!u->IsOwner()){
      source.Reply(ACCESS_DENIED);
      return;
    }
    if(!IsValidChannel(chan)){
      source.Reply("Channel \2%s\2 is not a valad channel.", chan.c_str());
      log(LOG_NORMAL, "%s attempted to make bot join %s", u->nick.c_str(), chan.c_str());
    }else{
      log(LOG_NORMAL, "%s made the bot join %s", u->nick.c_str(), chan.c_str());
      Channel *c = findchannel(chan);
      if(c){
	c->SendJoin();
	c->SendMessage(welcome_msg, Config->BotNick.c_str(), Config->BotNick.c_str());
      }else{
	Send->command->join(chan);
	Send->privmsg(chan, welcome_msg, Config->BotNick.c_str(), Config->BotNick.c_str());
      }
      
    }
  }
  
};
class Join : public module
{
  CommandJoin cmdjoin;
public:
  Join():module("JOIN", PRIORITY_DONTCARE)
  { 
    this->AddCommand(&cmdjoin);
  }
};

MODULE_HOOK(Join)