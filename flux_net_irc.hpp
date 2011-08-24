#ifndef DERP_H
#define DERP_H
#include "user.h"
#include "defs.h"
#define isvalidnick(c) (isalnum(c) || ((c) >= '\x5B' && (c) <= '\x60') || ((c) >= '\x7B' && (c) <= '\x7D') || (c) == '-')
SendMessage *Send;
SocketIO *sock;
/**Runtime directory finder
 * This will get the bots runtime directory
 * @param getprogdir(const Flux::string dir)
 */
Flux::string getprogdir(const Flux::string dir){
  char buffer[FILENAME_MAX];
  if (GetCurrentDir(buffer, sizeof(buffer))) {
    Flux::string remainder = dir;
    bot_bin = remainder;
    Flux::string::size_type n = bot_bin.rfind("/");
    Flux::string fullpath;
    if (bot_bin[0] == '/')
      fullpath = bot_bin.substr(0, n);
    else
      fullpath = Flux::string(buffer) + "/" + bot_bin.substr(0, n);
    bot_bin = bot_bin.substr(n + 1, remainder.length());
    return fullpath;
  }
  return "/";
}
class irc_string:Flux::string{
  /** \class irc_string
 * NOTE this MUST be included in the main file.
 * This class wraps around a Flux::string (usually revieved from the irc server)
 * and attempts to decode all the info recieved from the server
 * so as to make it easier to read different kinds of information
 * (ei. who said something, their host, username, etc.)
 * \param toks Private vector that will contain the words in \a message.
 * \param raw_Flux::string Will hold the exact Flux::string that was recieved from the server.
 * \param usernick Will hold the usernick of the person who sent the message.
 * \param host Will hold the host of the person who sent the message.
 * \param user Will hold the user of the person who sent the message.
 * \param channel Will hold the channel of where the message was said.
 * \param message Will hold the message (stripped of irc protocol stuff)
 */
  private:
    std::vector<Flux::string> toks;
  public:
    Flux::string raw_string;
    Flux::string usernick; 
    Flux::string host; 
    Flux::string user; 
    Flux::string channel; 
    Flux::string message;
/**
  *\fn irc_string(Flux::string reply)
  * \deprecated
  *Constructor for \a irc_string class
  * This is where the magic happens. The constructor takes the Flux::string
  * and breaks it down into its component parts to find the 
  * \a usernick \a host \a user \a channel and \a message
  */
 DEPRECATED(irc_string(Flux::string reply)){
  raw_string = reply;
  usernick = isolate(':','!',reply);
  host = isolate('@',' ',reply);
  user = isolate('!','@',reply);
  channel = '#'+isolate('#',' ',reply);
  Flux::string space = " ";
  size_t pos = reply.find(" :");
  pos += 2;
  for (unsigned i = pos; i < reply.size(); i++){
    if (reply.at(i) == ' '){
      message.append(space);
    }else{message = message+reply.at(i);}
  }
  if(message.size()>2){
    message.resize(message.size()-2);
  }
  
  Flux::string fmessage = message;
  char * cmessage = (char *)fmessage.c_str();
  char * pch;
  pch = strtok(cmessage," ");
  while (pch != NULL)
  {
    toks.push_back(pch);
    pch = strtok(NULL, " ");
  }
}
/**
  * \fn Flux::string params(int i)
  * \brief Returns individual words from the message of a reply
  * \deprecated
  * Because \a toks is private, this is its "get" function.
  * We made this so someone writing a module doesn't try to go out 
  * of bounds while accessing an array.
  * \param i An integer value.
  * \return A Flux::string with the single specified word.
  */
DEPRECATED(Flux::string params(unsigned i)){
  if (i >= toks.size()){
    return " ";
  }else{return toks[i];}
}
/**
  * \overload Flux::string params(int b, int e)
  * \brief Overload of params. Returns a range of words.
  * \deprecated
  * We overloaded \a params() so that you could get a range of words from the message
  *  as requeseted by Justasic.
  * \param b An integer value describing the place of the first word wanted.
  * \param e An integer value describing the place of the last word wanted.
  * \return A Flux::string withing the range of words specified by \a b and \a e
  */
DEPRECATED(Flux::string params(unsigned b, unsigned e)){
  Flux::string buf = "";
  if (b >= toks.size()){
    b = toks.size();
  }
  if (e >= toks.size()){
    e = toks.size() - 1;
  }
  for (unsigned i = b; i <= (e); i++){
    buf += toks[i];
    buf.append(" ");
  }
  return buf;
}
/**
  * \fn static Flux::string isolate(char begin, char end, Flux::string msg)
  * \brief Isolates a Flux::string between two characters
  * Finds the first character, then begins to add every consecutive character from there to a Flux::string
  *  until it reaches the end character.
  * \param begin The character saying where the cut should begin.
  * \param end The character saying where the cut should end.
  * \param msg The Flux::string you are wanting to isolate from.
  */
static Flux::string isolate(char begin, char end, Flux::string msg){
  Flux::string to_find;
  size_t pos = msg.find(begin);
  pos += 1;
  for (unsigned i = pos; i < msg.length(); i++){
    if (msg.at(i) == end){
      break;
    }else{to_find = to_find+msg.at(i);}
  }
  return to_find;
}
/**
  * \fn bool said(Flux::string findee)
  * \brief Check if something was said.
  * \deprecated
  * \param findee The Flux::string you want to check if was said.
  * \return True if \a findee was found, false otherwise.
  */
DEPRECATED(bool said(Flux::string findee)){
  unsigned i = raw_string.find(findee);
  if (i != Flux::string::npos){
    return true;
  }else{return false;}
}
/**
  * \overload static bool said(Flux::string source, Flux::string findee)
  * \brief Static overload of said() function.
  * \param source A Flux::string that is to be searched through.
  * \param findee The Flux::string that is to be found.
  * We overloaded the said function and made it static because we thought it would be 
  *  very useful to have outside of an \a irc_string object.
  * \return True if \a findee was found, false otherwise.
  */
static bool said(Flux::string source, Flux::string findee){
  unsigned i = source.find(findee);
  if (i != Flux::string::npos){
    return true;
  }else{return false;}
}
};


