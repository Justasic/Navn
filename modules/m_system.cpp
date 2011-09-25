#include "flux_net_irc.hpp"

class CommandRehash : public Command
{
public:
  CommandRehash():Command("REHASH", 0, 0)
  {
    this->SetDesc("Rehashes the config file");
    this->SetSyntax("REHASH");
  }
  void Run(CommandSource &source, const std::vector<Flux::string> &params)
  {
    User *u = source.u;
   if(!u->IsOwner())
      source.Reply(ACCESS_DENIED);
   else{
     source.Reply("Rehashing Configuration file");
     log(LOG_NORMAL, "%s rehashed config file.", u->nick.c_str());
     Rehash();
   }
   log(LOG_NORMAL, "%s rehashed config file.", u->nick.c_str());
  }
};

class CommandKick : public Command
{
public:
  CommandKick():Command("KICK", 1, 3)
  {
    this->SetDesc("Kick a user from the channel");
    this->SetSyntax("KICK channel \37nick\15");
  }
  void Run(CommandSource &source, const std::vector<Flux::string> &params)
  {
    User *u = source.u;
    if(u->IsOwner()){
      Flux::string kickchan = params.size() == 3 ?params[1]:"";
      Flux::string kickee = params.size() == 3 ?params[2]:"";
      if(kickee.empty() || kickchan.empty()){
	this->SendSyntax(source);
	return;
      }
      if(!IsValidChannel(kickchan)){
	source.Reply(CHANNEL_X_INVALID, kickchan.c_str()); 
	return;
      }
      Channel *c = findchannel(kickchan);
      if(!c){
	source.Reply("I am not in channel \2%s\2", kickchan.c_str());
	return;
      }
      c->kick(kickee, "Kick from %s", u->nick.c_str());
    }else{
      source.Reply(ACCESS_DENIED);
    }
  }
};

class CommandChown : public Command
{
public:
  CommandChown():Command("CHOWN", 1, 2)
  {
    this->SetDesc("Change ownership over the bot");
    this->SetSyntax("CHOWN \37owner");
  }
  void Run(CommandSource &source, const std::vector<Flux::string> &params)
  {
    source.Reply("This command is broken!");
  }
};

class m_system : public module
{
  CommandChown cmdchown;
  CommandKick cmdkick;
  CommandRehash cmdrehash;
public:
  m_system():module("System", PRIORITY_DONTCARE)
  {
    this->AddCommand(&cmdrehash);
    this->AddCommand(&cmdkick);
    this->AddCommand(&cmdchown);
    this->SetAuthor("Justasic");
  }
};

MODULE_HOOK(m_system)