#ifndef DA_GOAT_H
#define DA_GOAT_H
#include "../includes.h"
#include "../flux_net_irc.hpp"
using namespace std;
using namespace flux_net_irc;
void Da_Goat(Socket &sock, irc_string *reply){
/***************************Da_Goat Functions*******************************/
	if(reply->said("!poke")){ //Easter egg ;P
		sock << me(chan, "Is angry at "+unick);
		sock << kick(chan, unick, "\002\00315Dont poke me!\017");
		log("%s used Da_Goats !poke command in %s", unick.c_str(), chan.c_str());
      }
	if(reply->said("!info")){
		sock << privmsg(chan, "Our forum is at \037castawaycrew.hyperboards.com\017");
		sock << privmsg(chan, "Our Website is \002Flux-Net.net\017");
		sock << privmsg(chan, "Ftp server \002178.63.127.231\002 login anonymous \002-no password-\002, Files in dir \002/ftp/pub\002");
		log("%s used Da_Goats !info command in %s", unick.c_str(), chan.c_str());
      }
      if(reply->said("!rename")){
		sock << privmsg(chan, "This channel is a Nickname only channel. This means that you MUST have your own Nickname! If you do not choose your own nick name you WILL be kicked.");
		sock << privmsg(chan, "To change your nickname type (without quotes) '/nick MyNewNickname' to change your nickname. (replacing MyNewNickname with a personal nickname).");
		log("%s used Da_Goats !rename command in %s", unick.c_str(), chan.c_str());
      }
      if(reply->said("!register")){
		sock << privmsg(chan, "To Register your nickname type:");
		sock << privmsg(chan, "\0034If this is the nick you want then skip step 1.\017");
		sock << privmsg(chan, "\0034Do not include brackets when registering, this will cause errors\017");
		sock << privmsg(chan, "Step 1: change your nick \00312/nick [nickname here]\017");
		sock << privmsg(chan, "Step 2: now to begin registration type:");
		sock << privmsg(chan, "\00312/ns register [\037YourPasswordHere\017] [\002YouremailHere\015]");
		sock << privmsg(chan, "Step 3: Then to identify so you can have op status type:");
		sock << privmsg(chan, "\00312/identify [passwordhere]\017");
		sock << privmsg(chan, "\0034You will need to indetify everytime you join into the chatroom\017");
		sock << privmsg(chan, "Unless your client does it automatically (ei. xchat, mIRC, iceChat).\017");
		log("%s used Da_Goats !register command in %s", unick.c_str(), chan.c_str());
      }
      if(reply->said("!socialinfo")){
		sock << privmsg(chan, "Ventrilo Server:\002 5.110.166.75 Port:\00313 3784");
		sock << privmsg(chan, "Our IRC server: \002irc.Flux-Net.net\002 port\00313 6667\017 or\00313 8067\017");
		log("%s used Da_Goats !socialinfo command in %s", unick.c_str(), chan.c_str());
      }
      if(reply->said("!help")){
		sock << privmsg(chan, "Local %s commands are:", chan.c_str());
		sock << privmsg(chan, "!help !info !register !socialinfo !version !time");
		sock << privmsg(chan, "!changelog !uptime !rules !spam !rename !bugs");
		log("%s used Da_Goats !help command in %s", unick.c_str(), chan.c_str());
      }
      if(reply->said("!ts3")){
		sock << privmsg(chan, "The Flux-Net TeamSpeak 3 server is: ");
		sock << privmsg(chan, "Galaxy.Flux-Net.net:9987");
		log("%s used Da_Goats !ts3 command in %s", unick.c_str(), chan.c_str());
	}
      if(reply->said("PRIVMSG "+nick+" :topic")){
		if(unick == owner_nick){
			string tchan = reply->params(1);
			if(!IsValadChannel(tchan))
				sock << notice(unick, "%s is not a valid channel.", tchan.c_str());
		  
			msg.erase(0,6);
			sock << topic(tchan, msg);
			fstream topic;

			topic.open("topic.tmp", fstream::in | fstream::out | fstream::app);

			if(!topic.is_open()){

				sock << notice(owner_nick, "Unable to write topic temp file");
				sock << notice(unick, "Unable to write topic temp file");
				cout << "Unable to write topic temp file" << nl;
			}else{
				topic << "<?xml version=\"1.0\" ?><rss version=\"2.0\"><channel><description> " << tchan << " Topic: " << strip(msg) << " </description></channel></rss>" << nl;
				system("sh ftp.sh");
			}
		}else{ 
			sock << notice(unick, access_denied); 
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
 
		sock << notice(unick, "Uptime: %d days, %d hours, %d minutes, %ld seconds",
                      days, hours, mins, sys_info.uptime % 60);
 
		// Load Averages for 1,5 and 15 minutes
		sock << notice(unick, "Load Avgs: 1min(%ld) 5min(%ld) 15min(%ld)",
				sys_info.loads[0], sys_info.loads[1], sys_info.loads[2]);
 
		// Total and free ram.
		sock << notice(unick, "Total Ram: %ldk\tFree: %ldk", sys_info.totalram / 1024,
                                        sys_info.freeram / 1024);
 
		// Shared and buffered ram.
		sock << notice(unick, "Shared Ram: %ldk", sys_info.sharedram / 1024);
		sock << notice(unick, "Buffered Ram: %ldk", sys_info.bufferram / 1024);
 
		// Swap space
		sock << notice(unick, "Total Swap: %ldk\tFree: %ldk", sys_info.totalswap / 1024,
                                           sys_info.freeswap / 1024);
 
		// Number of processes currently running.
		sock << notice(unick, "Number of processes: %d", sys_info.procs);
		sock << notice(unick, "\003");
		sock << notice(unick, "System Name: %s\tRelease: %s %s\tMachine: %s", uts.nodename, uts.sysname, uts.release, uts.machine );
		sock << notice(unick, "System Version: %s", uts.version);

		sock << notice(unick, strip(execute("grep 'model name' /proc/cpuinfo")));
		log("%s used stats command in %s", unick.c_str(), chan.c_str());
      }
      if(reply->said("!uptime")){
		if(sysinfo(&sys_info) != 0)
			perror("sysinfo");
 
		// Uptime
		days = sys_info.uptime / 86400;
		hours = (sys_info.uptime / 3600) - (days * 24);
		mins = (sys_info.uptime / 60) - (days * 1440) - (hours * 60);
 
		sock << privmsg(chan, "Uptime: %d days, %d hours, %d minutes, %ld seconds",
                      days, hours, mins, sys_info.uptime % 60);
		log("%s used !uptime command in %s", unick.c_str(), chan.c_str());
	  }
      if(reply->said("!rules")){
		sock << privmsg(chan, "There are only a few simple rules for %s.", chan.c_str());
        sock << privmsg(chan, "Do NOT hate on others in any way. Basically do not troll in any shape or form.");
        sock << privmsg(chan, "Do not ask for op status. you will be granted op status when the moderators feel you deserve op status.");
        sock << privmsg(chan, "Do not Mini-mod the chatroom. The moderators are there for a reason, we don't need more.");
        sock << privmsg(chan, "Do not spam the chatroom. This includes flooding by text, private messages, join/quit commands, External links, etc.");
		sock << privmsg(chan, "If you violate any of these rules you will be kicked and possably banned from %s.", chan.c_str());
		log("%s used Da_Goats !rules command in %s", unick.c_str(), chan.c_str());
      }
      if(reply->said("!spam")){
		log("%s used Da_Goats !spam command in %s", unick.c_str(), chan.c_str());
        sock << privmsg(chan, "Spam is the abuse of electronic messaging systems. This includes (but not limited to) external links, Flooding, mass join/quit messages, mass private messages or notices, mIRC color code abuse, CTCP abuse, mass nick changes, etc. If you violate the spam policy you will be kicked.");
      }
      if(reply->said("!version")){
        sock << privmsg(chan, "The Current Navn Bot Version is \002\0037%s\017", version.c_str());
        sock << privmsg(chan, "Navn (which includes Dah_Goat) is Full C++ coded from scratch by lordofsraam");
		sock << privmsg(chan, "Navn's Code can be found at \002git@gitorious.org:navn/navn.git");
		sock << privmsg(chan, "Report all bugs at: \2http://flux-net.net/bugs/\2");
        sock << privmsg(chan, "Navn is managed by \2%s\2", owner_nick.c_str());
		sock << privmsg(chan, "If you would like to add a command or function, talk to him.");
		log("%s used Da_Goats !version command in %s", unick.c_str(), chan.c_str());
      }
      /*******************************Easter Eggs*********************************/
      if(reply->said("!everything")){
		sock << privmsg(chan, "Yes, there is a script for everything..\007");
      }
      if(reply->said("cum ")){
         sock << privmsg(chan, "ewww..");
      }
      /***********************End Da_Goat Functions*******************************/

}
#endif