/**
 * \class IsoHost
 * \brief Wrapper for an irc host
 * This was written by Justasic to break up the parts of a messages host for easier use.
 */
class IsoHost:Flux::string{
public:
    Flux::string raw;
    Flux::string nick;
    Flux::string host;
    Flux::string user;
   /**
     * \fn IsoHost(Flux::string fullhost)
     * \param fullhost A Flux::string containing the full host of an irc message
     */
    IsoHost(Flux::string fullhost){
    nick = irc_string::isolate(':','!',fullhost);
    raw = fullhost;
    host = irc_string::isolate('@',' ',fullhost);
    user = irc_string::isolate('!','@',fullhost);
    }

};
/** 
 * \fn Flux::string removeCommand(Flux::string command, Flux::string s)
 * \brief Removes a command from a Flux::string.
 * \param command String to be taken out.
 * \param s Original Flux::string.
 * This takes out \a command from \a s and returns \a s without \a command It's very useful when you want 
 * to use the rest of a Flux::string as an argument for a command.
 * \return A Flux::string \a s without \a command.
 */
Flux::string removeCommand(Flux::string command, Flux::string s){
  size_t pos = s.find(command);
  return s.substr(pos+(command.size())+1);
}

/** 
 * \fn Flux::string urlify(Flux::string raw_searchstring)
 * \brief Replaces special chars in a Flux::string with url compliant codes.
 * \param raw_searchstring
 * Goes through each character in a Flux::string and if it finds a special character, 
 * it replaces it with what would be in a url for that character.
 * \return A Flux::string without any special characters other than %
 */
