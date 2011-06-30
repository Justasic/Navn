#ifndef SYSTEM_H
#define SYSTEM_H
#include "../includes.h"
#include "../flux_net_irc.hpp"
using namespace std;
using namespace flux_net_irc;
void system_m(Socket &sock, irc_string *reply, string rply){

  if (reply->said("PRIVMSG "+nick+" :pass")){
	if (unick == owner_nick){
	  sock << notice(unick, "The password is:\2 "+password);
	  log("%s requested the navn quit password: %s", unick.c_str(), password.c_str());
	} else {
	  sock << notice(unick, access_denied);
	  log("%s attempted to request the navn quit password.", unick.c_str());
	}
      }
	  if(reply->said("This nickname is registered and protected")){
	    sock << privmsg(unick, "identify %s %s", nsacc.c_str(), nspass.c_str());
		log("Identified to NickServ with account \"%s\"", nsacc.c_str());
	  }
	  if (reply->said("PRIVMSG "+nick+" :restart")){
	    if(reply->params(2) == password || reply->params(2) == usrpass){
	      //string reason = "-no reason-";
	      sock << quit("Restarting..");
	      restart("Restarting..");
	     }else{
	       sock << notice(unick, access_denied);
	   }
	}
      if (reply->said("PRIVMSG "+nick+" :join")){
	string blah = reply->params(1);
	if(IsValadChannel(blah)){
	 sock << notice(unick, "Channel %s is not a valad channel.", blah.c_str());
	 log("%s attempted to make bot join %s", unick.c_str(), blah.c_str());
	} else {
		log("%s made the bot join %s", unick.c_str(), blah.c_str());
		sock << join(blah);
		sock << privmsg(blah, welcome_msg, nick.c_str(), nick.c_str());
	}
      }
      if (reply->said("PRIVMSG "+nick+" :part")){
	string blah = reply->params(1);
	if(IsValadChannel(blah)){
	 sock << notice(unick, "Channel %s is not a valad channel.", blah.c_str());
	 log("%s attempted to make bot part %s", unick.c_str(), blah.c_str());
	} else {
		log("%s made the bot part %s", unick.c_str(), blah.c_str());
		sock << part(blah);
	}
      }
      if (reply->said("rejoin")){ //rejoin the channel
	if(!in_channel){
		sock << join(channel);
		sock << privmsg(channel, welcome_msg, nick.c_str());
		log("%s made the bot rejoin the channel", unick.c_str());
	}else{
	  sock << notice(unick, "The bot is already in the channel.");
	  log("%s attempted to rejoin the bot into a channel its already in.", unick.c_str());
	}
      }
      if (reply->said("JOIN :"+chan) && in_channel){ //welcomes everyone who joins the channel
	if(unick == nick){}else{
	   sock << notice(unick, "Welcome "+unick+" to "+strip(chan)+". Type !time for time or \"/msg "+nick+" help\" for help on more commands.");
	   log("%s joined %s", unick.c_str(), strip(chan).c_str());
	}
      }
      if (reply->said("PRIVMSG "+nick+" :quit "+password)){ //quits the bot.
		sock << notice(unick, "Quitting..");
		log("%s quit the bot with password: \"%s\"", unick.c_str(), password.c_str());
		DoQuit(sock, "Requested From \2"+unick+"\17. Pass: \00320"+password+"\017");
      }
      if (reply->said(killed)){ // if the bot is killed.. throw a core exception saying so.
	throw CoreException("You have been killed by "+unick);
      }
      if(reply->said("PRIVMSG "+nick+" :rehash")){
	string getpass = reply->params(1);
	if(unick == owner_nick || getpass == usrpass){
	sock << notice(unick, "Rehashing config file.");
	log("%s rehashed config file.", unick.c_str());
	Rehash(sock);
	}else{
	  sock << notice(unick, access_denied);
	  log("%s attempted a rehash.", unick.c_str());
	}
      }
	  if(reply->said("!bugs")){
	    sock.Send(privmsg(chan, "Report Bugs at: http://flux-net.net/bugs/"));
	  }
	  if(reply->said("482 "+nick+" "+chan+" :You must be a channel operator")){
	   cout << "\033[22;31mI require op to preform this function\033[22;36m" << nl;
	   log("Op is required in %s", chan.c_str());
	  }
      //this says that we are now in the server
      if(reply->said("005 "+nick)){
		sock << mode(nick, "+B");
		sock << join(channel);
		if(ouser.empty() || opass.empty()){
		}else{
		  sock << oper("%s %s", ouser.c_str(), opass.c_str());
		}if(nsacc.empty() || nspass.empty()){
		}else{
		  sock << privmsg("NickServ", "identify "+nsacc+" "+nspass);
		}
		log("Successfully connected to the server \"%s\" Port: %s Master Channel: %s", server.c_str(), port.c_str(), channel.c_str());
      }
	  if(reply->said("? git")){
	    sock << notice(unick, "Navn git: git://gitorious.org:navn/navn.git");
	    log("%s requested Git repository link.", unick.c_str());
	  }
	  if(reply->said(":DCC ")){
	   sock << notice(unick, "I do not accept or support DCC connections.");
	  }
	  if(reply->said("PRIVMSG "+nick+" :kick")){
	    if(unick == owner_nick){
		string kickee = reply->params(2);
		string kickchan = reply->params(1);
		sock << kick(kickchan, kickee, "Kick from %s", unick.c_str());
	    }
	   else{
	    sock << notice(unick, access_denied);
	   }
	  }
	  if(reply->said("PRIVMSG "+nick+" :chown")){
		string givinpass = reply->params(2);
	    if(givinpass == usrpass){
			log("Changing ownership from %s to %s", owner_nick.c_str(), unick.c_str());
			owner_nick = reply->params(1);
			sock << notice(unick, "New owner for \2%s\2 is \2%s\2", nick.c_str(), owner_nick.c_str());
		}else{
			sock << notice(unick, access_denied);
			log("%s attempted to change ownership of the bot", unick.c_str());
		}
	  }
      /*Find the channel join code (366)
      If found it makes in_channel true.
      This is so you can make things happen only if you know you are in
      a channel.
      */
      if(reply->said("252 "+nick)){
	cout << "\033[22;31mChannel join confirmation\033[22;30m... \033[1m\033[22;32mCHECK\033[1m\033[22;36m"<<nl;
	cout << "\033[22;31mReading Config File\033[22;30m... \033[1m\033[22;32mCHECK\033[1m\033[22;36m"<<nl;
	cout << "\033[22;31mSending password to owner\033[22;30m... \033[22;32mCHECK\033[22;36m"<<nl;;
	cout << "\033[22;31mStarted with PID \033[22;32m" << getpid() << "\033[22;36m" << nl;
	cout << "\033[22;34mSession Password: \033[01;32m"+password+"\033[22;36m"<<nl;
	sock << notice(owner_nick, "The randomly generated password is: "+password);
	sock << privmsg(channel, welcome_msg, nick.c_str(), nick.c_str());
        in_channel = true;
      }

   if(reply->said(con_closed_nick)){
		throw SocketException("Nickname is taken.");
      }
      //If someone kicked the bot, alert the owner
      if(reply->said("KICK "+chan+" "+nick)){
		sock << notice(owner_nick, "Someone kicked me!");
		in_channel = false;
		log("%s kicked bot out of channel %s", unick.c_str(), chan.c_str());
      }
}
#endif
