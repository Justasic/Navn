/* All code is licensed under GNU General Public License GPL v3 (http://www.gnu.org/licenses/gpl.html) */
#include "flux_net_irc.hpp"

/**
 * \file da_goat.h Header file holding the \a Da_Goat function.
 * \author Lordofsraam & Justasic
 */

/**
 * \defgroup da_goatM Da Goat Module
 * This is the module for the Da Goat.
 * For a better description see the function description.
 * \section commands Commands associated with this module.
 * \subsection poke !poke
 * Say \a !poke to poke Da Goat!
 * \subsection info !info
 * Say \a !info to get information about flux-net.
 * \subsection rename !rename
 * Say \a !rename to learn how to rename yourself.
 * \subsection register !register
 * Say \a !register to learn how to reegister yourself.
 * \subsection socialinfo !socialinfo
 * Say \a !socialinfo to learn about contacting flux-net.
 * \subsection help !help
 * Say \a !help to get help on IRC.
 * \subsection ts3 !ts3
 * Say \a !ts3 to get information about flux-net's TeamSpeak 3 server.
 * \subsection uptime !uptime
 * Say \a !uptime to get the uptime of the server that Navn is running on.
 * \subsection rules !rules
 * Say \a !rules to learn about our IRC rules.
 * \subsection spam !spam
 * Say \a !spam to get flux-net's definition of 
 * \subsection version !version
 * Say \a !version to get Navn's verion.
 * \subsection everything !everything
 * Say \a !everything
 * \subsection cum cum
 * Say \a cum
 * @{
 */

/**
 * \fn class Da_Goat(bool a):module("Da_Goat", a, PRIORITY_DONTCARE){ this->SetDesc("Da_Goat bot"); }
 * \brief Runs Da_Goat
 * Da Goat is used to repeat a lot of things we got tired of saying, like teaching new people how to do thigns and what the 
 * rules of the server were.
 */
struct sysinfo sys_info;
class CommandCVersion: public Command
{
public:
  CommandCVersion():Command("!VERSION")
  {
    this->SetDesc("Displays the bots version info into the channel");
  }
  void Run(CommandSource &source, const std::vector<Flux::string> &params)
  {
    User *u = source.u;
    Channel *c = source.c;
    c->SendMessage("The Current Navn Bot Version is \002\0037v%s\017", VERSION);
    c->SendMessage("Navn's code can be found at \002git://gitorious.org/navn/navn.git");
    c->SendMessage("Report all bugs at: \2http://flux-net.net/bugs/\2");
    c->SendMessage("Navn is managed by \2%s\2", Config->Owner.c_str());
    Log(u, this) << "command in " << c->name;
  }
};
class CommandCSpam: public Command
{
public:
  CommandCSpam():Command("!SPAM")
  {
    this->SetDesc("Displays the spam warning");
  }
  void Run(CommandSource &source, const std::vector<Flux::string> &params)
  {
    Log(source.u, this) << "command in " << source.c->name;
    source.c->SendMessage("Spam is the abuse of electronic messaging systems. This includes (but not limited to) external links, Flooding, mass join/quit messages, mass private messages or notices, mIRC color code abuse, CTCP abuse, mass nick changes, etc. If you violate the spam policy you will be kicked.");
  }
};
class CommandCRules: public Command
{
public:
  CommandCRules():Command("!RULES")
  {
    this->SetDesc("Displays the rules of the chatroom");
  }
  void Run(CommandSource &source, const std::vector<Flux::string> &params)
  {
    Channel *c = source.c;
    User *u = source.u;
    c->SendMessage("There are only a few simple rules for %s.", c->name.c_str());
    c->SendMessage("Do NOT hate on others in any way. Basically do not troll in any shape or form.");
    c->SendMessage("Do not ask for op status. you will be granted op status when the moderators feel you deserve op status.");
    c->SendMessage("Do not Mini-mod the chatroom. The moderators are there for a reason, we don't need more.");
    c->SendMessage("Do not spam the chatroom. This includes flooding by text, private messages, join/quit commands, External links, etc.");
    c->SendMessage("If you violate any of these rules you will be kicked and possably banned from %s.", c->name.c_str());
    Log(u, this) << "command in " << c->name;
  }
};
class CommandCUptime: public Command
{
public:
  CommandCUptime():Command("!UPTIME")
  {
    this->SetDesc("Prints Syatem Uptime in the channel");
  }
  void Run(CommandSource &source, const std::vector<Flux::string> &params)
  {
    Channel *c = source.c;
    User *u = source.u;
#ifndef _WIN32
      if(sysinfo(&sys_info) != 0)
	      perror("sysinfo");

      // Uptime
      int days, hours, mins;
      days = sys_info.uptime / 86400;
      hours = (sys_info.uptime / 3600) - (days * 24);
      mins = (sys_info.uptime / 60) - (days * 1440) - (hours * 60);
      c->SendMessage("System Uptime: %d days, %d hours, %d minutes, %ld seconds",days, hours, mins, sys_info.uptime % 60);
      c->SendMessage("Bot Uptime: %s", duration(time(NULL) - starttime).c_str());
#else
      source.Reply("This is currently not avalable on windows systems, sorry.");
#endif
      Log(u, this) << "command in " << c->name;
  }
};
class CommandCSocialInfo: public Command
{
public:
  CommandCSocialInfo():Command("!SOCIALINFO")
  {
    this->SetDesc("Displays Flux-Net's Social Information");
  }
  void Run(CommandSource &source, const std::vector<Flux::string> &params)
  {
    Channel *c = source.c;
    User *u = source.u;
    c->SendMessage("Ventrilo Server:\002 5.110.166.75 Port:\00313 3784\nOur IRC server: irc.flux-net.net:6667");
    c->SendMessage("The Flux-Net TeamSpeak 3 server is:\nGalaxy.Flux-Net.net:9987");
    Log(u, this) << "command in " << c->name;
  }
};
class CommandCRegister: public Command
{
public:
  CommandCRegister():Command("!REGISTER")
  {
    this->SetDesc("Displays how to register your nickname");
  }
  void Run(CommandSource &source, const std::vector<Flux::string> &params)
  {
    Channel *c = source.c;
    User *u = source.u;
    c->SendMessage("To Register your nickname type:");
    c->SendMessage("\0034If this is the nick you want then skip step 1.\017");
    c->SendMessage("\0034Do not include brackets when registering, this will cause errors\017");
    c->SendMessage("Step 1: change your nick \00312/nick [nickname here]\017");
    c->SendMessage("Step 2: now to begin registration type:");
    c->SendMessage("\00312/ns register [\037YourPasswordHere\017] [\002YouremailHere\015]");
    c->SendMessage("Step 3: Then to identify so you can have op status type:");
    c->SendMessage("\00312/identify [passwordhere]\017");
    c->SendMessage("\0034You will need to indetify everytime you join into the chatroom\017");
    c->SendMessage("Unless your client does it automatically (ei. xchat, mIRC, iceChat).\017");
    Log(u, this) << "command in " << c->name;
  }
};

