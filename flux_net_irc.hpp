#ifndef DERP_H
#define DERP_H
#include "includes.h"
#include "defs.h"
#define isvalidnick(c) (isalnum(c) || ((c) >= '\x5B' && (c) <= '\x60') || ((c) >= '\x7B' && (c) <= '\x7D') || (c) == '-')

SendMessage *Send;

  /** 
   * \namespace flux_net_irc
   * \brief This is where we keep all our algorithms
   * 
   * We made this namespace so we could group all the common functions
   * and algorithms we use really often in one place. These will 
   * hopefully help anyone else that tries to extend navn.
 */
  using namespace std;

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
 * \fn Flux::string makeSearchString(Flux::string raw_searchstring)
 * \brief Replaces special chars in a Flux::string with url compliant codes.
 * \param raw_searchstring
 * Goes through each character in a Flux::string and if it finds a special character, 
 * it replaces it with what would be in a url for that character.
 * \return A Flux::string without any special characters other than %
 */
Flux::string makeSearchString(Flux::string raw_searchstring){
  Flux::string searchstring;
  for (unsigned i=0; i < raw_searchstring.length(); i++){
     switch(raw_searchstring.at(i)){
      case ' ':
	searchstring = searchstring+"%20";
	break;
      case '+':
	searchstring = searchstring+"%2B";
	break;
      case '$':
	searchstring = searchstring+"%24";
	break;
      case '&':
	searchstring = searchstring+"%26";
	break;
      case ',':
	searchstring = searchstring+"%2C";
	break;
      case '/':
	searchstring = searchstring+"%2F";
	break;
      case ':':
	searchstring = searchstring+"%3A";
	break;
      case ';':
	searchstring = searchstring+"%3B";
	break;
      case '=':
	searchstring = searchstring+"%3D";
	break;
      case '?':
	searchstring = searchstring+"%3F";
	break;
      case '@':
	searchstring = searchstring+"%40";
	break;
      case '#':
	searchstring = searchstring+"%23";
	break;
      case '>':
	searchstring = searchstring+"%3E";
	break;
      case '<':
	searchstring = searchstring+"%3C";
	break;
      case '%':
	searchstring = searchstring+"%25";
	break;
      case '{':
	searchstring = searchstring+"%7B";
	break;
      case '}':
	searchstring = searchstring+"%7D";
	break;
      case '|':
	searchstring = searchstring+"%7C";
	break;
      case '\\':
	searchstring = searchstring+"%5C";
	break;
      case '^':
	searchstring = searchstring+"%5E";
	break;
      case '~':
	searchstring = searchstring+"%7E";
	break;
      case '[':
	searchstring = searchstring+"%5B";
	break;
      case ']':
	searchstring = searchstring+"%5D";
	break;
      case '`':
	searchstring = searchstring+"%60";
	break;
      case '*':
	searchstring = searchstring+"%2A";
	break;
      case '(':
	searchstring = searchstring+"%28";
	break;
      case ')':
	searchstring = searchstring+"%29";
	break;
      case '"':
	searchstring = searchstring+"%22";
	break;
      case '\'':
	searchstring = searchstring+"%27";
	break;
      case '.':
	searchstring = searchstring+"%2E";
	break;
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
  time ( &rawtime );
  return ctime(&rawtime);
}

/** 
 * \fn void log(const char *fmt, ...)
 * This is what logs everything that goes on with the bot
 * \param *fmt
 */
void log(const char *fmt, ...){
  fstream log;
  try{
  log.open(logfile.c_str(), fstream::in | fstream::out | fstream::app);
  if(!log.is_open())
     throw LogException("Failed to open log file.");
  }catch (LogException &e){
   cerr << "Log Exception Caught: " << e.GetReason() << nl;
  }
  va_list args;
  va_start(args, fmt);
  
  time_t t = time(NULL);
  struct tm *tm = localtime(&t);
  
  char buf[512];
  strftime(buf, sizeof(buf) - 1, "[%b %d %H:%M:%S %Y] ", tm);
  log << buf;
  vsnprintf(buf, sizeof(buf), fmt, args);
  log << strip(buf) << endl;
  va_end(args);
  va_end(args);
  log.close();
}

/**
 * \fn bool IsValadChannel(const Flux::string nerp)
 * This function returns if the channel is valid or not.
 * \param nerp Channel sring to be tested.
 * \return True if the Flux::string is a valid channel, false otherwise.
 */
bool IsValidChannel(const Flux::string chan){
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
void reconnect(SocketIO *sock){
 SocketIO *oldsock = sock;
 delete oldsock;
 sock = new SocketIO(server, port); 
}
/** 
 * \fn static void shutdown(Socket &sock, Flux::string quitmsg)
 * \brief shutdown the bot process
 * \param quitmsg The reason for the shutdown.
 * \param sock The Socket class
 */
static void shutdown(Flux::string quitmsg){
 if(quitmsg.empty())
      quitmsg = "Quitting: No Reason";
  cout << quitmsg << endl;
  Send->command->quit(quitmsg);
  log(quitmsg.c_str());
  log("Logging ended.");
}
/**
 * \overload static void Rehash()
 */
static void Rehash(){
 cout << "Rehashing config file." << nl;
 try{
   binary_dir += "/bot.conf";
   INIReader config(binary_dir.c_str());
   if (config.ParseError() < 0) {
       throw ConfigException("Cannot load bot.conf");
   }
  cout << "\033[22;31mReading Config File\033[22;30m... \033[1m\033[22;32mCHECK\033[1m\033[22;36m"<<nl;
  ReadConfig(config);
  }catch(ConfigException &ex){
    cout << "\r\nConfig Exception was caught: \033[22;31m" << ex.GetReason() << "\033[22;37m" << nl;
    log("Config Exception Caught: ", stringify(ex.GetReason()).c_str());
  } 
}

/** 
 * \fn static void Rehash(Socket &sock)
 * \brief Reload the bot config file
 * \param sock The Socket class
 */
static void Rehash(SendMessage *Send){
 cout << "Rehashing config file." << nl;
 try{
   binary_dir += "/bot.conf";
   INIReader config(binary_dir.c_str());
   if (config.ParseError() < 0) {
       throw ConfigException("Cannot load bot.conf");
   }
  cout << "\033[22;31mReading Config File\033[22;30m... \033[1m\033[22;32mCHECK\033[1m\033[22;36m"<<nl;
  ReadConfig(config);
  }catch(ConfigException &ex){
    cout << "\r\nConfig Exception was caught: \033[22;31m" << ex.GetReason() << "\033[22;37m" << nl;
    log("Config Exception Caught: ", stringify(ex.GetReason()).c_str());
    Send->notice(owner_nick, "Config Exception Caught: %s", stringify(ex.GetReason()).c_str());
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
      Rehash();
      break;
    case SIGPIPE:
      signal(sig, SIG_IGN);
      break;
    case SIGINT:
    case SIGTERM:
      signal(sig, SIG_IGN);
      signal(SIGHUP, SIG_IGN);
      cout << "\r\n\033[0m";
      sigstr = "Someone used Ctrl + C";
      signal(sig, SIG_IGN);
      quitmsg = "Recieved Signal: "+sigstr;
      quitting = true;
      break;
    default:
      quitmsg = "Recieved weird signal from terminal. Sig Number: "+stringify(sig);
      cout << "\r\n\033[0m";
      throw CoreException("Recieved weird signal from terminal. Signal Number: "+stringify(sig));
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
  try{
    binary_dir += "/bot.conf";
    INIReader config(binary_dir.c_str());
    if (config.ParseError() < 0) {
       throw ConfigException("Cannot load bot.conf");
    }
    ReadConfig(config);
  }catch(ConfigException &ex){
    cout << "\r\nConfig Exception was caught: \033[22;31m" << ex.GetReason() << "\033[22;37m" << nl;
    log("Config Exception Caught: ", stringify(ex.GetReason()).c_str());
    exit(1);
  }
    signal(SIGTERM, sigact);
    signal(SIGINT, sigact);
    signal(SIGHUP, sigact);
  //gets the command line paramitors if any.
  int Terminal = isatty(0) && isatty(1) && isatty(2);
  if (argc < 1 || argv[1] == NULL){
  }else{
    Flux::string arg = argv[1];
    for(int Arg=0; Arg < argc; Arg++){
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
        help(argv);
       }
       else if (arg == "--version" || arg == "-v"){
         cout << "Navn IRC C++ Bot Version " << version << nl;
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
  }
  }
   WritePID();
   log("Navn Started. PID: %d", getpid());
   if (!nofork){
	int i = fork();
	if(i < 0)
		throw CoreException("Unable to fork");
	else if (i != 0){
		cout << "Navn IRC Bot v" << version << " Started." << nl;
		cout << "Forking to background. PID: "<< i << nl;
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

#endif