Flux::string urlify(Flux::string raw_searchstring){
  Flux::string searchstring;
  for (unsigned i=0; i < raw_searchstring.length(); i++){
     switch(raw_searchstring.at(i)){
      case ' ': searchstring += "%20"; break;
      case '+': searchstring += "%2B"; break;
      case '$': searchstring += "%24"; break;
      case '&': searchstring += "%26"; break;
      case ',': searchstring += "%2C"; break;
      case '/': searchstring += "%2F"; break;
      case ':': searchstring += "%3A"; break;
      case ';': searchstring += "%3B"; break;
      case '=': searchstring += "%3D"; break;
      case '?': searchstring += "%3F"; break;
      case '@': searchstring += "%40"; break;
      case '#': searchstring += "%23"; break;
      case '>': searchstring += "%3E"; break;
      case '<': searchstring += "%3C"; break;
      case '%': searchstring += "%25"; break;
      case '{': searchstring += "%7B"; break;
      case '}': searchstring += "%7D"; break;
      case '|': searchstring += "%7C"; break;
      case '\\':searchstring += "%5C"; break;
      case '^': searchstring += "%5E"; break;
      case '~': searchstring += "%7E"; break;
      case '[': searchstring += "%5B"; break;
      case ']': searchstring += "%5D"; break;
      case '`': searchstring += "%60"; break;
      case '*': searchstring += "%2A"; break;
      case '(': searchstring += "%28"; break;
      case ')': searchstring += "%29"; break;
      case '"': searchstring += "%22"; break;
      case '\'': searchstring += "%27"; break;
      case '.': searchstring += "%2E"; break;
      default:
	searchstring = searchstring+raw_searchstring.at(i);
    }
  }
  return searchstring;
}

/** 
 * \fn Flux::string execute(const char *cmd)
 * \brief Sends a command to the OS
 * \param *cmd A command
 * \return A Flux::string containing the response from the OS.
 */
Flux::string execute(const char *cmd) {
  /* 
 * Roses are red,
 * Violets are blue,
 * I read StackOverflow
 * And so do you!
*/
    #ifdef _WIN32
    FILE* pipe = _popen
    #else
    FILE* pipe = popen(cmd, "r");
    #endif
    if (!pipe) return "ERROR";
    char buffer[128];
    Flux::string result = "";
    while(!feof(pipe)) {
        if(fgets(buffer, 128, pipe) != NULL)
                result += buffer;
    }
    #ifdef _WIN32
    _pclose(pipe);
    #else
    pclose(pipe);
    #endif
    return result;
}

/** 
 * \fn Flux::string make_two_digits(int x)
 * \brief Makes single digit int into a double digit Flux::string.
 * This was really just for the \a world_clock module but it's handy to have around.
 * \param x A single digit integer.
 * \return A Flux::string containing integer as a double digit.
 */
Flux::string make_two_digits(int x){
  if (x < 10){
    stringstream dd_ss;
    dd_ss << "0" << x;
    Flux::string dd_time = dd_ss.str();
    return dd_time;
  }else{
    stringstream sd_ss;
    sd_ss << x;
    Flux::string sd_time = sd_ss.str();
    return sd_time;
  }
}
/** \fn Flux::string strip(const Flux::string &buf)
 * \brief Strips \r and \n from a Flux::string
 * Takes out the '\r' and '\n' from a Flux::string. Mostly used in messing with irc replies.
 * \param &buf String to be stripped
 * \return \a &buf without the '\r' and '\n'.
 */
Flux::string strip(const Flux::string &buf){
	Flux::string newbuf = buf;
	char c = newbuf[newbuf.size() - 1];
	while (c == '\n' || c == '\r'){
		newbuf.erase(newbuf.end() - 1);
		c = newbuf[newbuf.size() - 1];
	}
	return newbuf;
}
/**
 *\fn  Flux::string os_time()
 *\brief Get the operating system's time
 * This is just a simple function that gets the time.
 * \return A Flux::string of the current system time.
 */
Flux::string os_time(){
  time_t rawtime;
  time(&rawtime);
  return ctime(&rawtime);
}
/**
 * \fn bool IsValadChannel(const Flux::string nerp)
 * This function returns if the channel is valid or not.
 * \param nerp Channel sring to be tested.
 * \return True if the Flux::string is a valid channel, false otherwise.
 */
bool IsValidChannel(const Flux::string &chan){
 if (chan[0] != '#')
    return false;
 return true;
}

/** 
 * \fn Flux::string make_pass()
 * \brief Makes a random password
 * This generates a 5 number random password for the bots
 * quit and other password protected commands.
 * \return A Flux::string containing the 5 digit password.
 */
Flux::string make_pass(){
  int p1,p2,p3,p4,p5;
  srand(time(NULL));
  p1 = rand()%10; 
  p2 = rand()%10;
  p3 = rand()%10;
  p4 = rand()%10;
  p5 = rand()%10;
  stringstream pass_ss;
  pass_ss << p1 << p2 << p3 << p4 << p5;
  Flux::string pass_str = pass_ss.str();
  return pass_str;
}
const Flux::string password = make_pass();

