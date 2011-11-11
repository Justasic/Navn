#include "flux_net_irc.hpp"

struct sysinfo sys_info;
class CommandRehash : public Command
{
public:
  CommandRehash():Command("REHASH", 0, 0)
  {
    this->SetDesc("Rehashes the config file");
  }
  void Run(CommandSource &source, const std::vector<Flux::string> &params)
  {
    User *u = source.u;
   if(!u->IsOwner())
      source.Reply(ACCESS_DENIED);
   else{
     source.Reply("Rehashing Configuration file");
     Log(u, this) << "to rehash the configuration";
     Rehash();
   }
  }
};

class CommandNick : public Command
{
public:
  CommandNick():Command("NICK", 1, 1)
  {
    this->SetDesc("Change the bots nickname");
    this->SetSyntax("\37nickname\37");
  }
  void Run(CommandSource &source, const std::vector<Flux::string> &params)
  {
    User *u = source.u;
    Flux::string newnick = params[1];
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
      ircproto->nick(newnick);
    Log(u, this) << "to change the bots nickname to " << newnick;
  }
};

class CommandRestart : public Command
{
public:
  CommandRestart():Command("RESTART", 0, 1)
  {
   this->SetDesc("Restarts the bot");
   this->SetSyntax("\37reason\37");
  }
  void Run(CommandSource &source, const std::vector<Flux::string> &params)
  {
    if(source.u->IsOwner()){
      ircproto->quit("Restarting..");
      restart("Restarting..");
      Log(source.u, this) << " to restart the bot.";
    }else
      source.Reply(ACCESS_DENIED);
  }
};

