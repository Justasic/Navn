/* main.cpp */
#include "flux_net_irc.hpp"
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
      if (reply->said(pass_req)){
	if (unick == owner_nick){
	  sock << notice(unick, "The password is:\2 "+password);
	  log("%s requested the navn quit password: %s", unick.c_str(), password.c_str());
	} else {
	  sock << notice(unick, access_denied);
	  log("%s attempted to request the navn quit password.", unick.c_str());
	}
      }
	  if(reply->said("This nickname is registered and protected")){
	    sock << privmsg("NickServ", "identify %s %s", nsacc.c_str(), nspass.c_str());
		log("Identified to NickServ with account \"%s\"", nsacc.c_str());
	  }
	  if (reply->said("PRIVMSG "+nick+" :restart")){
	    if(reply->params(2) == password){
	      string reason = "-no reason-";
	      sock << quit("Restarting: "+reason);
	      restart(reason);
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
      if (reply->said(join_req)){
	string blah = reply->params(1);
	if(IsValadChannel(blah)){
	 sock << notice(unick, "Channel %s is not a valad channel.", blah.c_str());
	 log("%s attempted to make bot join %s", unick.c_str(), blah.c_str());
	} else {
		log("%s made the bot join %s", unick.c_str(), blah.c_str());
		sock << join(blah);
		sock << privmsg(blah, welcome_msg);
	}
      }
      if (reply->said(part_req)){
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
		sock << chanmsg(welcome_msg);
		log("%s made the bot rejoin the channel", unick.c_str());
	}else{
	  sock << notice(unick, "The bot is already in the channel.");
	  log("%s attempted to rejoin the bot into a channel its already in.", unick.c_str());
	}
      }
      if (reply->said("JOIN :"+chan) && in_channel){ //welcomes everyone who joins the channel
	   sleep(2);
	   string buf = "Welcome "+unick+" to "+chan+". Type !time for time or \"/msg "+nick+" help\" for help on more commands.";
	   sock << notice(strip(chan), strip(buf));
	   log("%s joined %s", unick.c_str(), strip(chan).c_str());
      }
      if (reply->said(quitmsg_req+" "+password)){ //quits the bot.
		sock << quit("Requested from \2"+ unick +"\017. Pass:\00320 "+password+"\017");
		log("%s quit the bot with password: \"%s\"", unick.c_str(), password.c_str());
		DoQuit();
      }
      if (reply->said(killed)){ // if the bot is killed.. throw a core exception saying so.
	throw CoreException("You have been killed by "+unick);
      }
      if(reply->said("rehash")){
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
	}else{
		sock << notice(unick, access_denied);
		log("%s attempted a rehash.", unick.c_str());

	}
      }
	  if(reply->said("!bugs")){
	    sock.Send(privmsg(chan, "Report Bugs at: http://flux-net.net/bugs/"));
	  }
      //this says that we are now in the server
      if(reply->said(server_welcome)){
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
			sock << notice(unick, "New owner for \2%s\2 is \2%s\2", nick.c_str(), unick.c_str());
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
		cout << "\033[22;31mSending password to owner\033[22;30m... \033[1m\033[22;32mCHECK\033[1m\033[22;36m"<<nl;
		cout << "\033[22;31mStarted with PID \033[22;32m" << getpid() << "\033[22;36m" << nl;
		cout << "\033[01;33mWarning: \033[22;31mNavn is currently started with mode \""<< arg <<"\"\033[22;36m"<<nl;
		cout << "\033[22;34mSession Quit Password: \033[01;32m"+password+"\033[22;36m"<< nl;
		sock << notice(owner_nick, "The quit password is: "+password);
		sock << notice(owner_nick, "\0038Warning: \017\0034\2Navn is currently started in a non-normal mode. ("+arg+")\017");
		sock << chanmsg(welcome_msg);
	} else {
		cout << "\033[22;31mChannel join confirmation\033[22;30m... \033[22;32mCHECK\033[22;36m"<<nl;
		cout << "\033[22;31mSending password to owner\033[22;30m... \033[22;32mCHECK\033[22;36m"<<nl;;
		cout << "\033[22;31mStarted with PID \033[22;32m" << getpid() << "\033[22;36m" << nl;
		cout << "\033[22;34mSession Quit Password: \033[01;32m"+password+"\033[22;36m"<<nl;
		sock << notice(owner_nick, "The quit password is: "+password);
		sock << chanmsg(welcome_msg);
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
		sock << notice(unick, "System Name: %s \tRelease: %s %s \tMachine: %s", uts.nodename, uts.sysname, uts.release, uts.machine );
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
      if(reply->said("cum")){
         sock << privmsg(chan, "ewww..");
      }
      /***********************End Da_Goat Functions*******************************/

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
	sock << privmsg(chan, "Current time around the World:");
	sock << privmsg(chan, "GMT == %2d:%02d", (ptm->tm_hour+UTC)%24, minutes);
	sock << privmsg(chan, "New York (USA) == %2d:%02d", (ptm->tm_hour+EST)%24, minutes);
	sock << privmsg(chan, "California (USA) == %2d:%02d", (ptm->tm_hour+PST)%24, minutes);
	sock << privmsg(chan, "Beijing (China) == %2d:%02d", (ptm->tm_hour+CCT)%24, minutes);	
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
      if(reply->said(kick_msg)){
		sock << notice(owner_nick, "Someone kicked me!");
		in_channel = false;
		log("%s kicked bot out of channel %s", unick.c_str(), chan.c_str());
      }
      //if told to quit, then do so lol
      if(reply->said(quit_req)){
		sock << quit("Requested from \2"+unick+"\017. Pass:\00320 "+password+"\017");
		log("%s quit the bot with password: \"%s\"", unick.c_str(), password.c_str());
		DoQuit(0);
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
