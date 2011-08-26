#ifndef DA_GOAT_H
#define DA_GOAT_H
#include "../includes.h"
#include "../flux_net_irc.hpp"


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
class Da_Goat:public module
{
public:
    Da_Goat(bool a):module("Da_Goat", a, PRIORITY_DONTCARE){ this->SetDesc("Da_Goat bot"); }
    ModuleReturn run(CommandSource &source, std::vector<Flux::string> &params){
      Flux::string cmd = params.empty()?"":params[0]; /* we define chan because i am too lazy to convert all these */
      User *u = source.u;
      Channel *c = source.c;
/***************************Da_Goat Functions*******************************/
	if(cmd == "!poke"){ //Easter egg ;P
		Flux::string person = params.size() == 2?params[1]:"";
		person.trim();
		if(person == owner_nick && u->nick != owner_nick){
		 c->SendMessage("why would I wanna kick my master!?");
		 return MOD_STOP;
		}
		if (person.empty()){
		  c->SendAction("is angry at %s", u->nick.c_str());
		  c->kick(u, "\002\00315Dont poke me!\017");
		  log("%s found Da_Goats !poke command in %s", u->nick.c_str(), c->name.c_str());
		}else{
		  c->SendAction("Is angry at %s", person.c_str());
		  c->kick(person, "\002\00315Dont poke me!\017");
		  log("%s used Da_Goats !poke command in %s to poke %s", u->nick.c_str(), c->name.c_str(), person.c_str());
		}
      }
	if(cmd == "!info"){
		c->SendMessage("Our forum is at \037castawaycrew.hyperboards.com\017");
		c->SendMessage("Our Website is \002Flux-Net.net\017");
		c->SendMessage("Ftp server \002178.63.127.231\002 login anonymous \002-no password-\002, Files in dir \002/ftp/pub\002");
		log("%s used Da_Goats !info command in %s", u->nick.c_str(), c->name.c_str());
      }
      if(cmd == "!rename"){
		c->SendMessage("This channel is a Nickname only channel. This means that you MUST have your own Nickname! If you do not choose your own nick name you WILL be kicked.");
		c->SendMessage("To change your nickname type (without quotes) '/nick MyNewNickname' to change your nickname. (replacing MyNewNickname with a personal nickname).");
		log("%s used Da_Goats !rename command in %s", u->nick.c_str(), c->name.c_str());
      }
      if(cmd == "!register"){
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
		log("%s used Da_Goats !register command in %s", u->nick.c_str(), c->name.c_str());
      }
      if(cmd == "!socialinfo"){
		c->SendMessage("Ventrilo Server:\002 5.110.166.75 Port:\00313 3784\nOur IRC server: irc.flux-net.net:6667");
		log("%s used Da_Goats !socialinfo command in %s", u->nick.c_str(), c->name.c_str());
      }
      if(cmd == "!help"){
		c->SendMessage("Local %s commands are:", c->name.c_str());
		c->SendMessage("!help !info !register !socialinfo !version !time\n!uptime !rules !spam !rename !bugs");
		log("%s used Da_Goats !help command in %s", u->nick.c_str(), c->name.c_str());
      }
      if(cmd == "!ts3"){
		c->SendMessage("The Flux-Net TeamSpeak 3 server is:\n Galaxy.Flux-Net.net:9987");
		log("%s used Da_Goats !ts3 command in %s", u->nick.c_str(), c->name.c_str());
	}
      if(cmd == "topic"){
		if(u->nick == owner_nick){
			if(params.size() < 3){
			  source.Reply("Syntax: \2TOPIC \37channel\37 \37topic\37");
			  return MOD_STOP;
			}
			Flux::string tchan = params[1];
			if(!IsValidChannel(tchan)){
				source.Reply("\2%s\2 is not a valid channel.", tchan.c_str());
				return MOD_STOP;
			}
			Channel *ch = findchannel(tchan);
			if(!ch){
			 source.Reply("I am not in channel \2%s\2", tchan.c_str());
			 return MOD_STOP;
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
				std::cout << "Unable to write topic temp file" << nl;
				log("%s used /msg %s topic to change %s's topic to \"%s\" but could not write to topic temp file '%s'", 
				    u->nick.c_str(), nick.c_str(), tchan.c_str(), msg.c_str(), "topic.tmp");
			}else{
				topic << "<?xml version=\"1.0\" ?><rss version=\"2.0\"><channel><topic> " << tchan << " Topic: " << strip(msg) << " </topic></channel></rss>" << nl;
				system("sh ftp.sh");
			}
			log("%s used Da_Goats !topic command to change the topin in %s to \"%s\"",u->nick.c_str(), tchan.c_str(), strip(msg).c_str());
		}else{ 
			source.Reply(access_denied); 
		}
	}
      if(cmd == "stats"){
	  //Shows system stats in the channel.
	
		if(sysinfo(&sys_info) != 0){//now here Justasic got pissed because c strings suck ass
		  Flux::string fuckingshit = Flux::string("sys_info Error: ") + strerror(errno);
		  throw CoreException(fuckingshit.c_str());
		}
		struct utsname uts;
		if(uname(&uts) < 0){
		  Flux::string fuckingshit = Flux::string("uname() Error: ") + strerror(errno);
		  throw CoreException(fuckingshit.c_str());
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
		log("%s used stats command in %s", u->nick.c_str(), c->name.c_str());
      }
      if(cmd == "!uptime"){
		if(sysinfo(&sys_info) != 0)
			perror("sysinfo");
 
		// Uptime
		days = sys_info.uptime / 86400;
		hours = (sys_info.uptime / 3600) - (days * 24);
		mins = (sys_info.uptime / 60) - (days * 1440) - (hours * 60);
 
		c->SendMessage("Uptime: %d days, %d hours, %d minutes, %ld seconds",days, hours, mins, sys_info.uptime % 60);
		log("%s used !uptime command in %s", u->nick.c_str(), c->name.c_str());
	  }
      if(cmd == "!rules"){
	c->SendMessage("There are only a few simple rules for %s.", c->name.c_str());
        c->SendMessage("Do NOT hate on others in any way. Basically do not troll in any shape or form.");
        c->SendMessage("Do not ask for op status. you will be granted op status when the moderators feel you deserve op status.");
        c->SendMessage("Do not Mini-mod the chatroom. The moderators are there for a reason, we don't need more.");
        c->SendMessage("Do not spam the chatroom. This includes flooding by text, private messages, join/quit commands, External links, etc.");
	c->SendMessage("If you violate any of these rules you will be kicked and possably banned from %s.", c->name.c_str());
	log("%s used Da_Goats !rules command in %s", u->nick.c_str(), c->name.c_str());
      }
      if(cmd == "!spam"){
	log("%s used Da_Goats !spam command in %s", u->nick.c_str(), c->name.c_str());
        c->SendMessage("Spam is the abuse of electronic messaging systems. This includes (but not limited to) external links, Flooding, mass join/quit messages, mass private messages or notices, mIRC color code abuse, CTCP abuse, mass nick changes, etc. If you violate the spam policy you will be kicked.");
      }
      if(cmd == "!version"){
        c->SendMessage("The Current Navn Bot Version is \002\0037v%s\017", VERSION_SHORT.c_str());
        c->SendMessage("Navn (which includes Dah_Goat) is Full C++ code by lordofsraam\n and is further developed with Justasic");
	c->SendMessage("Navn's code can be found at \002git://gitorious.org/navn/navn.git");
	c->SendMessage("Report all bugs at: \2http://flux-net.net/bugs/\2");
        c->SendMessage("Navn is managed by \2%s\2", owner_nick.c_str());
	log("%s used Da_Goats !version command in %s", u->nick.c_str(), c->name.c_str());
      }
      /*******************************Easter Eggs*********************************/
      if(cmd == "!everything"){
	c->SendMessage("Yes, there is a script for everything..\007");
      }
      /***********************End Da_Goat Functions*******************************/
      return MOD_RUN;
  }
};
/**
 * @}
 */
#endif