/** 
 * \fn static void remove_pidfile()
 * \brief Removes the PID file on exit
 */
static void remove_pidfile()
{
 remove(pid_file.c_str());
}
/** 
 * \fn static void restart(Flux::string reason)
 * \brief Restart the bot process
 * \param reason The reason for the restart
 */
static void restart(Flux::string reason){
  char CurrentPath[FILENAME_MAX];
  GetCurrentDir(CurrentPath, sizeof(CurrentPath));
  if(reason.empty()){
    reason = "No Reason";
  }else{
    log("Restarting: %s", reason.c_str());
    Send->command->quit("Restarting: %s", reason.c_str());
    chdir(CurrentPath);
    int execvpret = execvp(my_av[0], my_av);
    if(execvpret > 0){
      throw CoreException("Restart Failed, Exiting");
    }
    remove_pidfile();
    exit(1);
  }
}

/** 
 * \fn static void Rehash(Socket &sock)
 * \brief Reload the bot config file
 * \param sock The Socket class
 */
static void Rehash(bool onstart = false){
  if(!onstart)
    cout << "Rehashing config file." << nl;
  try{
    binary_dir += "/bot.conf";
    INIReader config(binary_dir.c_str());
    if (config.ParseError() < 0) {
      Flux::string error = "Cannot load bot.conf: ";
      error += stringify(config.ParseError());
	throw ConfigException(error);
    }
    ReadConfig(config);
  }catch(ConfigException &ex){
      cout << "\r\nConfig Exception was caught: \033[22;31m" << ex.GetReason() << "\033[22;36m" << nl;
      log("Config Exception Caught: %s", ex.GetReason());
      if(!onstart)
        Send->notice(owner_nick, "Config Exception Caught: %s", ex.GetReason());
  } 
}
/** Terminal Signal Handler
 * Come here for weird signals
 * @param sigact(integer)
 */
void sigact(int sig)
{
  Flux::string sigstr;
  switch (sig){
    case SIGHUP:
      signal(sig, SIG_IGN);
      Rehash(false);
      break;
    case SIGINT:
    case SIGKILL:
    case SIGTERM:
      signal(sig, SIG_IGN);
      signal(SIGHUP, SIG_IGN);
      sigstr = "Someone used Ctrl + C";
      quitmsg = "Recieved Signal: "+sigstr;
      Send->command->quit(quitmsg);
      cout << "\r\n\033[0m";
      quitting = true;
      break;
    default:
      printf("Recieved weird signal from terminal. Sig Number: %i\n",sig);
  }
}
/** 
 * \fn static void WritePID()
 * \brief Write the bots PID file
 */
static void WritePID(){
  //logging to a text file and making the PID file.
  FILE *pidfile;
  pidfile = fopen(pid_file.c_str(), "w");
  if(pidfile){
#ifdef _WIN32
    fprintf(pidfile, "%d\n", static_cast<int>(GetCurrentProcessId()));
#else
    fprintf(pidfile, "%d\n", static_cast<int>(getpid()));
#endif
    fclose(pidfile);
    atexit(remove_pidfile);
  }
  else
    throw CoreException("Can not write to PID file "+pid_file);
}
/**This is the startup sequence that starts at the top to the try loop
 * @param startup(int, char)
 */