class CommandKick : public Command
{
public:
  CommandKick():Command("KICK", 1, 3)
  {
    this->SetDesc("Kick a user from the channel");
    this->SetSyntax("channel \37nick\15");
  }
  void Run(CommandSource &source, const std::vector<Flux::string> &params)
  {
    User *u = source.u;
    if(u->IsOwner()){
      Flux::string kickchan = params[1];
      Flux::string kickee = params[2];
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
  CommandChown():Command("CHOWN", 1, 1)
  {
    this->SetDesc("Change ownership over the bot");
    this->SetSyntax("\37owner\37");
  }
  void Run(CommandSource &source, const std::vector<Flux::string> &params)
  {
    source.Reply("This command is broken!");
  }
};

class CommandQuit : public Command
{
public:
  CommandQuit():Command("QUIT", 1, 1)
  {
    this->SetDesc("Quits the bot from IRC");
    this->SetSyntax("\37randompass\37");
  }
  void Run(CommandSource &source, const std::vector<Flux::string> &params)
  {
    User *u = source.u;
    Flux::string pass = params[1];
    
    if(pass == password || pass == Config->UserPass){
      source.Reply("Quitting..");
      Log(u) << "quit the bot with password: \"" << password << "\"";
      ircproto->quit("Requested From \2%s\17. Pass: \00320%s\017", u->nick.c_str(), password.c_str());
      quitting = true;
    }else{
      source.Reply(ACCESS_DENIED);
      Log(u) << "Attempted to quit the bot";
    }
  }
};

class CommandTopic: public Command
{
public:
  CommandTopic():Command("TOPIC", 2, 2)
  {
    this->SetDesc("Set the topic on a channel");
    this->SetSyntax("\37channel\37 [\37topic\37]");
  }
  void Run(CommandSource &source, const std::vector<Flux::string> &params)
  {
    User *u = source.u;
    if(!u->IsOwner()){
      source.Reply(ACCESS_DENIED);
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
	    Log(u, this) << " in an attempt to change the topic in " << tchan << " and failed to write to temp file 'topic.tmp'";
    }else{
	    topic << "<?xml version=\"1.0\" ?><rss version=\"2.0\"><channel><topic> " << tchan << " Topic: " << msg.strip() << " </topic></channel></rss>" << std::endl;
	    system("sh ftp.sh");
    }
    Log(u, this) << "to change " << tchan << "'s topic to \"" << msg << "\"";
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
      Log(u, this) << "command to get navn's PID " << getpid();
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
      Log(u, this) << "to request navn's quit password: " << password;
    }else{
      source.Reply(ACCESS_DENIED);
      Log(u, this) << "to attempt to request navn's quit password.";
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
      Flux::string fuckingshit = Flux::string("sys_info Error: ") + Flux::string(strerror(errno));
      throw ModuleException(fuckingshit.c_str());
    }
    struct utsname uts;
    if(uname(&uts) < 0){
      Flux::string fuckingshit = Flux::string("uname() Error: ") + Flux::string(strerror(errno));
      throw ModuleException(fuckingshit.c_str());
    }

    // Uptime
    days = sys_info.uptime / 86400;
    hours = (sys_info.uptime / 3600) - (days * 24);
    mins = (sys_info.uptime / 60) - (days * 1440) - (hours * 60);

    source.Reply("System Uptime: %d days, %d hours, %d minutes, %ld seconds",
	  days, hours, mins, sys_info.uptime % 60);
    source.Reply("Bot Uptime: %s", duration(time(NULL) - starttime).c_str());
    
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

    source.Reply(execute("grep 'model name' /proc/cpuinfo").strip());
#else
    source.Reply("This is currently not avalable on windows syetems, sorry.");
#endif
    Log(source.u, this) << "command";
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
  m_system(const Flux::string &Name):module(Name)
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
    Implementation i[] = { I_OnNumeric, I_OnJoin, I_OnKick, I_OnNotice, I_OnChannelOp };
    ModuleHandler::Attach(i, this, sizeof(i)/sizeof(Implementation));
    this->SetAuthor("Justasic");
    this->SetVersion(VERSION);
  }
  void OnNumeric(int i)
  {
    if((i == 4)){
      ircproto->mode(Config->BotNick, "+B");
      sepstream cs(Config->Channel, ',');
      Flux::string tok;
      while(cs.GetToken(tok))
      {
	tok.trim();
	Channel *c = new Channel(tok);
	c->SendJoin();
	c->SendMessage(welcome_msg, Config->BotNick.c_str(), Config->BotNick.c_str());
      }
      if(!Config->OperatorAccount.empty() || !Config->OperatorPass.empty()){
	ircproto->oper(Config->OperatorAccount, Config->OperatorPass);
	IsOper = true;
	ircproto->o = new Oper();
      }
      Log() << "Successfully connected to the server \"" << Config->Server+":"+Config->Port << "\" Master Channel(s): " << Config->Channel;
    }
    if((i == 433)){
      Config->BotNick.push_back(Flux::RandomNickString(5));
      ircproto->nick(Config->BotNick);
      istempnick = true;
    }
    if((i == 376))
    {
      Log(LOG_TERMINAL) << "\033[22;31mStarted with PID \033[22;32m" << getpid() << "\033[22;36m";
      Log(LOG_TERMINAL) << "\033[22;34mSession Password: \033[01;32m" << password << "\033[22;36m";
      ircproto->notice(Config->Owner, "The randomly generated password is: %s", password.c_str());
      started = true;
      /* Identify to the networks services */
      if((!Config->ServicesAccount.empty() || !Config->ServicesPass.empty()) && Config->IdentOnConn){
	Flux::string Sendns = Config->ServicesSendString.replace_all_ci("%a", Config->ServicesAccount);
	Sendns = Sendns.replace_all_ci("%p", Config->ServicesPass);
	ircproto->privmsg(Config->ServicesService, Sendns);
	Log() << "Identified to " << Config->ServicesService << " with account \"" << Config->ServicesAccount << "\"";
      }
    }
  }
  void OnJoin(User *u, Channel *c)
  {
    u->SendMessage("Welcome %s to %s. Type !time for time or \"/msg %s help\" for help on more commands.", u->nick.c_str(),
		   c->name.c_str(), Config->BotNick.c_str());
    Log(u) << "joined " << c->name;
  }
  void OnKick(User *u, User *kickee, Channel *c, const Flux::string &reason)
  {
     if(kickee && kickee->nick.equals_ci(Config->BotNick))
     {
       Log(u) << "kicked me from " << c->name << '(' << reason << ')';
	c->SendJoin(); 
     }
  }
  void OnNotice(User *u, const std::vector<Flux::string> &params)
  {
    Flux::string msg;
    for(unsigned i=0; i < params.size(); ++i)
      msg += params[i]+' ';
      
    msg.trim(); // Auto-Identify
    if(msg.search(Config->AutoIdentString)){
      if((!Config->ServicesPass.empty() || !Config->ServicesAccount.empty()) && u->nick == Config->ServicesService && Config->IdentOnConn){
	Flux::string Sendns = Config->ServicesSendString.replace_all_ci("%a", Config->ServicesAccount);
	Sendns = Sendns.replace_all_ci("%p", Config->ServicesPass);
	u->SendPrivmsg(Sendns);
	Log() << "Identified to " << u->nick << " with account \"" << Config->ServicesAccount << "\"";
      }
    }
  }
  void OnNickChange(User *u, const Flux::string &newnick)
  {
    IsoHost *Host = new IsoHost(u->fullhost);
    if(Host->nick == Config->BotNick)
      Config->BotNick = Host->nick;
    if(newnick == Config->Owner)
      Config->Owner = Host->nick;
    delete Host;
   /*if(command == "NICK"){
   if(u && u->nick == Config->BotNick){
       nick = params[0];
       delete u; //we shouldnt be a user in the 1st place (:
  }else if(u->IsOwner())
    owner_nick = params[0];
  else
    delete u; //we delete the user because the above if statement makes a new one for the nick change
   }*/
  }
};

MODULE_HOOK(m_system)