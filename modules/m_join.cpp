#include "flux_net_irc.hpp"

/*This module setup needs serious work!
 * Justasic will work on it but its going to be hard with los's module class.
 */
class CommandJoin : public Command
{
public:
  CommandJoin() : Command("JOIN", 1, 1)
  {
   this->SetDesc("Joins a channel");
   this->SetSyntax("\37channel\37");
  }
  void Run(CommandSource &source, const std::vector<Flux::string> &params)
  {
    User *u = source.u;
    Flux::string chan = params[1];
    if(!u->IsOwner()){
      source.Reply(ACCESS_DENIED);
      Log(u) << "attempted to make the bot join " << chan;
      return;
    }
    if(!IsValidChannel(chan))
      source.Reply(CHANNEL_X_INVALID, chan.c_str());
    else{
      Log(u) << "made the bot join " << chan;
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
class CommandPart : public Command
{
public:
  CommandPart():Command("PART", 1,1)
  {
    this->SetDesc("Part a channel");
    this->SetSyntax("\37channel\37");
  }
  void Run(CommandSource &source, const std::vector<Flux::string> &params)
  {
    Flux::string chan = params[1];
    User *u = source.u;
    if(!u->IsOwner()){
     source.Reply(ACCESS_DENIED);
     Log(u) << "attempted to make bot part " << chan;
     return;
    }
    if(!IsValidChannel(chan))
     source.Reply(CHANNEL_X_INVALID, chan.c_str());
    else{
      Channel *c = findchannel(chan);
      if(c)
	c->SendPart();
      else
	source.Reply("I am not in channel \2%s\2", chan.c_str());
      Log(u) << "made the bot part " << chan;
    }
  }
};
class Join : public module
{
  CommandJoin cmdjoin;
  CommandPart cmdpart;
public:
  Join(const Flux::string &Name):module(Name)
  { 
    this->AddCommand(&cmdjoin);
    this->AddCommand(&cmdpart);
    this->SetVersion(VERSION);
    this->SetAuthor("Justasic");
  }
};

MODULE_HOOK(Join)