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
    Flux::string chann = params.size() == 2 ? params[1] : "";
    if(!IsValidChannel(chann)){
      source.Reply("Channel \2%s\2 is not a valad channel.", chann.c_str());
      log(LOG_NORMAL, "%s attempted to make bot join %s", u->nick.c_str(), chann.c_str());
    }else{
      log(LOG_NORMAL, "%s made the bot join %s", u->nick.c_str(), chann.c_str());
      Channel *chan = findchannel(chann);
      if(chan){
	chan->SendJoin();
	chan->SendMessage(welcome_msg, Config->BotNick.c_str(), Config->BotNick.c_str());
      }else{
	Send->command->join(chann);
	Send->privmsg(chann, welcome_msg, Config->BotNick.c_str(), Config->BotNick.c_str());
      }
      
    }
  }
  
};
class Join : public module
{
  CommandJoin cmdjoin;
public:
  Join(bool a):module("JOIN", a, PRIORITY_DONTCARE)
  { 
    this->SetDesc("By Justasic");
    this->AddCommand(&cmdjoin);
  }
};

MODULE_HOOK(Join)