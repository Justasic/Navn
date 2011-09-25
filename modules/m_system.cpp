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
class CommandRestart : public Command
{
public:
  CommandRestart():Command("RESTART", 1, 1)
  {
   this->SetDesc("Restarts the bot");
   this->SetSyntax("Restart \37reason\37");
  }
  void Run(CommandSource &source, const std::vector<Flux::string> &params)
  {
    if(source.u->IsOwner()){
      Send->command->quit("Restarting..");
      restart("Restarting..");
    }else{
      source.Reply(ACCESS_DENIED);
    }
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

class CommandQuit : public Command
{
public:
  CommandQuit():Command("QUIT", 1,2)
  {
    this->SetDesc("Quits the bot from IRC");
    this->SetSyntax("QUIT \37randompass\37");
  }
  void Run(CommandSource &source, const std::vector<Flux::string> &params)
  {
    User *u = source.u;
    Flux::string pass = params.size() == 2 ? params[1] : "";
    if(pass.empty()){
     this->SendSyntax(source);
     return;
    }
    if(pass == password || pass == Config->UserPass){
      source.Reply("Quitting..");
      log(LOG_NORMAL, "%s quit the bot with password: \"%s\"", u->nick.c_str(), password.c_str());
      Send->command->quit("Requested From \2%s\17. Pass: \00320%s\017", u->nick.c_str(), password.c_str());
      quitting = true;
    }else{
      source.Reply(ACCESS_DENIED);
      log(LOG_NORMAL, "%s attempted to change ownership of the bot", u->nick.c_str());
    }
  }
};

class m_system : public module
{
  CommandChown cmdchown;
  CommandKick cmdkick;
  CommandRehash cmdrehash;
  CommandQuit cmdquit;
  CommandRestart cmdrestart;
public:
  m_system():module("System", PRIORITY_DONTCARE)
  {
    this->AddCommand(&cmdrehash);
    this->AddCommand(&cmdkick);
    this->AddCommand(&cmdchown);
    this->AddCommand(&cmdquit);
    this->AddCommand(&cmdrestart);
    Implementation i[] = { I_OnNumeric, I_OnJoin, I_OnKick };
    ModuleHandler::Attach(i, this, sizeof(i)/sizeof(Implementation));
    this->SetAuthor("Justasic");
  }
  void OnNumeric(int i)
  {
    if((i == 4)){
      Send->command->mode(Config->BotNick, "+B");
      Channel *c = new Channel(Config->Channel);
      c->SendJoin();
      c->SendMessage(welcome_msg, Config->BotNick.c_str(), Config->BotNick.c_str());
      if(!Config->OperatorAccount.empty() || !Config->OperatorPass.empty()){
	Send->command->oper(Config->OperatorAccount, Config->OperatorPass);
	IsOper = true;
	Send->o = new Oper();
      }
      if(!Config->ServicesAccount.empty() || !Config->ServicesPass.empty()){
	Send->privmsg("NickServ", "identify %s %s", Config->ServicesAccount.c_str(), Config->ServicesPass.c_str());
      }
      log(LOG_NORMAL, "Successfully connected to the server \"%s:%s\" Master Channel: %s", Config->Server.c_str(), Config->Port.c_str(), Config->Channel.c_str());
    }
    if((i == 433)){
      Config->BotNick.push_back('_');
      Send->command->nick(Config->BotNick);
    }
    if((i == 376))
    {
     std::cout << "\033[22;31mStarted with PID \033[22;32m" << getpid() << "\033[22;36m" << nl;
      std::cout << "\033[22;34mSession Password: \033[01;32m"+password+"\033[22;36m"<<nl;
      Send->notice(Config->Owner, "The randomly generated password is: "+password);
      started = true; 
    }
  }
  void OnJoin(User *u, Channel *c)
  {
    u->SendMessage("Welcome %s to %s. Type !time for time or \"/msg %s help\" for help on more commands.", u->nick.c_str(),
		   c->name.c_str(), Config->BotNick.c_str());
    log(LOG_NORMAL, "%s joined %s", u->nick.c_str(), c->name.c_str());
  }
  void OnKick(User *u, Channel *c, const Flux::string &reason)
  {
     if(u->nick == Config->BotNick)
     {
       log(LOG_NORMAL, "%s kicked me from %s (%s)", u->nick.c_str(), c->name.c_str(), reason.c_str());
      c->SendJoin(); 
     }
  }
};

MODULE_HOOK(m_system)