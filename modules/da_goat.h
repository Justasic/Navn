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
    ModuleReturn run(SendMessage *Send, Flux::string rply, irc_string *reply){
/***************************Da_Goat Functions*******************************/
	if(reply->params(0) == "!poke"){ //Easter egg ;P
		Flux::string person = reply->params(1);
		person.trim();
		if (person.empty()){
		  Send->action(chan, "is angry at "+unick);
		  Send->command->kick(chan, unick, "\002\00315Dont poke me!\017");
		  log("%s found Da_Goats !poke command in %s", unick.c_str(), chan.c_str());
		}else{
		  Send->action(chan, "Is angry at "+person);
		  Send->command->kick(chan, person, "\002\00315Dont poke me!\017");
		  log("%s used Da_Goats !poke command in %s to poke %s", unick.c_str(), chan.c_str(), person.c_str());
		}
      }
	if(reply->params(0) == "!info"){
		Send->privmsg(chan, "Our forum is at \037castawaycrew.hyperboards.com\017");
		Send->privmsg(chan, "Our Website is \002Flux-Net.net\017");
		Send->privmsg(chan, "Ftp server \002178.63.127.231\002 login anonymous \002-no password-\002, Files in dir \002/ftp/pub\002");
		log("%s used Da_Goats !info command in %s", unick.c_str(), chan.c_str());
      }
      if(reply->params(0) == "!rename"){
		Send->privmsg(chan, "This channel is a Nickname only channel. This means that you MUST have your own Nickname! If you do not choose your own nick name you WILL be kicked.");
		Send->privmsg(chan, "To change your nickname type (without quotes) '/nick MyNewNickname' to change your nickname. (replacing MyNewNickname with a personal nickname).");
		log("%s used Da_Goats !rename command in %s", unick.c_str(), chan.c_str());
      }
      if(reply->params(0) == "!register"){
		Send->privmsg(chan, "To Register your nickname type:");
		Send->privmsg(chan, "\0034If this is the nick you want then skip step 1.\017");
		Send->privmsg(chan, "\0034Do not include brackets when registering, this will cause errors\017");
		Send->privmsg(chan, "Step 1: change your nick \00312/nick [nickname here]\017");
		Send->privmsg(chan, "Step 2: now to begin registration type:");
		Send->privmsg(chan, "\00312/ns register [\037YourPasswordHere\017] [\002YouremailHere\015]");
		Send->privmsg(chan, "Step 3: Then to identify so you can have op status type:");
		Send->privmsg(chan, "\00312/identify [passwordhere]\017");
		Send->privmsg(chan, "\0034You will need to indetify everytime you join into the chatroom\017");
		Send->privmsg(chan, "Unless your client does it automatically (ei. xchat, mIRC, iceChat).\017");
		log("%s used Da_Goats !register command in %s", unick.c_str(), chan.c_str());
      }
      if(reply->params(0) == "!socialinfo"){
		Send->privmsg(chan, "Ventrilo Server:\002 5.110.166.75 Port:\00313 3784\nOur IRC server: irc.flux-net.net:6667");
		log("%s used Da_Goats !socialinfo command in %s", unick.c_str(), chan.c_str());
      }
      if(reply->params(0) == "!help"){
		Send->privmsg(chan, "Local %s commands are:", chan.c_str());
		Send->privmsg(chan, "!help !info !register !socialinfo !version !time\n!uptime !rules !spam !rename !bugs");
		log("%s used Da_Goats !help command in %s", unick.c_str(), chan.c_str());
      }
      if(reply->params(0) == "!ts3"){
		Send->privmsg(chan, "The Flux-Net TeamSpeak 3 server is: ");
		Send->privmsg(chan, "Galaxy.Flux-Net.net:9987");
		log("%s used Da_Goats !ts3 command in %s", unick.c_str(), chan.c_str());
	}
      if(reply->said("PRIVMSG "+nick+" :topic")){
		if(unick == owner_nick){
			Flux::string tchan = reply->params(1);
			tchan.trim();
			if(!IsValidChannel(tchan)){
				Send->notice(unick, "\2%s\2 is not a valid channel.", tchan.c_str());
				Send->notice(unick, "Syntax: \2TOPIC \37channel\37 \37topic\37");
				return MOD_STOP;
			}
			msg = msg.erase(0,6);
			msg = msg.erase(msg.find_first_of(tchan), tchan.size());
			msg.trim();
			Send->command->topic(tchan, msg);
			fstream topic;
			topic.open("topic.tmp", fstream::in | fstream::out | fstream::app);
			if(!topic.is_open()){
				Send->notice(unick, "Unable to write topic temp file");
				cout << "Unable to write topic temp file" << nl;
				log("%s used /msg %s topic to change %s's topic to \"%s\" but could not write to topic temp file '%s'", 
				    unick.c_str(), nick.c_str(), tchan.c_str(), msg.c_str(), "topic.tmp");
			}else{
				topic << "<?xml version=\"1.0\" ?><rss version=\"2.0\"><channel><topic> " << tchan << " Topic: " << strip(msg) << " </topic></channel></rss>" << nl;
				system("sh ftp.sh");
			}
			log("%s used Da_Goats !topic command to change the topin in %s to \"%s\"",unick.c_str(), tchan.c_str(), strip(msg).c_str());
		}else{ 
			Send->notice(unick, access_denied); 
		}
	}
      if(reply->said("PRIVMSG "+nick+" :stats")){
	  //Shows system stats in the channel.
	
		if(sysinfo(&sys_info) != 0)
			//perror("sysinfo");
			throw CoreException("sys_info Error");
		struct utsname uts;
		if(uname(&uts) < 0)
			//perror("uname() error");
			throw CoreException("uname() Error");
 
		// Uptime
		days = sys_info.uptime / 86400;
		hours = (sys_info.uptime / 3600) - (days * 24);
		mins = (sys_info.uptime / 60) - (days * 1440) - (hours * 60);
 
		Send->notice(unick, "Uptime: %d days, %d hours, %d minutes, %ld seconds",
                      days, hours, mins, sys_info.uptime % 60);
 
		// Load Averages for 1,5 and 15 minutes
		Send->notice(unick, "Load Avgs: 1min(%ld) 5min(%ld) 15min(%ld)",
				sys_info.loads[0], sys_info.loads[1], sys_info.loads[2]);
 
		// Total and free ram.
		Send->notice(unick, "Total Ram: %ldk\tFree: %ldk", sys_info.totalram / 1024,
                                        sys_info.freeram / 1024);
 
		// Shared and buffered ram.
		Send->notice(unick, "Shared Ram: %ldk", sys_info.sharedram / 1024);
		Send->notice(unick, "Buffered Ram: %ldk", sys_info.bufferram / 1024);
 
		// Swap space
		Send->notice(unick, "Total Swap: %ldk\tFree: %ldk", sys_info.totalswap / 1024,
                                           sys_info.freeswap / 1024);
 
		// Number of processes currently running.
		Send->notice(unick, "Number of processes: %d", sys_info.procs);
		Send->notice(unick, "\003");
		Send->notice(unick, "System Name: %s\tRelease: %s %s\tMachine: %s", uts.nodename, uts.sysname, uts.release, uts.machine);
		Send->notice(unick, "System Version: %s", uts.version);

		Send->notice(unick, strip(execute("grep 'model name' /proc/cpuinfo")));
		log("%s used stats command in %s", unick.c_str(), chan.c_str());
      }
      if(reply->params(0) == "!uptime"){
		if(sysinfo(&sys_info) != 0)
			perror("sysinfo");
 
		// Uptime
		days = sys_info.uptime / 86400;
		hours = (sys_info.uptime / 3600) - (days * 24);
		mins = (sys_info.uptime / 60) - (days * 1440) - (hours * 60);
 
		Send->privmsg(chan, "Uptime: %d days, %d hours, %d minutes, %ld seconds",days, hours, mins, sys_info.uptime % 60);
		log("%s used !uptime command in %s", unick.c_str(), chan.c_str());
	  }
      if(reply->params(0) == "!rules"){
	Send->privmsg(chan, "There are only a few simple rules for %s.", chan.c_str());
        Send->privmsg(chan, "Do NOT hate on others in any way. Basically do not troll in any shape or form.");
        Send->privmsg(chan, "Do not ask for op status. you will be granted op status when the moderators feel you deserve op status.");
        Send->privmsg(chan, "Do not Mini-mod the chatroom. The moderators are there for a reason, we don't need more.");
        Send->privmsg(chan, "Do not spam the chatroom. This includes flooding by text, private messages, join/quit commands, External links, etc.");
	Send->privmsg(chan, "If you violate any of these rules you will be kicked and possably banned from %s.", chan.c_str());
	log("%s used Da_Goats !rules command in %s", unick.c_str(), chan.c_str());
      }
      if(reply->params(0) == "!spam"){
	log("%s used Da_Goats !spam command in %s", unick.c_str(), chan.c_str());
        Send->privmsg(chan, "Spam is the abuse of electronic messaging systems. This includes (but not limited to) external links, Flooding, mass join/quit messages, mass private messages or notices, mIRC color code abuse, CTCP abuse, mass nick changes, etc. If you violate the spam policy you will be kicked.");
      }
      if(reply->params(0) == "!version"){
        Send->privmsg(chan, "The Current Navn Bot Version is \002\0037v%s\017", VERSION_SHORT.c_str());
        Send->privmsg(chan, "Navn (which includes Dah_Goat) is Full C++ code by lordofsraam\n and is further developed with Justasic");
	Send->privmsg(chan, "Navn's code can be found at \002git://gitorious.org/navn/navn.git");
	Send->privmsg(chan, "Report all bugs at: \2http://flux-net.net/bugs/\2");
        Send->privmsg(chan, "Navn is managed by \2%s\2", owner_nick.c_str());
	log("%s used Da_Goats !version command in %s", unick.c_str(), chan.c_str());
      }
      /*******************************Easter Eggs*********************************/
      if(reply->params(0) == "!everything"){
	Send->privmsg(chan, "Yes, there is a script for everything..\007");
      }
      if(reply->said("cum ")){
         Send->privmsg(chan, "ewww..");
      }
      /***********************End Da_Goat Functions*******************************/
      return MOD_RUN;
  }
};
/**
 * @}
 */
#endif
