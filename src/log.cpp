/* Navn IRC bot -- Logging classes
 *
 * (C) 2011-2012 Azuru
 * Contact us at Development@Azuru.net
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Anope by The Anope Team.
 */

#include "log.h"

/**
 * \fn Flux::string NoTermColor(const Flux::string &ret)
 * \brief Strip all *nix terminal style colors
 * \param buffer Buffer to strip
 */
Flux::string NoTermColor(const Flux::string &ret)
{
  Flux::string str;
  bool in_term_color = false;
  for(unsigned i=0; i < ret.length(); ++i)
  {
    char c = ret[i];
    if(in_term_color)
    {
      if(c == 'm')
	in_term_color = false;
      continue;
    }
    if(c == '\033')
    {
      in_term_color = true;
      continue;
    }
    if(!in_term_color)
      str += c;
  }
  return str;
}

/**
 * \fn static Flux::string GetLogDate(time_t t = time(NULL))
 * \brief Get the date for the log files that is user readable
 * \param time time to use on the log files
 * \return returns a string containing the human-readable date format
 */
static Flux::string GetLogDate(time_t t = time(NULL))
{
  char timestamp[32];

  time(&t);
  struct tm *tm = localtime(&t);

  strftime(timestamp, sizeof(timestamp), "%Y%m%d", tm);

  return timestamp;
}

/**
 * \fn static inline Flux::string CreateLogName(const Flux::string &file, time_t t = time(NULL))
 * \brief Returns a filename for the logs
 * \param file string containing the filename
 * \param time time to use on the filename
 * \return the filename that has been generated
 */
static inline Flux::string CreateLogName(const Flux::string &file, time_t t = time(NULL))
{
  return "logs/" + file + "." + GetLogDate(t) + "-" + value_cast<Flux::string>(t);
}

/**
 * \fn void CheckLogDelete(Log *log)
 * \brief Check to see if logs need to be removed due to old age
 * \param log A log class variable
 */
void CheckLogDelete(Log *log)
{
  Flux::string dir = Config->Binary_Dir+"/logs/";
  if(!TextFile::IsDirectory(dir))
  {
    Log(LOG_TERMINAL) << "Directory " << dir << " does not exist, making new directory.";
    if(mkdir(Flux::string(dir).c_str(), getuid()) != 0)
      throw LogException("Failed to create directory "+dir+": "+Flux::string(strerror(errno)));
  }

  Flux::vector files = TextFile::DirectoryListing(dir);
  if(log)
    files.push_back(log->filename);

  if(files.empty())
    Log(LOG_TERMINAL) << "No Logs!";

  for(Flux::vector::iterator it = files.begin(); it != files.end(); ++it)
  {
    Flux::string file = dir+(*it);

    if(TextFile::IsFile(file))
    {
      Flux::string t = file.isolate('-', ' ').strip('-');
      int timestamp = (int)t;

      if(timestamp > (time(NULL) - 86400 * Config->LogAge) && timestamp != starttime)
      {
	Delete(file.c_str());
	Log(LOG_DEBUG) << "Deleted old logfile " << file;
      }
    }
  }
}

/**
 * \fn Flux::string Log::TimeStamp()
 * \brief Returns a logging timestamp for use as log message prefixes
 * \return Generated human readable timestamp
 */
Flux::string Log::TimeStamp()
{
 char tbuf[256];
  time_t t;

  if (time(&t) < 0)
	  throw CoreException("time() failed");
  tm tm = *localtime(&t);
#if HAVE_GETTIMEOFDAY
  if (protocoldebug)
  {
    char *s;
    struct timeval tv;
    gettimeofday(&tv, NULL);
    strftime(tbuf, sizeof(tbuf) - 1, "[%b %d %H:%M:%S", &tm);
    s = tbuf + strlen(tbuf);
    s += snprintf(s, sizeof(tbuf) - (s - tbuf), ".%06d", static_cast<int>(tv.tv_usec));
    strftime(s, sizeof(tbuf) - (s - tbuf) - 1, " %Y]", &tm);
  }
  else
#endif
    strftime(tbuf, sizeof(tbuf) - 1, "[%b %d %H:%M:%S %Y]", &tm);

  return Flux::Sanitize(tbuf);
}

/**
 * \fn Log::Log(LogType t) : type(t), u(NULL), c(NULL)
 * \brief Logging class used for generating log files and messages
 */
