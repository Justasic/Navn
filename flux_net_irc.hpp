/* All code is licensed under GNU General Public License GPL v3 (http://www.gnu.org/licenses/gpl.html) */
#ifndef DERP_H
#define DERP_H
#include "module.h"
#include "defs.h"
#define isvalidnick(c) (isalnum(c) || ((c) >= '\x5B' && (c) <= '\x60') || ((c) >= '\x7B' && (c) <= '\x7D') || (c) == '-')
SendMessage *Send;
SocketIO *sock;
INIReader *config;
/**Runtime directory finder
 * This will get the bots runtime directory
 * @param getprogdir(const Flux::string dir)
 */
Flux::string getprogdir(const Flux::string &dir){
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
    return "";
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
DEPRECATED(static Flux::string cisolate(char begin, char end, Flux::string msg)){
  Flux::string to_find;
  size_t pos = msg.find(begin);
  pos += 1;
  for (unsigned i = pos; i < msg.length(); i++){
    if (msg[i] == end)
      break;
    else
      to_find = to_find+msg[i];
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
    IsoHost(const Flux::string &fullhost){
    nick = isolate(':','!',fullhost);
    raw = fullhost;
    host = isolate('@',' ',fullhost);
    user = isolate('!','@',fullhost);
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
 * \param cmd A command
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
    if (!pipe) return "";
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
void restart(Flux::string reason){
  char CurrentPath[FILENAME_MAX];
  GetCurrentDir(CurrentPath, sizeof(CurrentPath));
  if(reason.empty()){
    reason = "No Reason";
  }else{
    log(LOG_NORMAL, "Restarting: %s", reason.c_str());
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
 * \param boolean this boolean tells rehash if we are starting from start or not
 */
static void Rehash(bool onstart = false){
  if(!onstart)
    log(LOG_NORMAL, "Rehashing Config File: bot.conf");
  try{
    Flux::string conffile = binary_dir + "/bot.conf";
    if(config){
      INIReader *configtmp = config;
      config = new INIReader(conffile);
      delete configtmp;
    }
    config = new INIReader(conffile);
    if(config->ParseError() == -1)
      throw ConfigException("Cannot open file bot.conf");
    if (config->ParseError() != 0) {
      Flux::string error = "Error on line ";
      error += Flux::stringify(config->ParseError());
	throw ConfigException(error);
    }
    ReadConfig();
  }catch(const ConfigException &ex){
    if(onstart){
      Flux::string cfgerr = "Config: ";
      cfgerr += ex.GetReason();
      throw CoreException(cfgerr.c_str());
    }
    std::cout << "\r\nConfig Exception was caught: \033[22;31m" << ex.GetReason() << "\033[22;36m" << nl;
    log(LOG_NORMAL, "Config Exception Caught: %s", ex.GetReason());
    if(!onstart)
      Send->notice(owner_nick, "Config Exception Caught: %s", ex.GetReason());
  } 
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
    case 14: message = "C program not derived from main() { printf(\"Hello, world\"); } (ENONSEQUETOR)"; break;
    case 15: message = "Extended tape gap (EWATERGATE)"; break;
    case 16: message = "Aliens sighted (EWOK)"; break;
    case 17: message = "Your code appears to have been stir-fried (EWOK)"; break;
    case 18: message = "The feature you want has not been implemented yet (EWOULDBNICE)"; break;
    case 19: message = "Nuclear event occurred (SIGNUKE)"; break;
    case 20: message = "Someone pressed CTRL + C.."; break;
  }
  return message;
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
      sigstr = siginit(randint(1,20));
      quitmsg = "Recieved Signal: "+sigstr;
      if(Send)
        Send->command->quit(quitmsg);
      ModuleHandler::SanitizeRuntime();
      quitting = true;
      break;
    default:
      log(LOG_NORMAL, "Recieved weird signal from terminal. Sig Number: %i\n",sig);
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
  Flux::string dir = argv[0];
  Flux::string::size_type n = dir.rfind('/');
  dir = "." + dir.substr(n);
  //gets the command line paramitors if any.
  int Terminal = isatty(0) && isatty(1) && isatty(2);
  if (argc < 1 || argv[1] == NULL){
  }else{
    Flux::string arg = argv[1];
    for(int Arg=0; Arg < argc; ++Arg){
       if(arg == "--developer" || arg == "--dev" || arg == "-d")
       {
         dev = nofork = true;
	 log(LOG_DEBUG, "%s is started in Developer mode. (%s)", nick.c_str(), arg.c_str());
       }
       else if (arg == "--nofork" || arg == "-f"){
         nofork = true;
         log(LOG_DEBUG, "%s is started With No Forking enabled. (%s)", nick.c_str(), arg.c_str());
       }
       else if (arg == "--help" || arg == "-h"){
	  log(LOG_TERMINAL, "\033[22;37mNavn Internet Relay Chat Bot v%s\n", VERSION);
	  log(LOG_TERMINAL, "Usage: %s [options]\n", dir.c_str());
	  log(LOG_TERMINAL, "-h, --help\n");
	  log(LOG_TERMINAL, "-d, --developer\n");
	  log(LOG_TERMINAL, "-f, --nofork\n");
	  log(LOG_TERMINAL, "This bot does have Epic Powers.\n");
	  exit(0);
       }
       else if (arg == "--version" || arg == "-v"){
         log(LOG_TERMINAL, "\033[22;37mNavn IRC C++ Bot Version %s",VERSION_LONG.c_str());
         log(LOG_TERMINAL, "This bot was programmed from scratch by Justasic and Lordofsraam.");
         log(LOG_TERMINAL, "\n");
         log(LOG_TERMINAL, "IRC: IRC.Flux-Net.net #Computers");
         log(LOG_TERMINAL, "WWW: http://www.Flux-Net.net");
         log(LOG_TERMINAL, "Email: Staff@Flux-Net.net");
         log(LOG_TERMINAL, "Git: git://gitorious.org:navn/navn.git");
         log(LOG_TERMINAL, "\n");
         log(LOG_TERMINAL, "This bot does have Epic Powers.");
         log(LOG_TERMINAL, "Type %s --help for help on how to use navn, or read the readme.", dir.c_str());
         exit(0);
       }
       else if(arg == "--protocoldebug" || "-p"){
	 protocoldebug = true;
	 nofork = true;
	 log(LOG_RAWIO, "%s is started in Protocol Debug mode. (%s)", nick.c_str(), arg.c_str());
       }
       else
       {
	log(LOG_TERMINAL, "Unknown option %s", arg.c_str());
	exit(0);
       }
  }
  }
   WritePID();
   ModuleHandler::SanitizeRuntime();
   log(LOG_NORMAL, "%s Started. PID: %d", nick.c_str(), getpid());
   if (!nofork){
	int i;
	if((i = fork()) < 0)
		throw CoreException("Unable to fork");
	else if (i != 0){
		log(LOG_TERMINAL, "Navn IRC Bot v%s Started.", VERSION);
		log(LOG_TERMINAL, "Forking to background. PID: %i\033[22;37m", i);
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
/** 
 * \fn Flux::string xmlToString(Flux::string fileName)
 * \brief takes an xml file and converts it to a Flux::string
 * \param source The filename for conversion
 */

Flux::string xmlToString(const Flux::string &fileName){
  std::string buf, line;
  std::ifstream in(fileName.c_str());
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
Flux::string findInXML(const Flux::string &node, const Flux::string &info, const Flux::string &fileString){
  Flux::string findee = "<"+node;
  size_t p = fileString.find(findee);
  if(p > fileString.length()) /* Sanity check so we don't SIGABRT */
    return "";
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
namespace ThreadHandler
{
  class Thread
  {
  public:
    virtual void ToRun(){};
  };
  Thread * porker;
  void *nullpointer;
  static inline pthread_attr_t *GetAttr()
  {
   static pthread_attr_t attr;
   if(pthread_attr_init(&attr))
     throw CoreException("Error calling pthread_attr_init for Threads");
   if(pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE))
     throw CoreException("Unable to make threads joinable");
   return &attr;
  }
  void *bnyeh(void*)
  {
    porker->ToRun();
    return nullpointer;
  }
  
  void RunThread(Thread piggy)
  {    
    porker = &piggy;
    pthread_t t1;
    pthread_create(&t1,NULL,&bnyeh,NULL);
  }
}

void ProcessModules(CommandSource &source, std::vector<Flux::string> &params){
  for(unsigned i = 0; i < moduleList.size(); i++){
    if (moduleList[i]->activated == true){
      moduleList[i]->run(source, params);
    }
  } 
}
/***************************************************************************/
#define FOREACH_MOD(y, x) \
if(true) \
{ \
    std::vector<module*>::iterator safei; \
    for (std::vector<module*>::iterator _i = ModuleHandler::EventHandlers[y].begin(); _i != ModuleHandler::EventHandlers[y].end(); ) \
    { \
       safei = _i; \
       ++safei; \
       try \
       { \
          (*_i)->x ; \
       } \
       catch (const ModuleException &modexcept) \
       { \
          log(LOG_NORMAL, "Exception caught: %s", modexcept.GetReason()); \
       } \
        _i = safei; \
    } \
} \
else \
      static_cast<void>(0)

//const Flux::string &modname,
#define MODULE_HOOK(x) \
extern "C" module *ModInit(bool activated) \
        { \
                return new x(activated); \
        } \
        extern "C" void Moduninit(x *m) \
        { \
                delete m; \
        }
        
#endif
