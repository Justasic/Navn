/* All code is licensed under GNU General Public License GPL v3 (http://www.gnu.org/licenses/gpl.html) */
#include "user.h"
#include "INIReader.h"

//General misc functions
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
/**Random Number Generator
 * This will generate a random number x is start number, y is the stop number.
 * @param randint(int x, int y)
 */
int randint(int x, int y){
  srand(time(NULL));
  return rand()%(y-x+1)+x;
}

Flux::string Flux::Sanitize(const Flux::string &string){
 static struct special_chars{
   Flux::string character;
   Flux::string replace;
   special_chars(const Flux::string &c, const Flux::string &r) : character(c), replace(r) { }
 }
 special[] = {
  special_chars("  ", " "),
  special_chars("\n",""),
  special_chars("\002",""),
  special_chars("\003",""),
  special_chars("\035",""),
  special_chars("\037",""),
  special_chars("\026",""),
  special_chars("\001",""),
  special_chars("","")
 };
  Flux::string ret = string.c_str();
  for(int i = 0; special[i].character.empty() == false; ++i){
    ret = ret.replace_all_cs(special[i].character, special[i].replace);
  }
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
    char buf[1024];
    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);
    return buf;
  }else
    return fmt;
}
/**
 * \fn Anope::string duration(const time_t &t)
 * Expresses in a string the period of time represented by a given amount
 * of seconds (with days/hours/minutes).
 * \param seconds time in seconds
 * \return buffer
 */
Flux::string duration(const time_t &t)
{
  /* We first calculate everything */
  time_t days = (t / 86400);
  time_t hours = (t / 3600) % 24;
  time_t minutes = (t / 60) % 60;
  time_t seconds = (t) % 60;

  if (!days && !hours && !minutes)
    return Flux::stringify(seconds) + " " + (seconds != 1 ? "seconds" : "second");
  else
  {
    bool need_comma = false;
    Flux::string buffer;
    if (days)
    {
      buffer = Flux::stringify(days) + " " + (days != 1 ? "days" : "day");
      need_comma = true;
    }
    if (hours)
    {
      buffer += need_comma ? ", " : "";
      buffer += Flux::stringify(hours) + " " + (hours != 1 ? "hours" : "hour");
      need_comma = true;
    }
    if (minutes)
    {
      buffer += need_comma ? ", " : "";
      buffer += Flux::stringify(minutes) + " " + (minutes != 1 ? "minutes" : "minute");
    }
    return buffer;
  }
}

Flux::string do_strftime(const time_t &t, bool short_output)
{
	tm tm = *localtime(&t);
	char buf[4098];
	strftime(buf, sizeof(buf), "%b %d %H:%M:%S %Y %Z", &tm);
	if (short_output)
		return buf;
	if (t < time(NULL))
		return Flux::string(buf) + " " + fsprintf("(%s ago)", duration(time(NULL) - t).c_str());
	else
		return Flux::string(buf) + " " + fsprintf("(%s from now)", duration(t - time(NULL)).c_str());
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
 * \fn void log(LogType type = LOG_NORMAL, const char *fmt, ...)
 * This is what logs everything that goes on with the bot
 * \param LogType the kind of log you are making
 * \param char* the format and the string for the log
 */
void log(LogType type, const char *fmt, ...){
  std::fstream log;
  Flux::string timestamp;
  va_list args;
  va_start(args, fmt);
  
  time_t t = time(NULL);
  struct tm *tm = localtime(&t);
  
  char buf[512];
  std::stringstream ss;
  strftime(buf, sizeof(buf) - 1, "[%b %d %H:%M:%S %Y]", tm);
  timestamp = buf;
  vsnprintf(buf, sizeof(buf), fmt, args);
  ss << Flux::Sanitize(buf) << std::endl;
  if((type == LOG_TERMINAL)){
   printf("%s", ss.str().c_str());
   return;
  }
  try{
  log.open(Config->LogFile.c_str(), std::fstream::in | std::fstream::out | std::fstream::app);
  if(!log.is_open()){
    std::stringstream lerr;
    if(!Config->LogFile.empty())
      lerr << "Failed to open log file "<< Config->LogFile <<": " << strerror(errno);
    else
      lerr << "Cannot find logfile.";
     throw LogException(lerr.str().c_str());
  }
  }catch (LogException &e){
   std::cerr << "Log Exception Caught: " << e.GetReason() << std::endl;
  }
  if((type == LOG_RAWIO || type == LOG_DEBUG) && protocoldebug){
    printf("%s %s", timestamp.c_str(), ss.str().c_str());
    log << timestamp << " " << ss.str();
  }
  else if((type == LOG_DEBUG) && dev){
    if(nofork)
      printf("%s %s", timestamp.c_str(), ss.str().c_str());
    log << timestamp << " " << ss.str();
  }else if((type == LOG_NORMAL)){
    printf("%s %s", timestamp.c_str(), ss.str().c_str());
    log << timestamp << " " << ss.str();
  }
  va_end(args);
  va_end(args);
  log.close();
}
/** 
 * \fn Flux::string TempFile(const Flux::string &file)
 * \brief Creates a temporary file name for use in navn, can be quite useful.
 * \param Flux::string The Flux::string of the file location/name
 * NOTE: THIS _MUST_ HAVE 6 X's (XXXXXX) to work properly.
 */
Flux::string TempFile(const Flux::string &file){
char *tmp_output = strdup(file.c_str());
int target_fd = mkstemp(tmp_output);
if (target_fd == -1 || close(target_fd) == -1)
{
	free(tmp_output);
	return "";
}
Flux::string filestring = tmp_output;
free(tmp_output);
return filestring;
}
/** 
 * \fn std::vector<Flux::string> StringVector(const Flux::string &src, char delim)
 * \brief creates a vector that breaks down a string word-by-word using the delim as the seperater
 * \param src The source string for it to break down
 * \param delim The char used to seperate the words in the source string
 */
std::vector<Flux::string> StringVector(const Flux::string &src, char delim){
 sepstream tok(src, delim);
 Flux::string token;
 std::vector<Flux::string> ret;
 while(tok.GetToken(token))
   ret.push_back(token);
 return ret;
}

/* butt-plug?
 * http://www.albinoblacksheep.com/flash/plugs */