class CommandCRename: public Command
{
public:
  CommandCRename():Command("!RENAME")
  {
    this->SetDesc("Displays how to rename");
  }
  void Run(CommandSource &source, const std::vector<Flux::string> &params)
  {
    Channel *c = source.c;
    User *u = source.u;
    c->SendMessage("This channel is a Nickname only channel. This means that you MUST have your own Nickname! If you do not choose your own nick name you WILL be kicked.");
    c->SendMessage("To change your nickname type (without quotes) '/nick MyNewNickname' to change your nickname. (replacing MyNewNickname with a personal nickname).");
    Log(u, this) << "command in " << c->name;
  }
};
class CommandCInfo : public Command
{
public:
  CommandCInfo():Command("!INFO")
  {
    this->SetDesc("General Da_Goat info");
  }
  void Run(CommandSource &source, const std::vector<Flux::string> &params)
  {
    Channel *c = source.c;
    User *u = source.u;
    c->SendMessage("Our forum is at \037http://flux-net.net/forum2/\017");
    c->SendMessage("Our Website is \002Flux-Net.net\017");
    c->SendMessage("Ftp server \002178.63.127.231\002 login anonymous \002-no password-\002, Files in dir \002/ftp/pub\002");
    Log(u, this) << "command in " << c->name;
  }
};

class Da_Goat:public module
{
  CommandCInfo info;
  CommandCRename rename;
  CommandCRegister reg;
  CommandCSocialInfo sinfo;
  CommandCUptime uptime;
  CommandCRules rules;
  CommandCSpam spam; 
  CommandCVersion ver;
public:
    Da_Goat(const Flux::string &Name):module(Name){
      this->SetAuthor("Justasic & Lordofsraam");
      this->SetVersion(VERSION);
      this->AddChanCommand(&info);
      this->AddChanCommand(&rename);
      this->AddChanCommand(&reg);
      this->AddChanCommand(&sinfo);
      this->AddChanCommand(&uptime);
      this->AddChanCommand(&rules);
      this->AddChanCommand(&spam);
      this->AddChanCommand(&ver);
      ModuleHandler::Attach(I_OnPrivmsg, this);
    }
    void OnPrivmsg(User *u, Channel *c, const std::vector<Flux::string> &params){
      Flux::string cmd = params.empty()?"":params[0], msg;
      for(unsigned i=0; i < params.size(); ++i)
	msg += params[i]+' ';
      msg.trim();
      /*******************************Easter Eggs*********************************/
      if(cmd.equals_ci("!poke")){ //Easter egg ;P
	Flux::string person = params.size() == 2?params[1]:"";
	person.trim();
	if(u->IsOwner()){
	  c->SendMessage("why would I wanna kick my master!?");
	  return;
	}
	if(person.empty()){
	  c->SendAction("is angry at %s", u->nick.c_str());
	  c->kick(u, "\002\00315Dont poke me!\017");
	  Log() << u->nick << " found Da_Goats !poke command in " << c->name;
	}else{
	  c->SendAction("Is angry at %s", person.c_str());
	  c->kick(person, "\002\00315Dont poke me!\017");
	  Log() << u->nick << " used Da_Goats !poke command in " << c->name << " to poke " << person;
	}
      }
      if(msg.search("Dun kick me, asswipe. -_-")){
	Send->command->mode(c->name, "+b m:"+u->fullhost);
	c->SendMessage("Shut up!");
      }
      if(cmd.equals_ci("!everything")){
	c->SendMessage("Yes, there is a script for everything..\007");
      }
      /***********************End Da_Goat Functions*******************************/
      if(cmd.equals_ci("!bugs")){
	c->SendMessage("Report Bugs at: http://flux-net.net/bugs/");
      }
      if(cmd.equals_ci("!git")){
	u->SendMessage("Navn git: git://gitorious.org/navn/navn.git");
      }
  }
};

MODULE_HOOK(Da_Goat)
/**
 * @}
 */