void startup(int argc, char** argv) {
  binary_dir = getprogdir(argv[0]);
  if(binary_dir[binary_dir.length() - 1] == '.')
    binary_dir = binary_dir.substr(0, binary_dir.length() - 2);
  Rehash(true);
  signal(SIGTERM, sigact);
  signal(SIGINT, sigact);
  signal(SIGHUP, sigact);
  //gets the command line paramitors if any.
  int Terminal = isatty(0) && isatty(1) && isatty(2);
  if (argc < 1 || argv[1] == NULL){
  }else{
    Flux::string arg = argv[1];
    for(int Arg=0; Arg < argc; ++Arg){
       if(arg == "--developer" || arg == "--dev" || arg == "-d")
       {
         dev = true;
	 nofork = true;
	 log("Navn is started in Developer mode. (%s)", arg.c_str());
       }
       else if (arg == "--nofork" || arg == "-f"){
         nofork = true;
         log("Navn is started With No Forking enabled. (%s)", arg.c_str());
       }
       else if (arg == "--help" || arg == "-h"){
	  printf("\033[22;37mNavn Internet Relay Chat Bot v%s\n", VERSION.c_str());
	  Flux::string dir = argv[0];
	  Flux::string::size_type n = dir.rfind('/');
	  dir = "." + dir.substr(n);
	  printf("Usage: %s [options]\n", dir.c_str());
	  printf("-h, --help\n");
	  printf("-d, --developer\n");
	  printf("-f, --nofork\n");
	  printf("This bot does have Epic Powers.\n");
	  exit(0);
       }
       else if (arg == "--version" || arg == "-v"){
         cout << "\033[22;37mNavn IRC C++ Bot Version " << VERSION << nl;
         cout << "This bot was programmed from scratch by Justasic and Lordofsraam." << nl;
         cout << nl;
         cout << "IRC: IRC.Flux-Net.net #Computers" << nl;
         cout << "WWW: http://www.Flux-Net.net" << nl;
         cout << "Email: Staff@Flux-Net.net" << nl;
         cout << "Git: git://gitorious.org:navn/navn.git" << nl;
         cout << nl;
         cout << "This bot does have Epic Powers." << nl;
         cout << "Type ./navn --help for help on how to use navn, or read the readme." << nl;
         exit(0);
       }
       else if(arg == "--protocoldebug" || "-p"){
	 protocoldebug = true;
	 nofork = true;
	 log("Navn is started in Protocol Debug mode. (%s)", arg.c_str());
       }
       else{
	printf("Unknown option %s", arg.c_str());
	exit(0);
       }
  }
  }
   WritePID();
   log("Navn Started. PID: %d", getpid());
   if (!nofork){
	int i;
	if((i = fork()) < 0)
		throw CoreException("Unable to fork");
	else if (i != 0){
		cout << "Navn IRC Bot v" << VERSION_SHORT << " Started." << nl;
		cout << "Forking to background. PID: "<< i << "\033[22;37m" << nl;
		exit(0);
	}
	if(Terminal){
		close(0);
		close(1);
		close(2);
	}
	if(setpgid(0, 0) < 0)
		throw CoreException("Unable to setpgid()");
  }
}
/**Random Number Generator
 * This will generate a random number x is start number, y is the stop number.
 * @param randint(int x, int y)
 */
int randint(int x, int y){
  srand(time(NULL));
  return rand()%(y-x+1)+x;
}
/**Random Quit message selector
 * This is where it will set the quit message if there was a terminal quit or signal interrupt (ctrl+c)
 * @param siginit(integer)
 */
Flux::string siginit(int sigstring){
  Flux::string message;
  switch(sigstring){
    case 1: message = "Read on an empty pipe (ENOTOBACCO)"; break;
    case 2: message = "Invalid syntax (EBEFOREI)"; break;
    case 3: message = "Core dumped (ECHERNOBYL)"; break;
    case 4: message = "Program exited before being run (ECRAY)"; break;
    case 5: message = "The daemon is dead (EDINGDONG)"; break;
    case 6: message = "System needs tuning (EFLAT)"; break;
    case 7: message = "Program written by inept Frat member (EGEEK)"; break;
    case 8: message = "Here-a-bug, there-a-bug, .... (EIEIO)"; break;
    case 9: message = "Missing period (EIUD)"; break;
    case 10: message = "Your code could stand to be cleaned up (ELECTROLUX)"; break;
    case 11: message = "Wrong fork (EMILYPOST)"; break;
    case 12: message = "Silo overflow (END.ARMS.CONTROL)"; break;
    case 13: message = "Mount failed (ENOHORSE)"; break;
    case 14: message = "C program not derived from main(){printf(\"Hello, world\"); (ENONSEQUETOR)"; break;
    case 15: message = "Extended tape gap (EWATERGATE)"; break;
    case 16: message = "Aliens sighted (EWOK)"; break;
    case 17: message = "Your code appears to have been stir-fried (EWOK)"; break;
    case 18: message = "The feature you want has not been implemented yet (EWOULDBNICE)"; break;
    case 19: message = "Nuclear event occurred (SIGNUKE)"; break;
    case 20: message = "Someone pressed CTRL + C.."; break;
  }
  return message;
}

