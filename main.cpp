/* main.cpp */
//#include "algos.h"
#include "flux_net_irc.cpp"

using namespace std;
using namespace flux_net_irc;

int main (int argcx, char** argvx)
{
  startup(argcx, argvx);
  try
  {
    cout << "\033[22;31mStarted with PID \033[22;32m" << get_pID() << "\033[22;37m" << nl;
    //Make the socket used to connect to the server
    Socket sock(server,port);
    //Incase there is no connection, say so
    if ( !sock.get_address() ) throw SocketException("Could not resolve the IRC server.");
    if ( !sock.connect() ) throw SocketException("Could not connect to the IRC server.");
    //Accept some server replies after connecting
    sock >> rply;
    irc_string* reply = new irc_string(rply);
    //Set the username and nick
    sock << "USER "+usrname+" * 8 :"+realname+"\r\n";
    sock << setnick(nick);
    log("Successfully Started");

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
      /*for whenever justasic plans on making something with anope string
      if (reply->said("derpo")){
	//Anope::string &fnick = reply->usernick;
	//sock << "PRIVMSG " << chan << " " << fnick << nl;
	sock << chanmsg("lol youre a retard");
      }*/
      if(reply->said("!google") && in_channel){ // if !google is said in the channel
	sock << privmsg(chan, search(rply, "!google"));
	log("Channel Google Search from " + unick + " \"" +search(rply, "!google") + "\"");
      }
      if(reply->said(pmsggoogle)){ //if the message was sent to the bot as a private message.
	sock << notice(unick, search(rply, "google"));
	log("Private message Google Search from " + unick + " \"" + search(rply, "google") + "\"");
      }
      if(reply->said("!youtube") && in_channel){ // if !youtube is said in the channel
	sock << privmsg(chan, search(rply, "!youtube"));
	log("Channel youtube Search from " + unick + " \"" +search(rply, "!youtube") + "\"");
      }
      if(reply->said(pmsgyoutube)){ //if the message was sent to the bot as a private message.
	sock << notice(unick, search(rply, "youtube"));
	log("Private message Google Search from " + unick + " \"" + search(rply, "youtube") + "\"");
      }
      if(reply->said("!tpb") && in_channel){ // if !tpb xor !thepiratebay is said in the channel
	sock << privmsg(chan, search(rply, "!tpb"));
	log("Channel tpb Search from " + unick + " \"" +search(rply, "!youtube") + "\"");
      }
      if(reply->said("!define") && in_channel){ // if !tpb xor !thepiratebay is said in the channel
	sock << privmsg(chan, search(rply, "!define"));
	log("Channel define Search from " + unick + " \"" +search(rply, "!define") + "\"");
      }
      if(reply->said("!urban") && in_channel){ // if !tpb xor !thepiratebay is said in the channel
	sock << privmsg(chan, search(rply, "!urban"));
	log("Channel urban Search from " + unick + " \"" +search(rply, "!urban") + "\"");
      }
      if(reply->said("!movie") && in_channel){ // if !tpb xor !thepiratebay is said in the channel
	sock << privmsg(chan, search(rply, "!movie"));
	log("Channel movie Search from " + unick + " \"" +search(rply, "!urban") + "\"");
      }
      //request the quit password
      if (reply->said(pass_req)){
	if (host == "I.still.hate.packets" || host == "Netadmin.Flux-Net.net"){
	  sock << notice(unick, "The password is:\2 "+password);
	  log(unick + " requested the navn quit password: " + password);
	} else {
	  sock << notice(unick, access_denied);
	  log(unick + " attempted to request the navn quit password.");
	}
      }//gdb info rply
	  if(reply->said("This nickname is registered and protected")){
	    sock << privmsg("NickServ", "identify "+nsacc+" "+nspass);
	  }
      if (reply->said(gdb_req)){
	sock << notice(unick, gdb_msg);
	log(unick + " used the gdb reply.");
      }
      if(reply->said("about me")){
	sock << notice(unick, "Raw: "+reply->raw_string);
	sock << notice(unick, "Nickname: "+unick);
	sock << notice(unick, "Ident: "+reply->user);
	sock << notice(unick, "Host: "+host);
	sock << notice(unick, "Channel: "+chan);
	sock << notice(unick, "Fullhost: "+fullhost);
	log(unick + " requested information about themself.");
      }
      if(reply->said(chan+" :whats my full host?")){
	sock << privmsg(chan, unick+", Your full host is: "+fullhost);
	log(unick + " requested his full host of \"" + fullhost + "\"");
      }
      if (reply->said(join_req)){
	string blah;
	blah = reply->params(1);
	if(IsValadChannel(blah)){
	 sock << notice(unick, "Channel "+blah+" is not a valad channel.");
	 log(unick + " attempted to make bot join " + blah);
	} else {
	log(unick + " made the bot join " + blah);
	sock << join(blah);
	sock << privmsg(blah, welcome_msg);
	}
      }
      if (reply->said(part_req)){
	string blah;
	blah = reply->params(1);
	if(IsValadChannel(blah)){
	 sock << notice(unick, "Channel "+blah+" is not a valad channel.");
	 log(unick + " attempted to make bot part "+blah);
	} else {
	log(unick + " made the bot part "+blah);
	sock << part(blah);
	}
      }
      if (reply->said("rejoin")){ //rejoin the channel
	if(!in_channel){
	sock << join(channel);
	sock << chanmsg(welcome_msg);
	log(unick + " made the bot rejoin the channel");
	}else{
	  sock << notice(unick, "The bot is already in the channel.");
	  log(unick + " attempted to rejoin the bot into a channel its already in.");
	}
      }
      if (reply->said("JOIN :"+chan) && in_channel){ //welcomes everyone who joins the channel
	   cout << chanmsg("Welcome "+unick+" to "+reply->channel+". Type !time for time or \"/msg "+nick+" help\" for help on more commands.");
      }
      if (reply->said(quitmsg_req+" "+password)){ //quits the bot.
	sock << quit("Requested from \2"+ unick +"\017. Pass:\00320 "+password+"\017");
	log(unick + " quit the bot with password: \"" + password + "\"");
      }
      if (reply->said(killed)){ // if the bot is killed.. throw a socket exception saying so.
	throw CoreException("You have been killed by "+unick);
      }
      if(reply->said(":Navn!"+usrname+" NICK ")){
         nick = reply->params(3);
         sock << notice(owner_nick, "Someone changed my nickname to "+nick);
      }
      //this says that we are now in the server
      if(reply->said(server_welcome)){
	sock << mode(nick, "+B");
	sock << join(channel);
	sock << privmsg("NickServ", "identify "+nsacc+" "+nspass);
	log("Successfully connected to the server " +server +" " +port +" " +channel);
      }
      //if the nick is not avalable
      if(reply->said(nick_taken_rsl)){
	log("Nickname: " + nick + " is taken, attempting with " + nick+"_");
	nick = nick+"_";
	sock << setnick(nick);
      }
      if(reply->said(CTCP_VERS)){ // for CTCP VERSION reply
	cout << "\033[22;31mRecieved CTCP VERSION from "+unick+"\033[22;36m\r\n";
	sock << notice(unick, "\001VERSION Navn Bot "+version+"\001");
	log("Recieved CTCP VERSION from " + unick);
      }
      if(reply->said(CTCP_TIME)){ // for CTCP TIME reply
	cout << "\033[22;31mRecieved CTCP TIME from "+unick+"\033[22;36m\r\n";
	sock << notice(unick, "\001TIME "+ get_os_time()+"\001");
	log("Recieved CTCP TIME from " + unick);
      }
      /*Find the channel join code (366)
      If found it makes in_channel true.
      This is so you can make things happen only if you know you are in
      a channel.
      */
      if(reply->said(in_the_channel)){
	if (debug || dev){
	 cout << "\033[22;31mChannel join confirmation\033[22;30m... \033[1m\033[22;32mCHECK\033[1m\033[22;36m"<<nl;
	 cout << "\033[22;31mSending password to owner\033[22;30m... \033[1m\033[22;32mCHECK\033[1m\033[22;36m"<<nl;
	 cout << "\033[22;31mStarted with PID \033[22;32m" << getpid() << "\033[22;36m" << nl;
	 cout << "\033[01;33mWarning: \033[22;31mNavn is currently started in debug mode.\033[22;36m"<<nl;
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
      if(reply->said(help_req)){
	sock << notice(unick, "There are 8 commands:");
	sock << notice(unick, "quit \t \t \tQuits the bot (password needed)");
	sock << notice(unick, "pass\t \t \t Gets the quit password for the bot (need to be a bot admin)");
	sock << notice(unick, "gdb\t \t \t \tDisplays how to use gdb.");
	sock << notice(unick, "rejoin\t \t Rejoins the bot to the channel.");
	sock << notice(unick, "google\t \t Googles something.");
	sock << notice(unick, "youtube \t Youtubes something.");
	sock << notice(unick, "join \t \t \tTells the bot to join the specified channel.");
	sock << notice(unick, "part \t \t \tParts the channel");
	log(unick + " used help command");
      }
      if(reply->said("do a me")){
	sock << me(chan, "/me!");
      }
      /***************************Da_Goat Functions*******************************/
      if(reply->said("!poke")){ //Easter egg ;P
	sock << me(chan, "Is angry at "+unick);
	sock << kick(chan, unick, "\002\00315Dont poke me!\017");
	log(unick+" used Da_Goats !poke command in "+chan);
      }
      if(reply->said("!info")){
	sock << privmsg(chan, "Our forum is at \037castawaycrew.hyperboards.com\017");
	sock << privmsg(chan, "Our Website is \002Flux-Net.net\017");
	sock << privmsg(chan, "Flux-Net SVN is at: \002\0034http://flux-net.googlecode.com/\017");
	sock << privmsg(chan, "Ftp server \002178.63.127.231\002 login anonymous \002-no password-\002, Files in dir \002/ftp/pub\002");
	log(unick+" used Da_Goats !info command in "+chan);
      }
      if(reply->said("!rename")){
	sock << privmsg(chan, "This channel is a Nickname only channel. This means that you MUST have your own Nickname! If you do not choose your own nick name you WILL be kicked.");
	sock << privmsg(chan, "To change your nickname type (without quotes) '/nick MyNewNickname' to change your nickname. (replacing MyNewNickname with a personal nickname).");
	log(unick+" used Da_Goats !rename command in "+chan);
      }
      if(reply->said("!register")){
	sock << privmsg(chan, "To Register you nickname type:");
	sock << privmsg(chan, "\0034If this is the nick you want then skip step 1.\017");
	sock << privmsg(chan, "\0034Do not include brackets when registering, this will cause errors\017");
	sock << privmsg(chan, "Step 1: change your nick \00312/nick [nickname here]\017");
	sock << privmsg(chan, "Step 2: now to begin registration type:");
	sock << privmsg(chan, "\00312/ns register [\037YourPasswordHere\017] [\002YouremailHere\015]");
	sock << privmsg(chan, "Step 3: Then to identify so you can have op status type:");
	sock << privmsg(chan, "\00312/identify [passwordhere]\017");
	sock << privmsg(chan, "\0034You will need to indetify everytime you join into the chatroom\017");
	sock << privmsg(chan, "Unless your client does it automatically (ei. xchat, mIRC, iceChat).\017");
	log(unick+" used Da_Goats !register command in "+chan);
      }
      if(reply->said("!socialinfo")){
	sock << privmsg(chan, "Ventrilo Server:\002 5.110.166.75 Port:\00313 3784");
	sock << privmsg(chan, "Our IRC server: \002irc.Flux-Net.net\002 port\00313 6667\017 or\00313 8067\017");
	log(unick+" used Da_Goats !socialinfo command in "+chan);
      }
      if(reply->said("!help")){
	sock << privmsg(chan, "Local "+chan+ " commands are:");
	sock << privmsg(chan, "!help !info !register !socialinfo !version");
	sock << privmsg(chan, "!changelog !uptime !rules !spam !rename");
	log(unick+" used Da_Goats !help command in "+chan);
      }
      if(reply->said("!uptime")){
	/*
	if(sysinfo(&sys_info) != 0)
	  perror("sysinfo");

	// Uptime
        days = sys_info.uptime / 86400;
	hours = (sys_info.uptime / 3600) - (days * 24);
	mins = (sys_info.uptime / 60) - (days * 1440) - (hours * 60);

	sock << privmsg(chan, "Uptime: "+days+", "+hours+", "+mins+", "+);
	printf("Uptime: %ddays, %dhours, %dminutes, %ldseconds\n",
                      days, hours, mins, sys_info.uptime % 60);

	// Load Averages for 1,5 and 15 minutes
	sock << privmsg(chan, "Load Avgs: 1min("+sys_info.loads[0]+") 5mins("+sys_info.loads[1]+") 15mins("+sys_info.loads[2]+")");

	// Total and free ram.
	sock << privmsg(chan, "Total Ram: "+sys_info.totalram / 1024+"\tFree: "+sys_info.freeram / 1024);

	// Shared and buffered ram.
	sock << privmsg(chan, "Shared Ram: "+sys_info.sharedram / 1024);
	sock << privmsg(chan, "Buffered Ram: "+sys_info.bufferram / 1024);

	// Swap space
	sock << privmsg(chan, "Total Swap: "+sys_info.totalswap / 1024+"\tFree: "+sys_info.freeswap / 1024);

	// Number of processes currently running.
	sock << privmsg(chan, "Number of processes: "+sys_info.procs);
	*/
	log(unick+" used Da_Goats !uptime command in "+chan);
	sock << privmsg(chan, "This function needs work");
      }
      if(reply->said("!rules")){
	sock << privmsg(chan, "There are only a few simple rules for "+chan+".");
        sock << privmsg(chan, "Do NOT hate on others in any way. Basically do not troll in any shape or form.");
        sock << privmsg(chan, "Do not ask for op status. you will be granted op status when the moderators feel you deserve op status.");
        sock << privmsg(chan, "Do not Mini-mod the chatroom. The moderators are there for a reason, we don't need more.");
        sock << privmsg(chan, "Do not spam the chatroom. This includes flooding by text, private messages, join/quit commands, External links, etc.");
	sock << privmsg(chan, "If you violate any of these rules you will be kicked and possably banned from "+chan+".");
	log(unick+" used Da_Goats !rules command");
      }
      if(reply->said("!spam")){
	log(unick+" used Da_Goats !spam command in "+chan);
        sock << privmsg(chan, "Spam is the abuse of electronic messaging systems. This includes (but not limited to) external links, Flooding, mass join/quit messages, mass private messages or notices, mIRC color code abuse, CTCP abuse, mass nick changes, etc. If you violate the spam policy you will be kicked.");
      }
      if(reply->said("!version")){
        sock << privmsg(chan, "The Current Navn Bot Version is \002\0037"+version);
        sock << privmsg(chan, "Dah_Goat's old script was part python(xchat) and mIRC");
        sock << privmsg(chan, "Navn (which includes Dah_Goat) is Full C++ coded from scratch");
	sock << privmsg(chan, "Navn's Code can be found at \00312http://flux-net.googlecode.com/");
        sock << privmsg(chan, "Navn is managed by Lordofsraam and "+owner_nick);
	sock << privmsg(chan, "If you would like to add a command or function, talk to them.");
	log(unick+" used Da_Goats !version command in "+chan);
      }
      /*******************************Easter Eggs*********************************/
      if(reply->said("!everything")){
	sock << privmsg(chan, "Yes, there is a script for everything..\007");
      }
      if(reply->said(":\001ACTION "+unick+" hugs "+nick)){
         sock << me(chan, "Hugs "+unick);
      }
      if(reply->said(":\001ACTION "+unick+" kicks "+nick)){
         sock << privmsg(chan, "Ouch!");
      }
      if(reply->said(":\001ACTION "+unick+" eats "+nick)){
         sock << privmsg(chan, "how do I taste?");
      }
      if(reply->said(":\001ACTION "+unick+" licks "+nick)){
         sock << me(chan, "is a little creeped out that "+unick+" licked him");
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
	string dd_minutes = make_two_digits(minutes);
	sock << privmsg(chan, "Current time around the World:");

	int GMT_h = ((ptm->tm_hour+UTC)%24);
	sock << privmsg(chan, "GMT == " + make_two_digits(GMT_h) + ":" + dd_minutes);

	int new_york_h = ((ptm->tm_hour+EST)%24);
	if (new_york_h < 0){
	  new_york_h += 24;
	}
	sock << privmsg(chan, "New York (USA) == " + make_two_digits(new_york_h) + ":" + dd_minutes);

	int cali_h = ((ptm->tm_hour+PST)%24);
	if (cali_h < 0){
	  cali_h += 24;
	}
	sock << privmsg(chan, "California (USA) == " +  make_two_digits(cali_h) + ":" + dd_minutes);

	int beijing_h = ((ptm->tm_hour+CCT)%24);
	if (beijing_h < 0){
	  beijing_h += 24;
	}
	sock << privmsg(chan, "Beijing (China) == " + make_two_digits(beijing_h) + ":" + dd_minutes);
	log(unick + " requested !time command in " + chan);
      }
      //if the nick is taken and the server connection is terminated.
      if(reply->said(con_closed_nick)){
	throw SocketException("Nickname is taken.");
      }
      //If someone kicked the bot, alert the owner
      if(reply->said(kick_msg)){
	sock << notice(owner_nick, "Someone kicked me!");
	in_channel = false;
	log(unick + " kicked bot out of channel " + chan);
      }
      //if told to quit, then do so lol
      if(reply->said(quit_req)){
	sock << quit("Requested from \2"+ unick +"\017. Pass:\00320 "+password+"\017");
	log(unick +" quit the bot with password: \"" +password +"\"");
	do_quit();
      }
      delete reply;
    }//while loop ends here
     sock << quit(quitmsg); //quit the bot if we had a terminal signal.
     log("Quitting: "+quitmsg);
     do_quit();
  }//try ends here
  catch ( SocketException& e ) //catch any SocketExceptions sent.
  {
    cout << "\r\nSocket Exception was caught: \033[22;31m" << e.description() << "\033[22;37m" << nl;
    log("Socket Exception Caught: "+ e.description());
    do_quit(1);
  }
  catch(CoreException& e){
    cout << "\r\nCore Exception was caught: \033[22;31m" << e.GetReason() << "\033[22;37m" << nl;
    stringstream RCV;
    RCV << e.GetReason();
    log("Core Exception Caught: "+RCV.str());
    do_quit(1);
  }
  log("Logging ended at "+get_os_time());
  return EXIT_SUCCESS;
}