Log::Log(LogType t) : type(t), u(NULL), c(NULL) {}
/// \overload Log::Log(LogType t, User *user):type(t), u(user), c(NULL)
Log::Log(LogType t, User *user):type(t), u(user), c(NULL) { if(!u) throw CoreException("No user argument in Log()"); }
/// \overload Log::Log(User *user): type(LOG_NORMAL), u(user), c(NULL)
Log::Log(User *user): type(LOG_NORMAL), u(user), c(NULL) { if(!u) throw CoreException("No user argument in Log()"); }
/// \overload Log::Log(User *user, Command *command):type(LOG_NORMAL), u(user), c(command)
Log::Log(User *user, Command *command):type(LOG_NORMAL), u(user), c(command)
{
  if(!u) throw CoreException("No user argument in Log()");
  if(!c) throw CoreException("No command argument in Log()");
}
/// \overload Log::Log(LogType t, User *user, Command *command): type(t), u(user), c(command)
Log::Log(LogType t, User *user, Command *command): type(t), u(user), c(command)
{
  if(!u) throw CoreException("No user argument in Log()");
  if(!c) throw CoreException("No command argument in Log()");
}

/**
 * \fn Log::~Log()
 * \brief Logging destructor, this is where the files are actually written and messages are logged
 */
Log::~Log()
{
  Flux::string LogColor;
  if(Config)
  {
    LogColor = Config->LogColor;
    this->filename = CreateLogName(Config->LogFile, starttime);
  }
  else
  {
    LogColor = "\033[0m"; // Whatever the terminal is set at if we cant get colors from the config pointer
    this->filename = "";
  }
  
  Flux::string message = Flux::Sanitize(this->buffer.str()), raw = this->buffer.str();
  std::stringstream logstream;

  if(this->u && !this->c)
    message = this->u->nick + " " + message;
  if(this->u && this->c)
    message = this->u->nick + " used " + this->c->name + " " + message;

  switch(type)
  {
    case LOG_SILENT:
    case LOG_NORMAL:
      logstream << TimeStamp() << " " << message;
      break;
    case LOG_THREAD:
      if(protocoldebug)
	logstream << TimeStamp() << " [THREAD] " << message;
      break;
    case LOG_DEBUG:
      if(dev || protocoldebug)
	logstream << TimeStamp() << " " << message;
      break;
    case LOG_RAWIO:
      if(protocoldebug)
	logstream << TimeStamp() << " " << message;
      break;
    case LOG_CRITICAL:
      logstream << "\033[22;31m" << TimeStamp() << " [CRITICAL] " << message << LogColor;
      break;
    case LOG_WARN:
      logstream << TimeStamp() << " \033[22;33m[WARNING]" << Config->LogColor << " " << message;
      break;
    case LOG_MEMORY:
      if(memdebug)
	std::cout << TimeStamp() << " [MEMORY] " << message << std::endl;
      return; // ignore everything else, it doesn't matter
    case LOG_TERMINAL:
      if(InTerm())
	std::cout << raw << std::endl;
      return;
    default:
      Log(LOG_CRITICAL) << "Wtf log case is this?";
      Log(LOG_TERMINAL) << "\033[22;33m[UNDEFINED]" << LogColor << " " << message;
  }

  EventResult result;
  FOREACH_RESULT(I_OnLog, OnLog(this), result);
  if(result != EVENT_CONTINUE)
    return;
  
  if((type != LOG_SILENT || type != LOG_CRITICAL) && InTerm())
    std::cout << (nocolor?NoTermColor(logstream.str()):logstream.str()) << std::endl;

  if(type == LOG_CRITICAL && InTerm()) // Log to stderr instead of stdout
    std::cerr << (nocolor?NoTermColor(logstream.str()):logstream.str()) << std::endl;

  if(this->filename.empty())
  {
    std::cerr << "\033[22;31m" << TimeStamp() << " [CRITICAL] Cannot find log file specified!" << LogColor << std::endl;
    return; // Exit if there's no file to log to
  }
  
  try
    {
      CheckLogDelete(this);
      log.open(this->filename.c_str(), std::fstream::out | std::fstream::app);
      
      if(!log.is_open())
      {
	if(!Config)
		throw LogException("Cannot read log file from config! (is there a bot.conf?)");
	else
		throw LogException(Config->LogFile.empty()?"Cannot open Log File.":
			Flux::string("Failed to open Log File "+this->filename+": "+strerror(errno)).c_str());
      }
      
      log << NoTermColor(logstream.str()) << std::endl;
      
      if(log.is_open())
	log.close();
    }
    catch (LogException &e)
    {
      if(InTerm())
	std::cerr << "Log Exception Caught: " << e.GetReason() << std::endl;
    }
}