/** 
 * \fn Flux::string xmlToString(Flux::string fileName)
 * \brief takes an xml file and converts it to a Flux::string
 * \param source The filename for conversion
 */

Flux::string xmlToString(Flux::string fileName){
  std::string buf, line;
  ifstream in(fileName.c_str());
  while(std::getline(in,line)){
    buf += line;
  }
  return buf.c_str();
}
/** 
 * \fn Flux::string findInXML(Flux::string node, Flux::string info, Flux::string fileString)
 * \brief xml Flux::string parser
 * \param node An XML node
 * \param info The info from that node
 * \param fileString Flux::string to parse
 */
Flux::string findInXML(Flux::string node, Flux::string info, Flux::string fileString){
 
  Flux::string findee = "<"+node;
  size_t p = fileString.find(findee);
  if(p > fileString.length()) /* Sanity check so we don't SIGABRT */
    return "Error: length too large";
  bool foundInfo = false;
  int a = 0;
  while(!foundInfo){
    Flux::string infoFound = "";
    for(unsigned m = 0; m < info.length(); m++){
      infoFound += fileString.at(p+a+m);
    }
    if (infoFound == info){
      foundInfo = true;
    }else{a++;}
  }
  p += a+info.length()+2;
  Flux::string output;
  int i = 0;
  while(!(fileString.at(p+i) == '"')){
    output += fileString.at(p+i);
    i++;
  }
  return output;
}
/*******************************************************************/
enum Implementation{
  I_BEGIN,
	I_OnPrivmsg,
  I_END
};
enum ModuleReturn{
  MOD_RUN,
  MOD_STOP
};
enum ModulePriority{
  PRIORITY_FIRST,
  PRIORITY_DONTCARE,
  PRIORITY_LAST
};
//This code sucks, you know it and I know it. 
//Move on and call me an idiot later.

class module{ 
  Flux::string desc;
protected:
  void SetDesc(const Flux::string&);
public:
  Flux::string name;
  const Flux::string GetDesc() const;
  bool activated;
  ModulePriority priority;
  Flux::string author;
  module (Flux::string , bool, ModulePriority);
  int AddCommand(Command *c);
  int DelCommand(Command *c);
  
  virtual ModuleReturn run(CommandSource&, std::vector<Flux::string>&) =0;
  virtual void OnPrivmsg(const Flux::string &nick, const std::vector<Flux::string> &params) { }
  
};
void module::SetDesc(const Flux::string &desc){
  this->desc = desc;
}
const Flux::string module::GetDesc() const{
 return this->desc; 
}
std::vector<module*> moduleList;

module::module(Flux::string n, bool a, ModulePriority p){
  name = n;
  activated = a;
  priority = p;
  moduleList.push_back(this);
}
module *FindModule(const Flux::string &name){
 for(std::vector<module*>::const_iterator it = moduleList.begin(), it_end = moduleList.end(); it != it_end; ++it){
  module *m = *it;
  if(m->name == name)
    return m;
 }
 return NULL;
}
/***********************************************************************/
/* commands stuff */
CommandMap Commandsmap;
int module::AddCommand(Command *c){
 if(!c)
   return 1;
 std::pair<CommandMap::iterator, bool> it = Commandsmap.insert(std::make_pair(c->name, c));
 if(it.second != true){
   log("Command %s already loaded!", c->name.c_str());
   return 2;
 }
 c->mod = this;
 return 0;
}
int module::DelCommand(Command *c){
  if(!c)
    return 1;
  if(!Commandsmap.erase(c->name))
    return 2;
  c->mod = NULL;
  return 0;
}
std::vector<module *> EventHandlers[I_END];
void ProcessModules(CommandSource &source, std::vector<Flux::string> &params){
  for(unsigned i = 0; i < moduleList.size(); i++){
    if (moduleList[i]->activated == true){
      moduleList[i]->run(source, params);
    }
  } 
}
Command *FindCommand(const Flux::string &name){
 if(name.empty())
   return NULL;
 CommandMap::iterator it = Commandsmap.find(name);
 if(it != Commandsmap.end())
   return it->second;
 return NULL;
}
/***************************************************************************/
#define FOREACH_MOD(y, x) \
if(true) \
{ \
    std::vector<module*>::iterator safei; \
    for (std::vector<module*>::iterator _i = EventHandlers[y].begin(); _i != EventHandlers[y].end(); ) \
    { \
       safei = _i; \
       ++safei; \
       try \
       { \
          (*_i)->x ; \
       } \
       catch (const ModuleException &modexcept) \
       { \
          log("Exception caught: %s", modexcept.GetReason()); \
       } \
        _i = safei; \
    } \
} \
else \
      static_cast<void>(0)
