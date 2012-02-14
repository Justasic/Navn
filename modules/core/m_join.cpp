/* Navn IRC bot -- Join-part management for the bot
 * 
 * (C) 2011-2012 Flux-Net
 * Contact us at Dev@Flux-Net.net
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Anope by The Anope Team.
 */
#include "flux_net_irc.hpp"

/*This module setup needs serious work!
 * Justasic will work on it but its going to be hard with los's module class.
 */
class CommandJoin : public Command
{
public:
  CommandJoin(module *m) : Command(m, "JOIN", C_PRIVATE, 1, 1)
  {
   this->SetDesc("Joins a channel");
   this->SetSyntax("\37channel\37");
  }
  void Run(CommandSource &source, const Flux::vector &params)
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
	ircproto->join(chan);
	ircproto->privmsg(chan, welcome_msg, Config->BotNick.c_str(), Config->BotNick.c_str());
      }
    }
  }
  bool OnHelp(CommandSource &source, const Flux::string &nill)
  {
    this->SendSyntax(source);
    source.Reply(" ");
    source.Reply("This command makes the bot join a channel.\n"
		 "You must be the bots owner to use this command.");
    return true;
  }
};
class CommandPart : public Command
{
public:
  CommandPart(module *m):Command(m, "PART", C_PRIVATE, 1,1)
  {
    this->SetDesc("Part a channel");
    this->SetSyntax("\37channel\37");
  }
  void Run(CommandSource &source, const Flux::vector &params)
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
  bool OnHelp(CommandSource &source, const Flux::string &nill)
  {
    this->SendSyntax(source);
    source.Reply(" ");
    source.Reply("This command makes the bot part a channel.\n"
		 "You must be the bots owner to use this command.");
    return true;
  }
};
class Join : public module
{
  CommandJoin cmdjoin;
  CommandPart cmdpart;
public:
  Join(const Flux::string &Name):module(Name), cmdjoin(this), cmdpart(this)
  { 
    this->SetVersion(VERSION);
    this->SetAuthor("Justasic");
  }
};

MODULE_HOOK(Join)