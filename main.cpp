/* main.cpp */
#include "flux_net_irc.hpp"
#include "da_goat.h"
#include "dns.h"
string binary_path, services_dir, services_bin;
using namespace std;
using namespace flux_net_irc;

int main (int argcx, char** argvx, char *envp[])
{
  my_av = argvx;
  my_envp = envp;
  try{
  INIReader config("bot.conf");
    if (config.ParseError() < 0) {
       throw ConfigException("Cannot load bot.conf");
    }
  ReadConfig(config);
  }catch(ConfigException &ex){
    cout << "\r\nConfig Exception was caught: \033[22;31m" << ex.GetReason() << "\033[22;37m" << nl;
    log("Config Exception Caught: ", stringify(ex.GetReason()).c_str());
    DoQuit(1);
  }
  startup(argcx, argvx);
  try
  {
    cout << "\033[22;31mStarted with PID \033[22;32m" << getpid() << "\033[22;37m" << nl;
    //Make the socket used to connect to the server
    Socket sock(server,port);
    //Incase there is no connection, say so
    if ( !sock.get_address() ) throw SocketException("Could not resolve the IRC server.");
    if ( !sock.connect() ) throw SocketException("Could not connect to the IRC server.");
    //Accept some server replies after connecting
    sock >> rply;
    irc_string* reply = new irc_string(rply);
    //Set the username and nick
    sock << "USER "+usrname+" * * :"+realname+nl;
    sock << setnick(nick);

    if (reply->said("PING :")){
      sock << "PONG :"+rply.substr(6,-1);
    }
    
    sock >> rply;
    string line;
    struct sigaction act; // to catch a signal sent from the terminal.
    act.sa_handler = sigact;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGINT, &act, 0);

    delete reply;

    while (!quitting){ //infi loop to stay connected
      sock >> rply; //keep the connection messages in the rply string.

      irc_string* reply = new irc_string(rply);
      host = reply->host;
      fullhost = reply->usernick+"!"+reply->user+"@"+host;
      chan = reply->channel; //sets the variables.
      unick = reply->usernick;
      msg = reply->message;
      ident = reply->user;
      //keep the connection active
      if (reply->said("PING :")){
		sock << "PONG :"+rply.substr(6,-1);
      }
      if(reply->said("!google") && in_channel){ // if !google is said in the channel
		sock << privmsg(chan, search(rply, "!google"));
		log("Channel Google Search from %s \"%s\"", unick.c_str(), search(rply, "!google").c_str());
      }
      if(reply->said(pmsggoogle)){ //if the message was sent to the bot as a private message.
		sock << notice(unick, search(rply, "google"));
		log("Private message Google Search from %s \"%s\"", unick.c_str(), search(rply, "google").c_str());
      }
      if(reply->said("!youtube") && in_channel){ // if !youtube is said in the channel
		sock << privmsg(chan, search(rply, "!youtube"));
		log("Channel youtube Search from %s \"%s\"", unick.c_str(), search(rply, "!youtube").c_str());
      }
      if(reply->said(pmsgyoutube)){ //if the message was sent to the bot as a private message.
		sock << notice(unick, search(rply, "youtube"));
		log("Private message Google Search from %s \"%s\"", unick.c_str(), search(rply, "youtube").c_str());
      }
      if(reply->said("!tpb") && in_channel){ // if !tpb xor !thepiratebay is said in the channel
		sock << privmsg(chan, search(rply, "!tpb"));
		log("Channel tpb Search from %s \"%s\"", unick.c_str(), search(rply, "!tpb").c_str());
      }
      if(reply->said("!define") && in_channel){
		sock << privmsg(chan, search(rply, "!define"));
		log("Channel define Search from %s \"%s\"", unick.c_str(), search(rply, "!define").c_str());
      }
      if(reply->said("!urban") && in_channel){ 
		sock << privmsg(chan, search(rply, "!urban"));
		log("Channel urban Search from %s \"%s\"", unick.c_str(), search(rply, "!urban").c_str());
      }
      if(reply->said("!movie") && in_channel){ 
		sock << privmsg(chan, search(rply, "!movie"));
		log("Channel movie Search from %s \"%s\"", unick.c_str(), search(rply, "!movie").c_str());
      }
      //request the quit password
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
      if (reply->said(gdb_req)){//gdb info rply
		sock << notice(unick, gdb_msg);
		log("%s used the gdb reply.", unick.c_str());
      }
      if(reply->said("about me")){
		sock << notice(unick, "Raw: "+reply->raw_string);
		sock << notice(unick, "message: "+msg);
		sock << notice(unick, "Nickname: "+unick);
		sock << notice(unick, "Ident: "+reply->user);
		sock << notice(unick, "Host: "+host);
		sock << notice(unick, "Channel: "+chan);
		sock << notice(unick, "Fullhost: "+fullhost);
		log("%s requested information about themself.", unick.c_str());
      }
	  if(reply->said("? weather")){
		string area = reply->params(3);
	        sock << privmsg(chan, get_weather(area));
		log("%s used weather command to get the weather for %s in %s", unick.c_str(), area.c_str(), chan.c_str());
	  }
      if (reply->said("PRIVMSG "+nick+" :join")){
	string blah = reply->params(1);
	if(IsValadChannel(blah)){
	 sock << notice(unick, "Channel %s is not a valad channel.", blah.c_str());
	 log("%s attempted to make bot join %s", unick.c_str(), blah.c_str());
	} else {
		log("%s made the bot join %s", unick.c_str(), blah.c_str());
		sock << join(blah);
		sock << privmsg(blah, welcome_msg, nick.c_str());
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
      if (reply->said("PRIVMSG "+nick+" :pass "+password)){ //quits the bot.
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
	cout << "Rehashing config file." << nl;
	log("%s rehashed config file.", unick.c_str());
	try{
  	 INIReader config("bot.conf");
    	 if (config.ParseError() < 0) {
       		throw ConfigException("Cannot load bot.conf");
    	 }
  	 ReadConfig(config);
  	 }catch(ConfigException &ex){
    		cout << "\r\nConfig Exception was caught: \033[22;31m" << ex.GetReason() << "\033[22;37m" << nl;
    		log("Config Exception Caught: %s", stringify(ex.GetReason()).c_str());
    		sock << notice(unick, "Config Exception Caught: %s", stringify(ex.GetReason()).c_str());
  	 }
	cout << "\033[22;31mReading Config File\033[22;30m... \033[1m\033[22;32mCHECK\033[1m\033[22;36m"<<nl;
	}else{
		sock << notice(unick, access_denied);
		log("%s attempted a rehash.", unick.c_str());

	}
      }
	  if(reply->said("!bugs")){
	    sock.Send(privmsg(chan, "Report Bugs at: http://flux-net.net/bugs/"));
	  }
      //this says that we are now in the server
      if(reply->said("005 "+nick)){
		sock << mode(nick, "+B");
		sock << join(channel);
		sock << privmsg("NickServ", "identify "+nsacc+" "+nspass);
		log("Successfully connected to the server \"%s\" Port: %s Master Channel: %s", server.c_str(), port.c_str(), channel.c_str());
      }
	  if(reply->said("? git")){
	    sock << notice(unick, "Navn git: git@gitorious.org:navn/navn.git");
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
      if(reply->said(CTCP_VERS)){ // for CTCP VERSION reply
		cout << "\033[22;31mRecieved CTCP VERSION from "+unick+"\033[22;36m\r\n";
		struct utsname uts;
		if(uname(&uts) < 0)
			//perror("uname() error");
			throw CoreException("uname() Error");

		sock << notice(unick, "\001VERSION Navn Bot %s %s %s\001",version.c_str(), uts.sysname, uts.machine);
		log("Recieved CTCP VERSION from %s", unick.c_str());
      }
      if(reply->said(CTCP_TIME)){ // for CTCP TIME reply
		cout << "\033[22;31mRecieved CTCP TIME from "+unick+"\033[22;36m\r\n";
		sock << notice(unick, "\001TIME "+os_time()+"\001");
		log("Recieved CTCP TIME from %s", unick.c_str());
      }
      /*Find the channel join code (366)
      If found it makes in_channel true.
      This is so you can make things happen only if you know you are in
      a channel.
      */
      if(reply->said("252 "+nick)){
	if (dev){
		cout << "\033[22;31mChannel join confirmation\033[22;30m... \033[1m\033[22;32mCHECK\033[1m\033[22;36m"<<nl;
		cout << "\033[22;31mReading Config File\033[22;30m... \033[1m\033[22;32mCHECK\033[1m\033[22;36m"<<nl;
		cout << "\033[22;31mSending password to owner\033[22;30m... \033[1m\033[22;32mCHECK\033[1m\033[22;36m"<<nl;
		cout << "\033[22;31mStarted with PID \033[22;32m" << getpid() << "\033[22;36m" << nl;
		cout << "\033[01;33mWarning: \033[22;31mNavn is currently started with mode \""<< arg <<"\"\033[22;36m"<<nl;
		cout << "\033[22;34mSession Password: \033[01;32m"+password+"\033[22;36m"<< nl;
		sock << notice(owner_nick, "The randomly generated password is: "+password);
		sock << notice(owner_nick, "\0038Warning: \017\0034\2%s is currently started in a non-normal mode. (%s)\017", nick.c_str(), arg.c_str());
		sock << privmsg(channel, welcome_msg, nick.c_str());
	} else {
		cout << "\033[22;31mChannel join confirmation\033[22;30m... \033[1m\033[22;32mCHECK\033[1m\033[22;36m"<<nl;
		cout << "\033[22;31mReading Config File\033[22;30m... \033[1m\033[22;32mCHECK\033[1m\033[22;36m"<<nl;
		cout << "\033[22;31mSending password to owner\033[22;30m... \033[22;32mCHECK\033[22;36m"<<nl;;
		cout << "\033[22;31mStarted with PID \033[22;32m" << getpid() << "\033[22;36m" << nl;
		cout << "\033[22;34mSession Password: \033[01;32m"+password+"\033[22;36m"<<nl;
		sock << notice(owner_nick, "The randomly generated password is: "+password);
		sock << privmsg(channel, welcome_msg, nick.c_str());
        }
        in_channel = true;
      }
      //help replies
	if(reply->said("PRIVMSG "+nick+" :help")){
		sock << notice(unick, "There are 13 commands:");
		sock << notice(unick, "quit \t \t \tQuits the bot (password needed)");
		sock << notice(unick, "pass\t \t \t Gets the quit password for the bot (must be a bot admin)");
		sock << notice(unick, "gdb\t \t \t \tDisplays how to use gdb.");
		sock << notice(unick, "rejoin\t \t Rejoins the bot to the channel.");
		sock << notice(unick, "chown \t \t Changes ownership over the bot (must be a bot admin)");
		sock << notice(unick, "google\t \t Googles something.");
		sock << notice(unick, "youtube \t Youtubes something.");
		sock << notice(unick, "join \t \t \tTells the bot to join the specified channel.");
		sock << notice(unick, "part \t \t \tParts the channel");
		sock << notice(unick, "kick \t \t \tkicks a user from the channel (must be bot owner)");
		sock << notice(unick, "restart \t Restarts the Bot (Password needed)");
		sock << notice(unick, "rehash\t \t Rehashes the Bot (Password needed)");
		sock << notice(unick, "stats \t \t Shows system statistics.");
		log("%s used help command", unick.c_str());
      }
	Da_Goat(sock, rply);
	if(reply->said("!rdns")){
	 string ip = reply->params(1);
	 rdns(sock, rply, chan, ip);
	}
	if(reply->said("!dns")){
	 string host = reply->params(1);
	 dns(sock, chan, host);
	}
      //If it looks for !time in a server rply (aka if anyone in the channel
      //says it) and if it finds it, it tells the time.
      if (reply->said("!time") && in_channel){

	/*WORLD CLOCK
	Since this is all C++ and not IRC or socket stuff, I won't bother
	trying to explain it. (Mainly because if youre trying something
	like this you should already have a decent grasp on C++, and partly
	because I'm lazy)
	*/
	time_t rawtime;
	tm * ptm;
	time ( &rawtime );
	ptm = gmtime ( &rawtime );
	int minutes = (ptm->tm_min);
	int seconds = (ptm->tm_sec);
	sock << privmsg(chan, "Current time around the World:");
	sock << privmsg(chan, "GMT == %2d:%02d:%02d", (ptm->tm_hour+UTC)%24, minutes, seconds);
	sock << privmsg(chan, "New York (USA) == %2d:%02d:%02d", (ptm->tm_hour+EST)%24, minutes, seconds);
	sock << privmsg(chan, "California (USA) == %2d:%02d:%02d", (ptm->tm_hour+PST)%24, minutes, seconds);
	sock << privmsg(chan, "Beijing (China) == %2d:%02d:%02d", (ptm->tm_hour+CCT)%24, minutes, seconds);
	sock << privmsg(chan, "Sydney (Australia) == %2d:%02d:%02d", (ptm->tm_hour+AUS)%24, minutes, seconds);	
	char buf[100];
	ptm = localtime(&rawtime);
	strftime(buf,100,"Navn's Time: %Z %c",ptm);
	sock << privmsg(chan, buf);
	log("%s requested !time command in %s", unick.c_str(), chan.c_str());
      }
      //if the nick is taken and the server connection is terminated.
      if(reply->said(con_closed_nick)){
		throw SocketException("Nickname is taken.");
      }
      //If someone kicked the bot, alert the owner
      if(reply->said("KICK "+chan+" "+nick)){
		sock << notice(owner_nick, "Someone kicked me!");
		in_channel = false;
		log("%s kicked bot out of channel %s", unick.c_str(), chan.c_str());
      }
      delete reply;
    }//while loop ends here
     sock << quit(quitmsg); //quit the bot if we had a terminal signal.
     log("Quitting: %s", quitmsg.c_str());
     DoQuit(0);
  }//try ends here
  catch ( SocketException& e ) //catch any Exceptions sent.
  {
    cout << "\r\nSocket Exception was caught: \033[22;31m" << e.description() << "\033[22;37m" << nl;
    log("Socket Exception Caught: %s", e.description().c_str());
    DoQuit(1);
  }
  catch(CoreException& e){
    cout << "\r\nCore Exception was caught: \033[22;31m" << e.GetReason() << "\033[22;37m" << nl;
    log("Core Exception Caught: ", stringify(e.GetReason()).c_str());
    DoQuit(1);
	}
  return EXIT_SUCCESS;
}