/**
 * \brief main processing loop
 * 
 * 
 * 
 */
void process(const Flux::string &buffer){
  Flux::string buf = buffer;
  buf = buf.replace_all_cs("  ", " ");
  if(buf.empty())
    return;
  Flux::string source;
  if(buf[0] == ':'){
   size_t space = buf.find_first_of(" ");
   if(space == Flux::string::npos)
     return;
   source = buf.substr(1, space - 1);
   buf = buf.substr(space + 1);
   if(source.empty() || buf.empty())
     return;
  }
  sepstream bufferseparator(buf, ' ');
  Flux::string bufferseparator_token;
  Flux::string command = buf;
  if(bufferseparator.GetToken(bufferseparator_token))
    command = bufferseparator_token;
  std::vector<Flux::string> params;
  
  while(bufferseparator.GetToken(bufferseparator_token)){
    if(bufferseparator_token[0] == ':'){
      if(!bufferseparator.StreamEnd())
	params.push_back(bufferseparator_token.substr(1)+" "+bufferseparator.GetRemaining());
      else
	params.push_back(bufferseparator_token.substr(1));
      break;
    }
    else
      params.push_back(bufferseparator_token);
  }
  if(protocoldebug){
   printf("--> %s\n", Flux::Sanitize(buffer).c_str());
   printf("Source: %s\n", source.empty() ? "No Source" : source.c_str());
   printf("%s: %s\n", command.is_number_only() ? "Numeric" : "Command", command.c_str());
   if(params.empty())
     printf("No Params\n");
   else
     for(unsigned i =0; i < params.size(); ++i)
       printf("Params %i: %s\n", i, params[i].c_str());
     
  }
  /**************************************/
  const Flux::string &receiver = params.size() > 0 ? params[0] : "";
  Flux::string message = params.size() > 1? params[1] : "";
  
  if(command == "PRIVMSG"){
    if(protocoldebug){
    }else
      if(!receiver.empty() && !params[1].empty())
       printf("<%s-%s> %s\n", irc_string::isolate(':','!',source).c_str(), receiver.c_str(), params[1].c_str());
  }else
    if(!protocoldebug) printf("--> %s\n", Flux::Sanitize(buffer).c_str());
  /**************************************/
  /* make local variables instead of global ones */
  Flux::string nickname = irc_string::isolate(':','!',source),
  uident = irc_string::isolate('!','@',source),
  uhost = irc_string::isolate('@',' ',source);
  
  User *u = finduser(nickname);
  if(!u){
    if(nickname.empty() || uident.empty() || uhost.empty()){ }else
      u = new User(nickname, uident, uhost);
  }
  if(command == "QUIT"){
    User *u = finduser(nickname);
   if(!u){}else{
     delete u;
   }
  }
  Channel *c;
  if(command == "PART"){
    if(receiver.empty()){
     printf("wtf?\n");
     return;
    }
    if(IsValidChannel(receiver)){
     c = findchannel(receiver);
     if(c)
       delete c;
    }
  }
  if(command == "JOIN"){
    if(receiver.empty()){
     printf("wtf?\n");
     return;
    }
    if(IsValidChannel(receiver)){
     c = findchannel(receiver);
     if(!c)
       c = new Channel(receiver);
    }
  }
  CommandSource Source;
  Source.u = u; //User class
  Source.c = c; //Channel class
  Source.command = command;
  Source.message = message;
  Source.params = params;
  Source.raw = buffer;
  std::vector<Flux::string> params2 = StringVector(message, ' ');
  if(source.empty() || message.empty() || params2.empty())
    return;
  ProcessModules(Source, params2);
}
//const Flux::string &modname,
#define MODULE_HOOK(x) \
extern "C" module *ModInit(const bool activated) \
        { \
                return new x(activated); \
        } \
        extern "C" void Modunini(x *m) \
        { \
                delete m; \
        }
        
#endif
