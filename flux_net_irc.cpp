#ifndef flux_net_irc_ns_h
#define flux_net_irc_ns_h
#include "defs.h"
#include "includes.h"

template<typename T> inline std::string stringify(const T &x){
	std::ostringstream stream;
	if(!(stream << x))
		throw CoreException("Stringify Fail");
	return stream.str();
}

namespace flux_net_irc{
  
  using namespace std;
  /** irc_string Class
 * Lordofsraam must document this
 * NOTE this MUST be used in the main file.
 * @param variable->params(#)
 * @param variable->raw_string
 * @param variable->usernick
 * @param variable->host
 * @param variable->user
 * @param variable->channel
 * @param variable->message
 * @param variable->said(whats-said)
 */
class irc_string:string{
  private :
    vector<string> toks;
  public :
    string raw_string;
    string usernick;
    string host;
    string user;
    string channel;
    string message;
       
    irc_string(string reply){
      raw_string = reply;
      usernick = isolate(':','!',reply);
      host = isolate('@',' ',reply);
      user = isolate('!','@',reply);
      channel = '#'+isolate('#',' ',reply);
      
      size_t pos = reply.find(" :");
      pos += 2;
      for (int i = pos; i < reply.length(); i++){
	if (reply.at(i) == ' '){
	  message = message+' ';
	}else{message = message+reply.at(i);}
      }
      if(message.size()>2){
	message.resize(message.size()-2);
      }
      string fmessage = message;
      char * cmessage = (char *)fmessage.c_str();
      char * pch;
      pch = strtok (cmessage," ");
      while (pch != NULL)
      {
	toks.push_back(pch);
	pch = strtok (NULL, " ");
      }
    }
    
    string params(int i){
      if (i >= toks.size()){
	return " ";
      }else{return toks[i];}
    }
    
    string isolate(char begin, char end, string msg){
      string to_find;
      size_t pos = msg.find(begin);
      pos += 1;
      for (int i = pos; i < msg.length(); i++){
	if (msg.at(i) == end){
	  break;
	}else{to_find = to_find+msg.at(i);}
      }
      return to_find;
    }
    
