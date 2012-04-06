/* Navn IRC bot -- Miscellaneous Functions
 *
 * (C) 2011-2012 Azuru
 * Contact us at Development@Azuru.net
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Anope by The Anope Team.
 */
#include "module.h"
#include "INIReader.h"

//General misc functions
/** Random Number Generator
 *  This will generate a random number x is start number, y is the stop number.
 *  \param randint(int x, int y)
 */
int randint(int x, int y)
{
  srand(time(NULL));
  return rand()%(y-x+1)+x;
}

/**
 * \fn IsoHost(const Flux::string &fullhost)
 * \brief Basic class for isolating and decoding a basic IRC hostname
 * \param fullhost A Flux::string containing the full host of an irc message
 */
IsoHost::IsoHost(const Flux::string &fullhost)
{
  nick = fullhost.isolate(':','!');
  raw = fullhost;
  host = fullhost.isolate('@',' ');
  ident = fullhost.isolate('!','@');
}

/**
 * \fn void Fork()
 * \brief This is a simple start-up function which justifies whether we fork to background or not.
 */
void Fork()
{
  if (!nofork && InTerm()){
    int i = fork();
    if(i > 0){
	    Log(LOG_TERMINAL) << "Navn IRC Bot v" << VERSION << " Started";
	    Log(LOG_TERMINAL) << "Forking to background. PID: " << i << "\033[0m";
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

/**
 * \fn Flux::string Flux::RandomNickString(size_t length)
 * \brief Generates a completely random, valid IRC nickname string, Good for temp nicknames.
 * \param length the size type of how long you want the string.
 */
Flux::string Flux::RandomNickString(size_t length)
{
  Flux::string randomchars;
  srand(time(NULL));
  for(unsigned i=0; i < length; ++i)
  {
    top:
    char c = static_cast<char>((rand() % ('z' - '0' + 1) + '0'));
    if(isalphibeticnum(c))
      randomchars += c;
    else
      goto top;
  }
  return randomchars;
}

/**
 * \fn Flux::string Flux::RandomString(size_t length)
 * \brief Similar to the Flux::RandomNickString, it generates a random string of valid ASCII characters.
 * \param length the size type of how long you want the string.
 */
Flux::string Flux::RandomString(size_t length)
{
  Flux::string randomchars;
  srand(static_cast<unsigned>(time(NULL)));
  for(unsigned i=0; i < length; ++i)
    randomchars += static_cast<char>((rand() % ('z' - '0' + 1) + '0'));
  return randomchars;
}

/**
 * \fn Flux::string StripColors(const Flux::string &input)
 * \brief Attempts to remove all mIRC color codes from an IRC message
 * \param buffer A Flux::string containing mIRC color codes
 * This function was an import of inspircd's mirc color stripper and was
 * rewritten into C++ for compatibility with Flux::strings
 */

// The following function was taken from m_stripcolor.cpp from InspIRCd 2.0 and
// modified to fit this project.
Flux::string StripColors(const Flux::string &input)
{
  /* refactor this completely due to SQUIT bug since the old code would strip last char and replace with \0 --peavey */
  Flux::string sentence = input;
  int seq = 0;
  Flux::string::iterator i,safei;
  for (i = sentence.begin(); i != sentence.end();)
  {
    if ((*i == 3))
      seq = 1;
    else if (seq && (( ((*i >= '0') && (*i <= '9')) || (*i == ',') ) ))
    {
      seq++;
      if ( (seq <= 4) && (*i == ',') )
	seq = 1;
      else if (seq > 3)
	seq = 0;
    }
    else
      seq = 0;

    if (seq || ((*i == 2) || (*i == 15) || (*i == 22) || (*i == 21) || (*i == 31)))
    {
      if (i != sentence.begin())
      {
	safei = i;
	--i;
	sentence.erase(safei);
	++i;
      }
      else
      {
	sentence.erase(i);
	i = sentence.begin();
      }
    }
    else
      ++i;
  }

  return sentence;
}

/**
 * \fn Flux::string Flux::Sanitize(const Flux::string &string)
 * \brief Strips ALL color codes and anything of the like from a string, including \001 chars and even terminal colors.
 * \param buffer A Flux::string containing mIRC color codes
 * This function was an import of eggdrops mirc stripper and was
 * rewritten into C++ for compatability with Flux::strings
 */
Flux::string Flux::Sanitize(const Flux::string &string)
{
 static struct special_chars
 {
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

  Flux::string ret = StripColors(string.c_str());

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
Flux::string make_pass()
{
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
 * \fn bool IsValidChannel(const Flux::string &chan)
 * This function returns if the channel is valid or not.
 * \param channel Channel sring to be tested.
 * \return True if the Flux::string is a valid channel, false otherwise.
 */
bool IsValidChannel(const Flux::string &chan)
{
 if (chan[0] != '#')
    return false;
 return true;
}

/**
 * \fn Flux::string printfify(const char *fmt, ...)
 * \brief Returns a formatted string similar to printf() just in a Flux::string
 * \param Text The text to format
 * \param Format The format, in printf() form, to be formatted
 * \return a formatted Flux::string
 */
Flux::string printfify(const char *fmt, ...)
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
 * \fn Flux::string VectorString(const Flux::vector &params)
 * \brief Returns a condensed string of a Flux::vector
 * \param Flux::string A string of a Flux::vector condensed
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
 * \fn Flux::vector SerializeString(const Flux::string &src, char delim)
 * \brief creates a vector that breaks down a string word-by-word using the delim as the separater
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
 * \brief returns if the bot is started in terminal
 * \return true if in terminal, false otherwise
 */
bool InTerm() { return isatty(fileno(stdout) && isatty(fileno(stdin)) && isatty(fileno(stderr))); }

void Cleanup()
{
  // a FIFO queue of all the pointers to delete
  std::queue<void*> ptrs_to_delete;

  // Schedule all channels for deletion
  for(Flux::insensitive_map<Channel*>::iterator it = ChanMap.begin(); it != ChanMap.end(); ++it)
    ptrs_to_delete.push(it->second);
  ChanMap.clear();

  // Schedule all Users for deletion
  for(Flux::insensitive_map<User *>::iterator it = UserNickList.begin(); it != UserNickList.end(); ++it)
    ptrs_to_delete.push(it->second);
  UserNickList.clear();

  while (!ptrs_to_delete.empty())
  {
    void *ptr = ptrs_to_delete.front();
    ptrs_to_delete.pop();

    if(ptr)
    {
      if(typeid(ptr) == typeid(User*))
	delete static_cast<User*>(ptr);
      if(typeid(ptr) == typeid(Channel*))
	delete static_cast<Channel*>(ptr);
    }
  }
  
  if(sock)
  {
    delete sock;
    sock = nullptr;
  }

  if(ircproto)
  {
    delete ircproto;
    ircproto = nullptr;
  }

}

/* butt-plug?
 * http://www.albinoblacksheep.com/flash/plugs */
