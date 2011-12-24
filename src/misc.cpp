/* All code is licensed under GNU General Public License GPL v3 (http://www.gnu.org/licenses/gpl.html) */
#include "module.h"
#include "INIReader.h"

//General misc functions
/**Random Number Generator
 * This will generate a random number x is start number, y is the stop number.
 * @param randint(int x, int y)
 */
int randint(int x, int y)
{
  srand(time(NULL));
  return rand()%(y-x+1)+x;
}

/**
 * \fn IsoHost(const Flux::string &fullhost)
 * \param fullhost A Flux::string containing the full host of an irc message
 */
IsoHost::IsoHost(const Flux::string &fullhost)
{
  nick = isolate(':','!',fullhost);
  raw = fullhost;
  host = isolate('@',' ',fullhost);
  ident = isolate('!','@',fullhost);
}

void Fork()
{
  if (!nofork && InTerm()){
    int i = fork();
    if(i > 0){
	    Log(LOG_TERMINAL) << "Navn IRC Bot v" << VERSION << " Started";
	    Log(LOG_TERMINAL) << "Forking to background. PID: " << i << "\033[22;37m";
	    FOREACH_MOD(I_OnFork, OnFork(i));
	    exit(0);
    }
    if(isatty(fileno(stdout)))
      fclose(stdout);
    if(isatty(fileno(stdin)))
      fclose(stdin);
    if(isatty(fileno(stderr)))
      fclose(stderr);
    if(setpgid(0, 0) < 0)
	    throw CoreException("Unable to setpgid()");
    else if(i == -1)
      Log() << "Error, unable to fork: " << strerror(errno);
  }else
    Log() << Config->BotNick << " Started, PID: " << getpid() << "\033[22;36m";
}

Flux::string Flux::Sanitize(const Flux::string &string)
{
 static struct special_chars{
   Flux::string character;
   Flux::string replace;
   special_chars(const Flux::string &c, const Flux::string &r) : character(c), replace(r) { }
 }
 special[] = {
  special_chars("  ", " "),
  special_chars("\n",""),
  special_chars("\002",""),
  special_chars("\035",""),
  special_chars("\037",""),
  special_chars("\026",""),
  special_chars("\001",""),
  special_chars("","")
 };
  Flux::string ret = string.c_str();
  while(ret.search('\003')){ //Strip color codes completely
      size_t l = ret.find('\003');
      if(isdigit(ret[l+1]))
	ret = ret.erase(l, l+1);
      else if(isdigit(ret[l+2]))
	ret = ret.erase(l, l+2);
      else if(isdigit(ret[l+3]))
	ret = ret.erase(l, l+3);
  }
  for(int i = 0; special[i].character.empty() == false; ++i)
    ret = ret.replace_all_cs(special[i].character, special[i].replace);
  return ret.c_str();
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
  std::stringstream pass_ss;
  pass_ss << p1 << p2 << p3 << p4 << p5;
  return pass_ss.str();
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

Flux::string fsprintf(const char *fmt, ...)
{
  if(fmt)
  {
    va_list args;
    char buf[BUFSIZE];
    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);
    return buf;
  }else
    return fmt;
}

/**
  * \fn Flux::string isolate(char begin, char end, Flux::string msg)
  * \brief Isolates a Flux::string between two characters
  * Finds the first character, then begins to add every consecutive character from there to a Flux::string
  *  until it reaches the end character.
  * \param begin The character saying where the cut should begin.
  * \param end The character saying where the cut should end.
  * \param msg The Flux::string you are wanting to isolate from.
  */
Flux::string isolate(char begin, char end, const Flux::string &msg){
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
 * \fn Flux::string VectorString(const Flux::vector &params)
 * \brief Returns a condensed string of a Flux::vector
 * \param Flux::vector a vector containing a string that needs condensing
 */
Flux::string CondenseString(const Flux::vector &p)
{
  Flux::vector params = *const_cast<Flux::vector*>(&(p)); //Why is this so complex?
  Flux::string ret;
  for(Flux::vector::iterator it = params.begin(); it != params.end(); ++it)
    ret += *it + " ";
  ret.trim();
  return ret;
}

/**
 * \fn std::vector<Flux::string> StringVector(const Flux::string &src, char delim)
 * \brief creates a vector that breaks down a string word-by-word using the delim as the seperater
 * \param src The source string for it to break down
 * \param delim The char used to seperate the words in the source string
 */
Flux::vector SerializeString(const Flux::string &src, char delim)
{
 sepstream tok(src, delim);
 Flux::string token;
 std::vector<Flux::string> ret;
 while(tok.GetToken(token))
   ret.push_back(token);
 return ret;
}
/** Check if a file exists
 * \fn bool InTerm()
 * \brief returns if the 
 * \return true if the file exists, false if it doens't
 */
bool InTerm() { return isatty(fileno(stdout) && isatty(fileno(stdin)) && isatty(fileno(stderr))); }
/* butt-plug?
 * http://www.albinoblacksheep.com/flash/plugs */