    bool said(string findee){
      int i = raw_string.find(findee);
      if (i != string::npos){
	return true;
      }else{return false;}
    }

};

/** PID handler
 * Gets the Process ID of the process or parent process
 * @param getpid()
 * @param getppid()
 */
pid_t get_pID(){
return getpid();
}
pid_t getppid(void);

/** Search Function
 * This is what generates the search links
 * @param search(origonal-string, command-used) 
 */
string search(string s, string command){
  string raw_searchstring;
  string searchstring;
  size_t pos = s.find(command);
  raw_searchstring = s.substr(pos+(command.size())+1);
  for (int i=0; i < raw_searchstring.length(); i++){
    if (raw_searchstring.at(i) == ' '){
      searchstring = searchstring+"%20";
    }else if (raw_searchstring.at(i) == '+'){
     searchstring = searchstring+"%2B"; 
     }else if (raw_searchstring.at(i) == '$'){
     searchstring = searchstring+"%24"; 
     }else if (raw_searchstring.at(i) == '&'){
     searchstring = searchstring+"%26"; 
     }else if (raw_searchstring.at(i) == ','){
     searchstring = searchstring+"%2C"; 
     }else if (raw_searchstring.at(i) == '/'){
     searchstring = searchstring+"%2F"; 
     }else if (raw_searchstring.at(i) == ':'){
     searchstring = searchstring+"%3A";
     }else if (raw_searchstring.at(i) == ';'){
     searchstring = searchstring+"%3B"; 
     }else if (raw_searchstring.at(i) == '='){
     searchstring = searchstring+"%3D";
     }else if (raw_searchstring.at(i) == '?'){
     searchstring = searchstring+"%3F";
     }else if (raw_searchstring.at(i) == '@'){
     searchstring = searchstring+"%40"; 
     }else if (raw_searchstring.at(i) == '#'){
     searchstring = searchstring+"%23";
     }else if (raw_searchstring.at(i) == '>'){
     searchstring = searchstring+"%3E";
     }else if (raw_searchstring.at(i) == '<'){
     searchstring = searchstring+"%3C";
     }else if (raw_searchstring.at(i) == '%'){
     searchstring = searchstring+"%25";
     }else if (raw_searchstring.at(i) == '{'){
     searchstring = searchstring+"%7B";
     }else if (raw_searchstring.at(i) == '}'){
     searchstring = searchstring+"%7D"; 
     }else if (raw_searchstring.at(i) == '|'){
     searchstring = searchstring+"%7C"; 
     }else if (raw_searchstring.at(i) == '\\'){
     searchstring = searchstring+"%5C"; 
     }else if (raw_searchstring.at(i) == '^'){
     searchstring = searchstring+"%5E"; 
     }else if (raw_searchstring.at(i) == '~'){
     searchstring = searchstring+"%7E";
     }else if (raw_searchstring.at(i) == '['){
     searchstring = searchstring+"%5B";
     }else if (raw_searchstring.at(i) == ']'){
     searchstring = searchstring+"%5D";
     }else if (raw_searchstring.at(i) == '`'){
     searchstring = searchstring+"%60"; 
    }else{searchstring = searchstring+raw_searchstring.at(i);}
  }
  if (searchstring.empty()){
    return "Empty search string.";
  }else{
    if(command == "!google"){
      return "http://www.google.com/search?q="+searchstring;
    }else if(command == "!youtube"){
      return "http://www.youtube.com/results?search_query="+searchstring;
    }else if(command == "!tpb"){
      return "http://thepiratebay.org/search/"+searchstring;
    }else if(command == "!define"){
      return "http://dictionary.reference.com/browse/"+searchstring;
    }else if(command == "!urban"){
      return "http://www.urbandictionary.com/define.php?term="+searchstring;
    }else if(command == "!movie"){
      return "www.letmewatchthis.ch/index.php?search_keywords="+searchstring;
    }else{
      return "http://www.google.com/search?q="+searchstring;
    }
  }
}

string make_two_digits(int x){
  if (x < 10){
    stringstream dd_ss;
    dd_ss << "0" << x;
    string dd_time = dd_ss.str();
    return dd_time;
  }else{
    stringstream sd_ss;
    sd_ss << x;
    string sd_time = sd_ss.str();
    return sd_time;
  }
}
/** Get the operating systems time
 * This is just a simple function that gets the time
 * @param os_time()
 */
string os_time(){
  time_t rawtime;
  time ( &rawtime );
  return ctime (&rawtime);
}
/** The Log
 * This is what logs everything that goes on with the bot
 * @param log(Message)
 */
void log(string message){
  if (message == "navn start 5047"){ //needed to keep the time from sending /r/n and the var nl from doing the same
   fstream log;
   log.open(logfile, fstream::in | fstream::out | fstream::app);
   log << "Navn started " << os_time();
   log.close();
  }else{
   fstream log;
   log.open(logfile, fstream::in | fstream::out | fstream::app);
   log << "[ "<< os_time() << " ] "<< message << nl;
   log.close();
  }
}
/**Channel Validation
 * This function returns if the channel is valid or not.
 * @param isValadChannel(channel)
 */
bool IsValadChannel(const string nerp){
 if (nerp[0] != '#')
    return true;
 return false;
}
/** Password generator
 * This generates a 5 number random password for the bots
 * quit and other password protected commands
 * @param make_pass()
 */
string make_pass(){
  int p1,p2,p3,p4,p5;
  srand(time(NULL));
  p1 = rand()%10; 
  p2 = rand()%10;
  p3 = rand()%10;
  p4 = rand()%10;
  p5 = rand()%10;
  stringstream pass_ss;
  pass_ss << p1 << p2 << p3 << p4 << p5;
  string pass_str = pass_ss.str();
  return pass_str;
}
const string password = make_pass();
const string quit_req = "NOTICE "+nick+" :!quit "+password;

/** Kick Function
 * Handles kick requests
 * NOTE: must be used with 'sock'
 * @param kick(channel, nickname, reason)
 */
string kick(string channel, string user, string reason){
 stringstream kick_ss;
 kick_ss << "KICK " << channel << " " << user <<" "<< reason << nl;
 return kick_ss.str();
}
/** Action handler
 * handles the IRC action function (/me)
 * NOTE: must be used with 'sock'
 * @param act(destination, message)
 */
string me(string dest, string message){
 stringstream me_ss;
 me_ss << "PRIVMSG " << dest << " :\001ACTION " << message << "\001" << nl;
 return me_ss.str();
}
/** Privmsg Function
 * Sends a PRIVMSG to a user, service, or channel
 * NOTE: must be sent with 'sock'
 * @param privmsg(destination, message)
 */
string privmsg(string stringy1, string stringy2){
  stringstream stringy_ss_privmsg;
  stringy_ss_privmsg << "PRIVMSG " << stringy1 << " " << stringy2 << nl;
  return stringy_ss_privmsg.str();
}
/** Channel message Function
 * This function sends a channel message to the pre-defined
 * channel in the defs.h file.
 * NOTE: must be sent with 'sock'
 * @param chanmsg(Message)
 */
string chanmsg(string stringy){
  stringstream stringy_ss;
  stringy_ss << "PRIVMSG " << channel << " " << stringy << nl;
  return stringy_ss.str();
}
/** Set Nickname Function
 * Sets the bots nickname in IRC.
 * NOTE: must be sent with 'sock'
 * @param setnick("nickname")
 */
string setnick(string nickname){
 stringstream nick_ss;
 nick_ss << "NICK " << nickname << nl;
 return nick_ss.str();
}
/** Join channel function
 * Makes the bot join a channel
 * NOTE: must be sent with 'sock'
 * @param join("#channel")
 */
string join(string stringy_chan){
  stringstream stringy_join_ss;
  stringy_join_ss << "JOIN " << stringy_chan << nl;
  return stringy_join_ss.str();
}
/** Part Function
 * Sends part with message
 * NOTE: must be used with 'sock'
 * @param part(channel, reason)
 */
string part(string channel, string reason){
  stringstream part_ss;
  part_ss << "PART " << channel << " " << reason << nl;
  return part_ss.str();
}
string part(string channel){
  stringstream part_ss;
  part_ss << "PART "<< channel << nl;
  return part_ss.str();
}
/** Whois Function
 * Sends a IRC Whois to Server.
 * NOTE: must be sent with 'sock'
 * @param whois(nickname)
 */
 string whois(string Nick){
  stringstream whois_ss;
  whois_ss << "WHOIS " << Nick << nl;
  return whois_ss.str();
 }
/** Notice Function
 * Sends a IRC notice to a user or channel
 * NOTE: must be sent with 'sock'
 * @param notice(Destination, message)
 */
string notice(string Destination, string Message){
  stringstream notice_ss;
  notice_ss << "NOTICE " << Destination << " " << Message << nl;
  return notice_ss.str();
}
/** Mode Function
 * Sends a mode to be set in IRC
 * NOTE: must be sent with 'sock'
 * @param mode(Destination, mode)
 */
string mode(string nickname, string mode){
  stringstream stringy_ss_mode;
  stringy_ss_mode << "MODE " << nickname << " " << mode << nl;
  string out_put = stringy_ss_mode.str();
  return out_put;
}
/** Channel Mode Function
 * Sends a channel mode to the IRC network
 * @param chmode(channel, mode, username)
 */
string chmode(string channel, string mode, string username){
  stringstream stringy_ss_mode;
  stringy_ss_mode << "MODE " << channel << " " << mode << " " << username << nl;
  string out_put = stringy_ss_mode.str();
  return out_put;
}
/** Quit Function
 * Sends a disconnect message to IRC
 * @param quit(reason)
 */
string quit(string msg){
  stringstream msg_ss;
  msg_ss << "QUIT " << msg << nl;
  if (!quitting){
  cout << "\033[01;31mRequested Quit from "+unick+". Pass: "+password+" \033[22;37m\r\n";
  }
  return msg_ss.str();
}
void DoQuit(int K){
  log("Logging ended at "+os_time());
  remove("Navn.pid");
  if(K > 0){
  exit(K);
  }else{
   exit(0);
  }
}
void DoQuit(){
  log("Logging ended at "+os_time());
  remove("Navn.pid");
  exit(0);
}
/** Restart Function
 * Restarts the Bot
 * @param restart(message)
 */
void restart(string reason){
   #define GetCurrentDir getcwd
   char CurrentPath[FILENAME_MAX];
   GetCurrentDir(CurrentPath, sizeof(CurrentPath));
 if(reason.empty()){
  reason = "-no reason-";
  log("Restarting: "+reason);
  chdir(CurrentPath);
  execvp(my_av[0], my_av);
  exit(1);
  }else{
    log("Restarting: "+reason);
	chdir(CurrentPath);
    execvp(my_av[0], my_av);
	exit(1);
  }
}
/**This is the startup sequence that starts before the try loop starts
 * @param startup(int, char)
 */
void startup(int argc, char** argv) {
  //gets the command line paramitors if any.
  if(argv[1] != NULL){
    arg = argv[1];
    if(arg == "--developer" || arg == "--dev" || arg == "-d")
    {
      dev = true;
	 log("Navn is started in Developer mode. (" +arg+ ")");
    }
    else if (arg == "--debug" || arg == "-D"){ // for possable future use
      debug = true;
      log("Navn is started in Debug mode. (" + arg + ")");
    }
    else if (arg == "--help" || arg == "-h"){
     help();
    }
	else if (arg == "--version" || arg == "-v"){
	 cout << "Navn IRC C++ Bot Version " << version << nl;
	 cout << "This bot was programmed from scratch by Justasic and Lordofsraam." << nl;
	 cout << nl;
	 cout << "IRC: irc.Flux-Net.net #Computers" << nl;
	 cout << "WWW: http://www.Flux-Net.net" << nl;
	 cout << "Email: staff@flux-net.net" << nl;
	 cout << "Git: git@gitorious.org:navn/navn.git" << nl;
	 cout << nl;
	 cout << "This bot does have Epic Powers." << nl;
	 cout << "Type ./navn --help for help on how to use navn, or read the readme." << nl;
	 exit(0);
	}
  }
  //logging to a text file and making the PID file.
   log("navn start 5047");
   fstream pid;
   pid.open("Navn.pid", fstream::in | fstream::out | fstream::app);
   pid << getpid();
   pid.close();
   stringstream h;
   h << getpid();
   string U;
   U = h.str();
   log("Created PID file. PID: "+U);
   
   if (debug || dev){
  } else {
  cout << "What server would you like to connect to?" << nl;
  cin >> server;
  cout << "What port?" << nl;
  cin >> port;
  cout << "Channel?" << nl;
  cin >> channel;
  cout << "Navn Bots Nickname?" << nl;
  cin >> nick; 
  cout << "Owner's Nickname?" << nl;
  cin >> owner_nick;
  }
}
/**Random Number Generator
 * This will generate a random number x is start number, y is the stop number.
 * @param randint(1,5)
 */
int randint(int x, int y) {
  srand(time(NULL));
  return rand()%(y-x+1)+x;
}
/**Random Quit message selector
 * This is where it will set the quit message if there was a terminal quit or signal interrupt (ctrl+c)
 * @param siginit(12)
 */
string siginit(int sigstring){
  string message;
  switch(sigstring){
    case 1:
      message = "Read on an empty pipe (ENOTOBACCO)";
      break;
    case 2:
      message = "Invalid syntax (EBEFOREI)";
      break;
    case 3:
      message = "Core dumped (ECHERNOBYL)";
      break;
    case 4:
      message = "Program exited before being run (ECRAY)";
      break;
    case 5:
      message = "The daemon is dead (EDINGDONG)";
      break;
    case 6:
      message = "System needs tuning (EFLAT)";
      break;
    case 7:
      message = "Program written by inept Frat member (EGEEK)";
      break;
    case 8:
      message = "Here-a-bug, there-a-bug, .... (EIEIO)";
      break;
    case 9:
      message = "Missing period (EIUD)";
      break;
    case 10:
      message = "Your code could stand to be cleaned up (ELECTROLUX)";
      break;
    case 11:
      message = "Wrong fork (EMILYPOST)";
      break;
    case 12:
      message = "Silo overflow (END.ARMS.CONTROL)";
      break;
    case 13:
      message = "Mount failed (ENOHORSE)";
      break;
    case 14:
      message = "C program not derived from main(){printf(\"Hello, world\"); (ENONSEQUETOR)";
      break;
    case 15:
      message = "Extended tape gap (EWATERGATE)";
      break;
    case 16:
      message = "Aliens sighted (EWOK)";
      break;
    case 17:
      message = "Your code appears to have been stir-fried (EWOK)";
      break;
    case 18:
      message = "The feature you want has not been implemented yet (EWOULDBNICE)";
      break;
    case 19:
      message = "Nuclear event occurred (SIGNUKE)";
      break;
    case 20:
      message = "Someone pressed CTRL + C..";
      break;
  }
  return message;
}
/** Terminal Signal Handler
 * Come here for weird signals
 * @param sigact(number-only)
 */
void sigact(int sig)
{
  string sigstr;
  switch (sig){
    case 1:
      //this case can later be used to rehash the bot.
      sigstr = "Recieved rehash, no handler definition found. Quitting.";
      break;
    case 2:
      sigstr = siginit(randint(1,20));
      quitting = true;
      break;
    case 3:
      sigstr = siginit(randint(1,20));
      quitting = true;
      break;
    case 4:
      sigstr = "Illegal instruction (SIGILL)";
      quitting = true;
      break;
    case 6:
      sigstr = "Abort signal (SIGABRT)";
      quitting = true;
      break;
    case 9:
      sigstr = "KILL (SIGKILL)";
      quitting = true;
      break;
    case 10:
      sigstr = "User defined signal 1 (SIGUSR1)";
      quitting = true;
      break;
    case 13:
      sigstr = "Broken pipe (SIGPIPE)";
      quitting = true;
      break;
    case 14:
      sigstr = "Timer signal from alarm (SIGALRM)";
      quitting = true;
      break;
    case 15:
      sigstr = siginit(randint(1,20));
      quitting = true;
      break;
    default:
      stringstream signal1;
      string output;
      signal1 << sig;
      output = signal1.str();
      quitmsg = "Recieved weird signal from terminal. Sig Number: "+output;
      throw CoreException("Recieved weird signal from terminal. Signal Number: "+output);
  }
  quitmsg = "Recieved Signal: "+sigstr;
  cout << "\r\n\033[0m";
}

}
#endif