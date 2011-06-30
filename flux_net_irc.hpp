#ifndef DERP_H
#define DERP_H
#include <fstream>
#include "includes.h"
#include "defs.h"
template<typename T> inline std::string stringify(const T &x){
	std::ostringstream stream;
	if(!(stream << x))
		throw CoreException("Stringify Fail");
	return stream.str();
}
namespace flux_net_irc{
  /** 
   * \namespace flux_net_irc
   * \brief This is where we keep all our algorithms
   * 
   * We made this namespace so we could group all the common functions
   * and algorithms we use really often in one place. These will 
   * hopefully help anyone else that tries to extend navn.
 */
  using namespace std;
class irc_string:string{
  /** \class irc_string
 * NOTE this MUST be included in the main file.
 * This class wraps around a string (usually revieved from the irc server)
 * and attempts to decode all the info recieved from the server
 * so as to make it easier to read different kinds of information
 * (ei. who said something, their host, username, etc.)
 * \param toks Private vector that will contain the words in \a message.
 * \param raw_string Will hold the exact string that was recieved from the server.
 * \param usernick Will hold the usernick of the person who sent the message.
 * \param host Will hold the host of the person who sent the message.
 * \param user Will hold the user of the person who sent the message.
 * \param channel Will hold the channel of where the message was said.
 * \param message Will hold the message (stripped of irc protocol stuff)
 */
  private :
    vector<string> toks;
  public :
    string raw_string;
    string usernick; 
    string host; 
    string user; 
    string channel; 
    string message; 
    /**
     *\fn irc_string(string reply)
     *Constructor for \a irc_string class
     * This is where the magic happens. The constructor takes the string
     * and breaks it down into its component parts to find the 
     * \a usernick \a host \a user \a channel and \a message
     */   
    irc_string(string reply){
      raw_string = reply;
      usernick = isolate(':','!',reply);
      host = isolate('@',' ',reply);
      user = isolate('!','@',reply);
      channel = '#'+isolate('#',' ',reply);
      string space = "  ";
      size_t pos = reply.find(" :");
      pos += 2;
      for (int i = pos; i < reply.length(); i++){
	if (reply.at(i) == ' '){
	  message.append(space);
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
    /**
     * \fn string params(int i)
     * \brief Returns individual words from the message of a reply
     * Because \a toks is private, this is its "get" function.
     * We made this so someone writing a module doesn't try to go out 
     * of bounds while accessing an array.
     * \param i An integer value.
     * \return A string with the single specified word.
     */
    string params(int i){
      if (i >= toks.size()){
	return " ";
      }else{return toks[i];}
    }
    /**
     * \overload string params(int b, int e)
     * \brief Overload of params. Returns a range of words.
     * We overloaded \a params() so that you could get a range of words from the message
     *  as requeseted by Justasic.
     * \param b An integer value describing the place of the first word wanted.
     * \param e An integer value describing the place of the last word wanted.
     * \return A string withing the range of words specified by \a b and \a e
     */
    string params(int b, int e){
      string buf = "";
      if (b >= toks.size()){
	b = toks.size();
      }
      if (e >= toks.size()){
	e = toks.size() - 1;
      }
      for (int i = b; i <= (e); i++){
	buf += toks[i];
	buf.append(" ");
      }
      return buf;
    }
    /**
     * \fn static string isolate(char begin, char end, string msg)
     * \brief Isolates a string between two characters
     * Finds the first character, then begins to add every consecutive character from there to a string
     *  until it reaches the end character.
     * \param begin The character saying where the cut should begin.
     * \param end The character saying where the cut should end.
     * \param msg The string you are wanting to isolate from.
     */
    static string isolate(char begin, char end, string msg){
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
    
    /**
     * \fn bool said(string findee)
     * \brief Check if something was said.
     * \param findee The string you want to check if was said.
     * \return True if \a findee was found, false otherwise.
     */
    bool said(string findee){
      int i = raw_string.find(findee);
      if (i != string::npos){
	return true;
      }else{return false;}
    }
    /**
     * \overload static bool said(string source, string findee)
     * \brief Static overload of said() function.
     * \param source A string that is to be searched through.
     * \param findee The string that is to be found.
     * We overloaded the said function and made it static because we thought it would be 
     *  very useful to have outside of an \a irc_string object.
     * \return True if \a findee was found, false otherwise.
     */
    static bool said(string source, string findee){
      int i = source.find(findee);
      if (i != string::npos){
	return true;
      }else{return false;}
    }
};

/**
 * \class IsoHost
 * \brief Wrapper for an irc host
 * This was written by Justasic to break up the parts of a messages host for easier use.
 */
class IsoHost:string{
public:
    string raw;
    string nick;
    string host;
    string user;
    /**
     * \fn IsoHost(string fullhost)
     * \param fullhost A string containing the full host of an irc message
     */
    IsoHost(string fullhost){
      nick = irc_string::isolate(':','!',fullhost);
      raw = fullhost;
      host = irc_string::isolate('@',' ',fullhost);
      user = irc_string::isolate('!','@',fullhost);
    }
};

/** 
 * \fn string search(string s, string command)
 * \brief generates search links for url's
 * This is what generates the search links.
 */
string search(string s, string command){
  string raw_searchstring;
  string searchstring;
  size_t pos = s.find(command);
  raw_searchstring = s.substr(pos+(command.size())+1);
  for (int i=0; i < raw_searchstring.length(); i++){
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

/** 
 * \fn string removeCommand(string command, string s)
 * \brief Removes a command from a string.
 * \param command String to be taken out.
 * \param s Original string.
 * This takes out \a command from \a s and returns \a s without \a command It's very useful when you want 
 * to use the rest of a string as an argument for a command.
 * \return A string \a s without \a command.
 */
string removeCommand(string command, string s){
  size_t pos = s.find(command);
  return s.substr(pos+(command.size())+1);
}

/** 
 * \fn string makeSearchString(string raw_searchstring)
 * \brief Replaces special chars in a string with url compliant codes.
 * \param raw_searchstring
 * Goes through each character in a string and if it finds a special character, 
 * it replaces it with what would be in a url for that character.
 * \return A string without any special characters other than %
 */
string makeSearchString(string raw_searchstring){
  string searchstring;
  for (int i=0; i < raw_searchstring.length(); i++){
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
 * \fn string execute(const char *cmd)
 * \brief Sends a command to the OS
 * \param *cmd A command
 * \return A string containing the response from the OS.
 */
string execute(const char *cmd) {
    #ifdef _WIN32
    FILE* pipe = _popen
    #else
    FILE* pipe = popen(cmd, "r");
    #endif
    if (!pipe) return "ERROR";
    char buffer[128];
    string result = "";
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
 * \fn string make_two_digits(int x)
 * \brief Makes single digit int into a double digit string.
 * This was really just for the \a world_clock module but it's handy to have around.
 * \param x A single digit integer.
 * \return A string containing integer as a double digit.
 */
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
/** \fn string strip(const string &buf)
 * \brief Strips \r and \n from a string
 * Takes out the '\r' and '\n' from a string. Mostly used in messing with irc replies.
 * \param &buf String to be stripped
 * \return \a &buf without the '\r' and '\n'.
 */
string strip(const string &buf){
	string newbuf = buf;
	char c = newbuf[newbuf.size() - 1];
	while (c == '\n' || c == '\r'){
		newbuf.erase(newbuf.end() - 1);
		c = newbuf[newbuf.size() - 1];
	}
	return newbuf;
}

/**
 *\fn  string os_time()
 *\brief Get the operating system's time
 * This is just a simple function that gets the time.
 * \return A string of the current system time.
 */
string os_time(){
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
 * \fn bool IsValadChannel(const string nerp)
 * This function returns if the channel is valid or not.
 * \param nerp Channel sring to be tested.
 * \return True if the string is a valid channel, false otherwise.
 */
bool IsValadChannel(const string nerp){
 if (nerp[0] != '#')
    return true;
 return false;
}

/** 
 * \fn string make_pass()
 * \brief Makes a random password
 * This generates a 5 number random password for the bots
 * quit and other password protected commands.
 * \return A string containing the 5 digit password.
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

/**
 * \fn string kick(string channel, string user, string reason)
 * \brief Handles kick requests
 * NOTE: must be used with \a sock
 * \param channel Channel to be kicked from.
 * \param user User to be kicked.
 * \param reason Reason for the kick.
 * \return A string that, if sent to the socket, will kick the \a user from \a channel.
 */
string kick(string channel, string user, string reason){
 stringstream kick_ss;
 kick_ss << "KICK " << channel << " " << user <<" "<< reason << nl;
 return kick_ss.str();
}

string kick(string Channel, string User, const char *fmt, ...){
va_list args;
va_start(args, fmt);
stringstream kick_ss;
char buf[4096];
vsnprintf(buf, sizeof(buf), fmt, args);
kick_ss << "KICK " << Channel << " " << User << " :" << buf << nl;
va_end(args);
return kick_ss.str();
}

/** 
 * \fn string me(string dest, string message)
 * \brief Handles the IRC action function (/me)
 * NOTE: must be used with \a sock
 * \param dest The channel to perform the 'action'.
 * \param message The 'action' to be performed.
 * \return A string that, if sent to the socket, will perform the \a message in channel \a dest.
 */
string me(string dest, string message){
 stringstream me_ss;
 me_ss << "PRIVMSG " << dest << " :\001ACTION " << message << "\001" << nl;
 return me_ss.str();
}

/**
 * \overload string me(string Dest, const char *fmt, ...)
 */
string me(string Dest, const char *fmt, ...){
va_list args;
va_start(args, fmt);
stringstream me_ss;
char buf[4096];
vsnprintf(buf, sizeof(buf), fmt, args);
me_ss << "PRIVMSG " << Dest << " :\001ACTION " << buf << "\001" << nl;
va_end(args);
return me_ss.str();
}

/** 
 * \fn string privmsg(string stringy1, string stringy2)
 * \brief Sends a PRIVMSG to a user, service, or channel
 * NOTE: must be sent with \a sock
 * \param stringy1 Who to send it to.
 * \param stringy2 What to send
 * \return A string that, if sent to the socket, will send a message \a stringy2 to a user \a stringy1
 */
string privmsg(string stringy1, string stringy2){
  stringstream stringy_ss_privmsg;
  stringy_ss_privmsg << "PRIVMSG " << stringy1 << " :" << stringy2 << nl;
  return stringy_ss_privmsg.str();
}

/**
 * \overload string privmsg(string Dest, char* message)
 */
string privmsg(string Dest, char* message){
  stringstream stringy_ss_privmsg;
  stringy_ss_privmsg << "PRIVMSG " << Dest << " :" << message << nl;
  return stringy_ss_privmsg.str();
}

/**
 * \overload string privmsg(string Dest, const char *fmt, ...)
 */
string privmsg(string Dest, const char *fmt, ...){
va_list args;
va_start(args, fmt);
stringstream privmsg_ss;
char buf[4096];
vsnprintf(buf, sizeof(buf), fmt, args);
privmsg_ss << "PRIVMSG " << Dest << " :" << buf << nl;
va_end(args);
return privmsg_ss.str();
}

/**
 * \fn string samode(string Dest, const char *fmt, ...)
 * \brief Sends an IRC /samode command
 */
string samode(string Dest, const char *fmt, ...){
va_list args;
va_start(args, fmt);
stringstream samode_ss;
char buf[4096];
vsnprintf(buf, sizeof(buf), fmt, args);
samode_ss << "SAMODE " << Dest << " :" << buf << nl;
va_end(args);
return samode_ss.str();
}

/**
 * \overload string topic(string Dest, const char *fmt, ...)
 */
string topic(string Dest, const char *fmt, ...){
va_list args;
va_start(args, fmt);
stringstream topic_ss;
char buf[4096];
vsnprintf(buf, sizeof(buf), fmt, args);
topic_ss << "TOPIC " << Dest << " :" << buf << nl;
va_end(args);
return topic_ss.str();
}

/**
 * \fn string topic(string Dest, string buf)
 * \brief Sets channel topic.
 */
string topic(string Dest, string buf){
stringstream topic_ss;
topic_ss << "TOPIC " << Dest << " :" << buf << nl;
return topic_ss.str();
}

/**
 * \fn string oper(const char *msg, ...)
 * \brief Sends IRC command /oper
 */
string oper(const char *msg, ...){
va_list args;
va_start(args, msg);
stringstream oper_ss;
char buf[4096];
vsnprintf(buf, sizeof(buf), msg, args);
oper_ss << "OPER " << buf << nl;
va_end(args);
return oper_ss.str();
}

/** 
 * \fn string chanmsg(string stringy)
 * \brief This function sends a channel message to the pre-defined channel in the defs.h file.
 * NOTE: must be sent with \a sock
 * \param stringy String to send to channel.
 * \deprecated This function is still available but we discourage its use due to the fact that it does not support mutilple channels.
 */
string chanmsg(string stringy){
  stringstream stringy_ss;
  stringy_ss << "PRIVMSG " << channel << " " << stringy << nl;
  return stringy_ss.str();
}

/** 
 * \fn string setnick(string nickname)
 * \brief Sets the bots nickname in IRC.
 * NOTE: must be sent with \a sock
 * \param nickname A string with the new nickname.
 */
string setnick(string nickname){
 stringstream nick_ss;
 nick_ss << "NICK " << nickname << nl;
 return nick_ss.str();
}

/** 
 * \fn string join(string stringy_chan)
 * \brief Makes the bot join a channel
 * NOTE: must be sent with \a sock.
 * \param stringy_chan A string with the channel you want to join.
 */
string join(string stringy_chan){
  stringstream stringy_join_ss;
  stringy_join_ss << "JOIN " << stringy_chan << nl;
  return stringy_join_ss.str();
}

/** 
 * \fn string part(string channel, string reason)
 * \brief Sends part with message
 * NOTE: must be used with \a sock.
 * \param channel Channel to part from.
 * \param reason Reason for parting.
 */
string part(string channel, string reason){
  stringstream part_ss;
  part_ss << "PART " << channel << " " << reason << nl;
  return part_ss.str();
}
/** 
 * \overload string part(string channel)
 * \brief Parts channel w/o reason.
 * NOTE: must be used with \a sock.
 * \param channel Channel to part from.
 */
string part(string channel){
  stringstream part_ss;
  part_ss << "PART "<< channel << nl;
  return part_ss.str();
}

/** 
 * \fn string whois(string Nick)
 * \brief Sends a IRC Whois to Server.
 * NOTE: must be sent with \a sock.
 * \param Nick Nick to query
 */
 string whois(string Nick){
  stringstream whois_ss;
  whois_ss << "WHOIS " << Nick << nl;
  return whois_ss.str();
 }
 
/** \fn string notice(string Destination, string Message)
 * \brief Sends a IRC notice to a user or channel
 * NOTE: must be sent with \a sock.
 * \param Destination Who to send the notice to.
 * \param Message Message to send to \a Destination.
 */
string notice(string Destination, string Message){
  stringstream notice_ss;
  notice_ss << "NOTICE " << Destination << " :" << Message << nl;
  return notice_ss.str();
}
/**
 * \overload  string notice(string Destination, string Message)
 * \brief Sends a IRC notice to a user or channel
 * NOTE: must be sent with \a sock.
 */
string notice(string Dest, const char *fmt, ...){
  stringstream notice_ss;
  va_list args;
  va_start(args, fmt);
  char buf[4096];
  vsnprintf(buf, sizeof(buf), fmt, args);
  notice_ss << "NOTICE " << Dest << " :" << buf << nl;
  va_end(args);
  return notice_ss.str();
}

/** 
 * \fn string mode(string nickname, string mode)
 * \brief Sends a mode to be set in IRC
 * NOTE: must be sent with \a sock.
 * \param nickname Nickname of who we are setting a more to.
 * \param mode The mode to set.
 */
string mode(string nickname, string mode){
  stringstream stringy_ss_mode;
  stringy_ss_mode << "MODE " << nickname << " " << mode << nl;
  string out_put = stringy_ss_mode.str();
  return out_put;
}

/** 
 * \fn string chmode(string channel, string mode, string username)
 * \brief Sends a channel mode to the IRC network.
 */
string chmode(string channel, string mode, string username){
  stringstream stringy_ss_mode;
  stringy_ss_mode << "MODE " << channel << " " << mode << " " << username << nl;
  string out_put = stringy_ss_mode.str();
  return out_put;
}
static void remove_pidfile()
{
 remove(pid_file.c_str());
}
/** Quit Function
 * Sends a disconnect message to IRC
 * @param quit(reason)
 */
string quit(string msg){
  stringstream msg_ss;
  msg_ss << "QUIT " << msg << nl;
  return msg_ss.str();
}
string quit(const char *fmt, ...){
va_list args;
va_start(args, fmt);
stringstream quit_ss;
char buf[4096];
vsnprintf(buf, sizeof(buf), fmt, args);
quit_ss << "QUIT :" << buf << nl;
va_end(args);
return quit_ss.str();
}
static void DoQuit(Socket &s, const string msg){
  s.Send("QUIT :"+msg+nl);
  log("Quitting: %s", msg.c_str());
  log("Logging ended.");
  exit(0);
}
static void DoQuit(Socket &s, const string msg, int K){
  s.Send("QUIT :"+msg+nl);
  log("Quitting: %s", msg.c_str());
  log("Logging ended.");
  exit(K);
}
static void DoQuit(int K){
  log("Logging ended.");
  exit(K);
}
static void DoQuit(){
  log("Logging ended.");
  exit(0);
}
/** Restart Function
 * Restarts the Bot
 * @param restart(message)
 */
static void restart(string reason){
   char CurrentPath[FILENAME_MAX];
   GetCurrentDir(CurrentPath, sizeof(CurrentPath));
 if(reason.empty()){
  throw CoreException("Recieved an empty string on shutdown.");
  }else{
    log("Restarting: %s", reason.c_str());
	chdir(CurrentPath);
    int execvpret = execvp(my_av[0], my_av);
	if(execvpret > 0){
		throw CoreException("Restart Failed, Exiting");
	}
	remove_pidfile();
	exit(1);
  }
}
static void shutdown(Socket &sock, string quitmsg){
 if(quitmsg.empty())
      quitmsg = "Quitting: Unknown reason";
    cout << quitmsg << endl;
    sock << quit(quitmsg);
    DoQuit(sock, quitmsg); 
}
static void Rehash(){
 cout << "Rehashing config file." << nl;
 try{
  INIReader config("bot.conf");
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
static void Rehash(Socket &sock){
 cout << "Rehashing config file." << nl;
 try{
  INIReader config("bot.conf");
    if (config.ParseError() < 0) {
       throw ConfigException("Cannot load bot.conf");
    }
  cout << "\033[22;31mReading Config File\033[22;30m... \033[1m\033[22;32mCHECK\033[1m\033[22;36m"<<nl;
  ReadConfig(config);
  }catch(ConfigException &ex){
    cout << "\r\nConfig Exception was caught: \033[22;31m" << ex.GetReason() << "\033[22;37m" << nl;
    log("Config Exception Caught: ", stringify(ex.GetReason()).c_str());
    sock << notice(owner_nick, "Config Exception Caught: %s", stringify(ex.GetReason()).c_str());
  } 
}
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
/**This is the startup sequence that starts before the try loop starts
 * @param startup(int, char)
 */
void startup(int argc, char** argv) {
  //gets the command line paramitors if any.
  int Terminal = isatty(0) && isatty(1) && isatty(2);
  if (argc < 1 || argv[1] == NULL){
  }else{
    string arg = argv[1];
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
        help();
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
		cout << "Navn IRC Bot " << version << " Started." << nl;
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
string trim(string const& source, char const* delims = " \t\r\n") {
  string result(source);
  string::size_type index = result.find_last_not_of(delims);
  if(index != string::npos)
    result.erase(++index);
  index = result.find_first_not_of(delims);
  if(index != string::npos)
    result.erase(0, index);
  else
    result.erase();
  return result;
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
    case SIGHUP:
    {
      signal(sig, SIG_IGN);
      Rehash();
      break;
    }
    case SIGINT:
    case SIGTERM:
      signal(sig, SIG_IGN);
      signal(SIGHUP, SIG_IGN);
      cout << "\r\n\033[0m";
      sigstr = "Someone killed me";
      signal(sig, SIG_IGN);
      quitmsg = "Recieved Signal: "+sigstr;
      quitting = true;
      throw CoreException(quitmsg);
      break;
    default:
      quitmsg = "Recieved weird signal from terminal. Sig Number: "+stringify(sig);
      cout << "\r\n\033[0m";
      throw CoreException("Recieved weird signal from terminal. Signal Number: "+stringify(sig));
  }
}

string xmlToString(string fileName){
  string buf;
  string line;
  ifstream in(fileName.c_str());
  while(std::getline(in,line)){
    buf += line;
  }
  return buf;
}

string findInXML(string node, string info, string fileString){
 
  string findee = "<"+node;
  size_t p = fileString.find(findee);
  bool foundInfo = false;
  int a = 0;
  while(!foundInfo){
    string infoFound = "";
    for(int m = 0; m < info.length(); m++){
      infoFound += fileString.at(p+a+m);
    }
    if (infoFound == info){
      foundInfo = true;
    }else{a++;}
  }
  p += a+info.length()+2;
  string output;
  int i = 0;
  while(!(fileString.at(p+i) == '"')){
    output += fileString.at(p+i);
    i++;
  }
  return output;
}

}
#endif
