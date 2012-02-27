/* Navn IRC bot -- Main include
 * 
 * (C) 2011-2012 Flux-Net
 * Contact us at Dev@Flux-Net.net
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Anope by The Anope Team.
 *
 * WARNING: this file is going bye bye VERY SOON!
 */
#ifndef DERP_H
#define DERP_H
#include "module.h"
#include "defs.h"
#include "xmlfile.h"
IRCProto *ircproto;
SocketIO *sock;
BotConfig *Config;
module *LastRunModule;

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
   * \fn irc_string(Flux::string reply)
   * \deprecated This class is deprecated, find another way of using your strings
   *Constructor for \a irc_string class
   * This is where the magic happens. The constructor takes the Flux::string
   * and breaks it down into its component parts to find the
   * \a usernick \a host \a user \a channel and \a message
   */
  DEPRECATED(irc_string(Flux::string reply)){
    raw_string = reply;
    usernick = reply.isolate(':','!');
    host = reply.isolate('@',' ');
    user = reply.isolate('!','@');
    channel = '#'+reply.isolate('#',' ');
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
    char * cmessage = const_cast<char*>(fmessage.c_str());
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
   * \deprecated use module parameter vectors instead
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
   * \deprecated use the plain isolate function instead
   * Finds the first character, then begins to add every consecutive character from there to a Flux::string
   *  until it reaches the end character.
   * \param begin The character saying where the cut should begin.
   * \param end The character saying where the cut should end.
   * \param msg The Flux::string you are wanting to isolate from.
   */
  DEPRECATED(static Flux::string isolate(char begin, char end, Flux::string msg)){
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
   * \deprecated use Flux::string::search() and Flux::string::search_ci() instead
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
   * \deprecated
   * \param source A Flux::string that is to be searched through.
   * \param findee The Flux::string that is to be found.
   * We overloaded the said function and made it static because we thought it would be
   *  very useful to have outside of an \a irc_string object.
   * \return True if \a findee was found, false otherwise.
   */
  DEPRECATED(static bool said(Flux::string source, Flux::string findee)){
    unsigned i = source.find(findee);
    if (i != Flux::string::npos){
      return true;
    }else{return false;}
  }
};
/**
 * \fn Flux::string removeCommand(Flux::string command, Flux::string s)
 * \brief Removes a command from a Flux::string.
 * \param command String to be taken out.
 * \param s Original Flux::string.
 * \deprecated use module parameters system instead
 * This takes out \a command from \a s and returns \a s without \a command It's very useful when you want
 * to use the rest of a Flux::string as an argument for a command.
 * \return A Flux::string \a s without \a command.
 */
DEPRECATED(Flux::string removeCommand(Flux::string command, Flux::string s));
Flux::string removeCommand(Flux::string command, Flux::string s)
{
  size_t pos = s.find(command);
  return s.substr(pos+(command.size())+1);
}

/**
 * \fn Flux::string urlify(Flux::string raw_searchstring)
 * \brief Replaces special chars in a Flux::string with url compliant codes.
 * \param raw_searchstring
 * \deprecated use Flux::string::url_str() instead
 * Goes through each character in a Flux::string and if it finds a special character,
 * it replaces it with what would be in a url for that character.
 * \return A Flux::string without any special characters other than %
 */
DEPRECATED(Flux::string urlify(const Flux::string &received));
Flux::string urlify(const Flux::string &received){
  Flux::string string;
  for(unsigned i=0; i < received.size(); ++i){
    switch(received[i]){
      case '%': string = string+"%25"; break;
      case ' ': string = string+"%20"; break;
      case '+': string = string+"%2B"; break;
      case '$': string = string+"%24"; break;
      case '&': string = string+"%26"; break;
      case ',': string = string+"%2C"; break;
      case '/': string = string+"%2F"; break;
      case ':': string = string+"%3A"; break;
      case ';': string = string+"%3B"; break;
      case '=': string = string+"%3D"; break;
      case '?': string = string+"%3F"; break;
      case '@': string = string+"%40"; break;
      case '#': string = string+"%23"; break;
      case '>': string = string+"%3E"; break;
      case '<': string = string+"%3C"; break;
      case '{': string = string+"%7B"; break;
      case '}': string = string+"%7D"; break;
      case '|': string = string+"%7C"; break;
      case '\\':string = string+"%5C"; break;
      case '^': string = string+"%5E"; break;
      case '~': string = string+"%7E"; break;
      case '[': string = string+"%5B"; break;
      case ']': string = string+"%5D"; break;
      case '`': string = string+"%60"; break;
      case '*': string = string+"%2A"; break;
      case '(': string = string+"%28"; break;
      case ')': string = string+"%29"; break;
      case '\'':string = string+"%27"; break;
      case '"': string = string+"%22"; break;
      case '.': string = string+"%2E"; break;
      case '-': string = string+"%2D"; break;
      default:
	string = string+received[i];
    }
  }
  return string;
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
  FILE* pipe = popen(cmd, "r");
  if (!pipe) return "";
  char buffer[128];
  Flux::string result = "";
  while(!feof(pipe)) {
    if(fgets(buffer, 128, pipe) != NULL)
      result += buffer;
  }
  pclose(pipe);
  return result;
}
/**
 * \fn static void restart(Flux::string reason)
 * \brief Restart the bot process
 * \param reason The reason for the restart
 */
void restart(const Flux::string &reason){
  char CurrentPath[FILENAME_MAX];
  GetCurrentDir(CurrentPath, sizeof(CurrentPath));
  FOREACH_MOD(I_OnRestart, OnRestart(reason));
  if(reason.empty()){
    Log() << "Restarting: No Reason";
    if(ircproto)
      ircproto->quit("Restarting: No Reason");
  }else{
    Log() << "Restarting: " << reason;
    if(ircproto)
      ircproto->quit("Restarting: %s", reason.c_str());
  }
  chdir(CurrentPath);
  int execvpret = execvp(my_av[0], my_av);
  if(execvpret > 0)
    throw CoreException("Restart Failed, Exiting");
  Delete(Config->PidFile.c_str());
  exit(1);
}

/**
 * \fn void Rehash()
 * \brief Reload the bot config file
 * \param boolean this boolean tells rehash if we are starting from start or not
 */
void Rehash(){
  Log() << "Rehashing Configuration File";
  try{
    const Flux::string bi_dir = Config->Binary_Dir;
    BotConfig *configtmp = Config;
    Config = new BotConfig(bi_dir);
    delete configtmp;
    if(!Config)
      throw ConfigException("Could not read config.");
    FOREACH_MOD(I_OnReload, OnReload());
    ReadConfig();
  }catch(const ConfigException &ex){
    Log() << "Configuration Exception Caught: " << ex.GetReason();
    ircproto->notice(Config->Owner, "Config Exception Caught: %s", ex.GetReason());
  }
}

/**
 * \fn static void remove_pidfile()
 * \brief Removes the PID file on exit
 */
static void remove_pidfile() { Delete(Config->PidFile.c_str()); }
/**
 * \fn static void WritePID()
 * \brief Write the bots PID file
 */
static void WritePID(){
  //logging to a text file and making the PID file.
  if(Config->PidFile.empty())
    throw CoreException("Cannot write PID file, no PID file specified.");

  if(TextFile::IsFile(Config->PidFile))
  {
    Log(LOG_DEBUG) << "Deleting stale pid file " << Config->PidFile;
    Delete(Config->PidFile.c_str());
  }
  FILE *pidfile = fopen(Config->PidFile.c_str(), "w");
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
    throw CoreException("Can not write to PID file "+Config->PidFile);
}
/**This is the startup sequence that starts at the top to the try loop
 * @param startup(int, char)
 */
void startup(int argc, char** argv, char *envp[]) {
  SET_SEGV_LOCATION();
  
  InitSignals();
  Config = NULL;
  ircproto = NULL;
  sock = NULL;
  my_av = argv;
  my_envp = envp;
  starttime = time(NULL); //for bot uptime
  
  binary_dir = getprogdir(argv[0]);
  if(binary_dir[binary_dir.length() - 1] == '.')
    binary_dir = binary_dir.substr(0, binary_dir.length() - 2);
  
  Config = new BotConfig(binary_dir);
  if(!Config)
    throw CoreException("Config Error.");
  
  Flux::string dir = argv[0];
  Flux::string::size_type n = dir.rfind('/');
  dir = "." + dir.substr(n);
  
  //gets the command line paramitors if any.
  if (!(argc < 1) || argv[1] != NULL)
  {
    for(int Arg=1; Arg < argc; ++Arg)
    {
      Flux::string arg = argv[Arg];
      if((arg.equals_ci("--developer")) ^ (arg.equals_ci("--dev")) ^ (arg == "-d"))
      {
	dev = nofork = true;
	Log(LOG_DEBUG) << Config->BotNick << " is started in Developer mode. (" << arg << ")";
      }
      else if ((arg.equals_ci("--nofork")) ^ (arg == "-n"))
      {
	nofork = true;
	Log(LOG_DEBUG) << Config->BotNick << " is started With No Forking enabled. (" << arg << ")";
      }
      else if ((arg.equals_ci("--help")) ^ (arg == "-h"))
      {
	Log(LOG_TERMINAL) << "Navn Internet Relay Chat Bot v" << VERSION;
	Log(LOG_TERMINAL) << "Usage: " << dir << " [options]";
	Log(LOG_TERMINAL) << "-h, --help";
	Log(LOG_TERMINAL) << "-d, --developer";
	Log(LOG_TERMINAL) << "-n, --nofork";
	Log(LOG_TERMINAL) << "-p, --protocoldebug";
	Log(LOG_TERMINAL) << "-c, --nocolor";
	Log(LOG_TERMINAL) << "This bot does have Epic Powers.";
	exit(0);
      }
      else if ((arg.equals_ci("--version")) ^ (arg == "-v"))
      {
	Log(LOG_TERMINAL) << "Navn IRC C++ Bot Version " << VERSION_FULL;
	Log(LOG_TERMINAL) << "This bot was programmed from scratch by Justasic and Lordofsraam.";
	Log(LOG_TERMINAL) << "";
	Log(LOG_TERMINAL) << "IRC: IRC.Flux-Net.net #Computers";
	Log(LOG_TERMINAL) << "WWW: http://www.Flux-Net.net";
	Log(LOG_TERMINAL) << "Email: Staff@Flux-Net.net";
	Log(LOG_TERMINAL) << "Git: git://gitorious.org:navn/navn.git";
	Log(LOG_TERMINAL) << "";
	Log(LOG_TERMINAL) << "This bot does have Epic Powers.";
	Log(LOG_TERMINAL) << "Type " << dir << " --help for help on how to use navn, or read the readme.";
	exit(0);
      }
      else if((arg.equals_ci("--protocoldebug")) ^ (arg == "-p"))
      {
	protocoldebug = true;
	Log(LOG_RAWIO) << Config->BotNick << " is started in Protocol Debug mode. (" << arg << ")";
      }
      else if((arg.equals_ci("--nocolor")) ^ (arg == "-c"))
      {
	nocolor = true;
	Log() << Config->BotNick << " is started in No Colors mode. (" << arg << ")\033[0m"; //reset terminal colors
      }
      else
      {
	Log(LOG_TERMINAL) << "Unknown option " << arg;
	exit(0);
      }
    }
  }
  
  if(!nocolor)
    Log(LOG_TERMINAL) << "\033[22;36m";
  
  ModuleHandler::SanitizeRuntime();
  ReadConfig(); //load modules
  WritePID(); //Write the pid file
  FOREACH_MOD(I_OnStart, OnStart(argc, argv)); //announce we are starting the bot
  Fork(); //Fork to background
}

/***************************************************************************/
#endif
