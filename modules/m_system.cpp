#include "flux_net_irc.hpp"

struct sysinfo sys_info;
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

class CommandNick : public Command
{
public:
  CommandNick():Command("NICK", 1, 2)
  {
    this->SetDesc("Change the bots nickname");
  }
  void Run(CommandSource &source, const std::vector<Flux::string> &params)
  {
    User *u = source.u;
    Flux::string newnick = params.size() == 2 ? params[1]:"";
    if(newnick.empty()){
     this->SendSyntax(source);
     return;
    }
    if(!u->IsOwner()){
     source.Reply(ACCESS_DENIED);
     return;
    }
    for(unsigned i = 0, end = newnick.length(); i < end; i++)
      if(!isvalidnick(newnick[i])){
	source.Reply("\2%s\2 is an invalid nickname.");
	Config->BotNick = newnick;
      }
      Send->command->nick(newnick);
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

class CommandTopic: public Command
{
public:
  CommandTopic():Command("TOPIC", 2, 3)
  {
    this->SetDesc("Set the topic on a channel");
    this->SetSyntax("TOPIC \37channel\37 [\37topic\37]");
  }
  void Run(CommandSource &source, const std::vector<Flux::string> &params)
  {
    User *u = source.u;
    if(!u->IsOwner()){
      source.Reply(ACCESS_DENIED);
      return;
    }
    if(params.size() < 3){
     this->SendSyntax(source);
     return;
    }
    Flux::string tchan = params[1];
    if(!IsValidChannel(tchan)){
      source.Reply(CHANNEL_X_INVALID, tchan.c_str());
      return;
    }
    Channel *ch = findchannel(tchan);
    if(!ch){
      source.Reply("I am not in channel \2%s\2", tchan.c_str());
      return;
    }
    Flux::string msg = source.message;
    msg = msg.erase(0,6);
    msg = msg.erase(msg.find_first_of(tchan), tchan.size());
    msg.trim();
    ch->ChangeTopic(msg);
    std::fstream topic;
    topic.open("topic.tmp", std::fstream::in | std::fstream::out | std::fstream::app);
    if(!topic.is_open()){
	    source.Reply("Unable to write topic temp file");
	    log(LOG_NORMAL, "%s used /msg %s topic to change %s's topic to \"%s\" but could not write to topic temp file '%s'", 
		u->nick.c_str(), Config->BotNick.c_str(), tchan.c_str(), msg.c_str(), "topic.tmp");
    }else{
	    topic << "<?xml version=\"1.0\" ?><rss version=\"2.0\"><channel><topic> " << tchan << " Topic: " << strip(msg) << " </topic></channel></rss>" << std::endl;
	    system("sh ftp.sh");
    }
    log(LOG_NORMAL, "%s used Da_Goats !topic command to change the topin in %s to \"%s\"",u->nick.c_str(), tchan.c_str(), strip(msg).c_str());
  }
};

class CommandPID: public Command
{
public:
  CommandPID():Command("PID", 0,0)
  {
    this->SetDesc("Gets the bots Process ID");
  }
  void Run(CommandSource &source, const std::vector<Flux::string> &params)
  {
    User *u = source.u;
    if(u->IsOwner()){
      source.Reply("My PID is: \2%i\2", (int)getpid()); 
      log(LOG_NORMAL, "%s used pid function to get PID %i", u->nick.c_str(), getpid());
    }else{
      source.Reply(ACCESS_DENIED);
    }
  }
};

class CommandPass: public Command
{
public:
  CommandPass():Command("PASS", 0,0)
  {
    this->SetDesc("Gets the bots Random Password");
  }
  void Run(CommandSource &source, const std::vector<Flux::string> &params)
  {
    User *u = source.u;
    if (u->IsOwner()){
      source.Reply("The password is:\2 %s", password.c_str());
      log(LOG_NORMAL, "%s requested the navn quit password: %s", u->nick.c_str(), password.c_str());
    }else{
      source.Reply(ACCESS_DENIED);
      log(LOG_NORMAL, "%s attempted to request the navn quit password.", u->nick.c_str());
    }
  }
};

class CommandStats: public Command
{
public:
  CommandStats():Command("STATS", 0, 0)
  {
    this->SetDesc("Shows system stats");
  }
  void Run(CommandSource &source, const std::vector<Flux::string> &params)
  {
    int days, hours, mins;
#ifndef _WIN32
    if(sysinfo(&sys_info) != 0){//now here Justasic got pissed because c strings suck ass
      Flux::string fuckingshit = Flux::string("sys_info Error: ") + strerror(errno);
      throw ModuleException(fuckingshit.c_str());
    }
    struct utsname uts;
    if(uname(&uts) < 0){
      Flux::string fuckingshit = Flux::string("uname() Error: ") + strerror(errno);
      throw ModuleException(fuckingshit.c_str());
    }

    // Uptime
    days = sys_info.uptime / 86400;
    hours = (sys_info.uptime / 3600) - (days * 24);
    mins = (sys_info.uptime / 60) - (days * 1440) - (hours * 60);

    source.Reply("Uptime: %d days, %d hours, %d minutes, %ld seconds",
	  days, hours, mins, sys_info.uptime % 60);

    // Load Averages for 1,5 and 15 minutes
    source.Reply("Load Avgs: 1min(%ld) 5min(%ld) 15min(%ld)",
		    sys_info.loads[0], sys_info.loads[1], sys_info.loads[2]);

    // Total and free ram.
    source.Reply("Total Ram: %ldk\tFree: %ldk", sys_info.totalram / 1024,
			    sys_info.freeram / 1024);

    // Shared and buffered ram.
    source.Reply("Shared Ram: %ldk", sys_info.sharedram / 1024);
    source.Reply("Buffered Ram: %ldk", sys_info.bufferram / 1024);

    // Swap space
    source.Reply("Total Swap: %ldk\tFree: %ldk", sys_info.totalswap / 1024,
				sys_info.freeswap / 1024);

    // Number of processes currently running.
    source.Reply("Number of processes: %d", sys_info.procs);
    source.Reply(" ");
    source.Reply("System Name: %s\tRelease: %s %s\tMachine: %s", uts.nodename, uts.sysname, uts.release, uts.machine);
    source.Reply("System Version: %s", uts.version);

    source.Reply(strip(execute("grep 'model name' /proc/cpuinfo")));
#else
    source.Reply("This is currently not avalable on windows syetems, sorry.");
#endif
    User *u = source.u;
    log(LOG_NORMAL, "%s used stats command", u->nick.c_str());
  }
};
class m_system : public module
{
  CommandChown cmdchown;
  CommandKick cmdkick;
  CommandRehash cmdrehash;
  CommandQuit cmdquit;
  CommandRestart cmdrestart;
  CommandTopic topic;
  CommandStats stats;
  CommandNick nick;
  CommandPID pid;
  CommandPass pass;
public:
  m_system():module("System", PRIORITY_DONTCARE)
  {
    this->AddCommand(&cmdrehash);//So many commands! .-.
    this->AddCommand(&cmdkick);
    this->AddCommand(&cmdchown);
    this->AddCommand(&cmdquit);
    this->AddCommand(&cmdrestart);
    this->AddCommand(&nick);
    this->AddCommand(&stats);
    this->AddCommand(&topic);
    this->AddCommand(&pid);
    this->AddCommand(&pass);
    
    Implementation i[] = { I_OnNumeric, I_OnJoin, I_OnKick, I_OnNotice };
    ModuleHandler::Attach(i, this, sizeof(i)/sizeof(Implementation));
    this->SetAuthor("Justasic");
    this->SetVersion(VERSION);
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
      log(LOG_TERMINAL, "\033[22;31mStarted with PID \033[22;32m%i\033[22;36m", (int)getpid());
      log(LOG_TERMINAL, "\033[22;34mSession Password: \033[01;32m%s\033[22;36m", password.c_str());
      Send->notice(Config->Owner, "The randomly generated password is: %s", password.c_str());
      started = true; 
    }
  }
  void OnJoin(User *u, Channel *c)
  {
    u->SendMessage("Welcome %s to %s. Type !time for time or \"/msg %s help\" for help on more commands.", u->nick.c_str(),
		   c->name.c_str(), Config->BotNick.c_str());
    log(LOG_NORMAL, "%s joined %s", u->nick.c_str(), c->name.c_str());
  }
  void OnKick(User *u, User *kickee, Channel *c, const Flux::string &reason)
  {
    std::cout << "User: " << u << " Kickee: " << kickee << " Channel: " << c << " Reason: " << reason << std::endl;
     if(kickee && kickee->nick.equals_ci(Config->BotNick))
     {
       log(LOG_NORMAL, "%s kicked me from %s (%s)", u->nick.c_str(), c->name.c_str(), reason.c_str());
	c->SendJoin(); 
     }
  }
  void OnNotice(User *u, const std::vector<Flux::string> &params)
  {
    Flux::string msg;
    for(unsigned i=0; i < params.size(); ++i){
      msg += params[i];
      msg.AddSpace();
    }
    msg.trim(); // Auto-Identify
    if(irc_string::said(msg, "This nickname is registered and protected. If it is your")){
      if((!Config->ServicesPass.empty() || !Config->ServicesAccount.empty()) && u->nick == "NickServ"){
	u->SendPrivmsg("identify %s %s", Config->ServicesAccount.c_str(), Config->ServicesPass.c_str());
	log(LOG_NORMAL, "Identified to NickServ with account \"%s\"", Config->ServicesAccount.c_str());
      }
    }
  }
  void OnNickChange(User *u, const Flux::string &newnick)
  {
   /*if(source.command == "NICK"){
    IsoHost *Host = new IsoHost(u->fullhost);
    Flux::string newnick = Host->nick;
    if (newnick == nick)
      nick = newnick;
    if(newnick == owner_nick)
      owner_nick = newnick;
    delete Host;
  }*/ 
  }
};

MODULE_HOOK(m_system)