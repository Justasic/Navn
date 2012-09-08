/* Navn IRC bot -- Signal Handler
 *
 * (C) 2011-2012 Azuru
 * Contact us at Development@Azuru.net
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Anope by The Anope Team.
 */

#include "includes.h"
#include "module.h"

#ifdef _WIN32
# define SIGHUP -1
# define SIGUSR1 -2
# define SIGPIPE -3
# define SIGKILL -4
#endif

/** Random Quit message selector
 * \fn Flux::string siginit(int sigstring)
 * \brief this selects a quit message out of 20, this should be used with a random number between 1 and 20.
 * \param int the string number to return.
 */
Flux::string siginit(int sigstring)
{
  Flux::string message;
  switch(sigstring)
  {
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
/** Segmentation Fault Handler
 * \fn void HandleSegfault(Module *m)
 * \brief A segfault handler to report what happened and where it happened.
 * \param Module the Module class in which the segfault happened include
 */
void HandleSegfault(Module *m)
{
#ifdef HAVE_BACKTRACE
bool havecxx11 = false; //Check if we compiled with C++11
#ifdef _CXX11
havecxx11 = true;
#endif
 void *array[10];
 char **strings;
 char tbuf[256];
 Flux::string mbuf;
 size_t size;
 time_t now = time(NULL);

 size = backtrace(array, 10);
 if(TextFile::IsFile("SEGFAULT.log"))
   Delete("SEGFAULT.log");
 std::stringstream slog;
 std::fstream sslog("SEGFAULT.log", std::ifstream::out | std::ifstream::app);
 if(sslog.is_open())
 {
   struct utsname uts;
   if(uname(&uts) < 0)
     throw CoreException("uname() Error");

   strftime(tbuf, sizeof(tbuf), "[%b %d %H:%M:%S %Y]", localtime(&now));
   slog << "====================== Segmentation Fault ======================" << std::endl;
   slog << "Please report this bug to http://bugs.Azuru.net/ and submit a bug report." << std::endl;
   slog << "Please note that the Azuru developers may ask you to re-run this under gdb!" << std::endl;
   slog << "Time of crash: " << tbuf << std::endl;
   slog << "Navn version: " << VERSION_FULL << std::endl;
   slog << "System info: " << uts.sysname << " " << uts.nodename << " " <<  uts.release << " " << uts.machine << std::endl;
   slog << "System version: " << uts.version << std::endl;
   slog << "C++ Version: " << __VERSION__ " C++11 support: " << (havecxx11?"true":"false") << std::endl;
   //if(sock)
    //slog << "Socket Buffer: " << sock->GetLastBuf() << std::endl;
   //else
     //slog << "Socket Buffer: No socket created." << std::endl;
   slog << "Location: " << segv_location << std::endl;
   if(m)
   {
     slog << "Module: " << m->name << std::endl;
     slog << "Module Version: " << m->GetVersion() << std::endl;
     slog << "Module Author: " << m->GetAuthor() << std::endl;
   }
   for(Flux::insensitive_map<Module*>::iterator it = Modules.begin(); it != Modules.end(); ++it)
     mbuf += it->second->name+" ";
   mbuf.trim();
   slog << "Modules Loaded: " << (mbuf.empty()?"None":mbuf) << std::endl;
   strings = backtrace_symbols(array, size);
   for(unsigned i=1; i < size; i++)
     slog << "BackTrace(" << (i - 1) << "): " << strings[i] << std::endl;
   free(strings);
   slog << "======================== END OF REPORT ==========================" << std::endl;
   sslog << slog.str() << std::endl; //Write to SEGFAULT.log
   sslog.close(); //Close pointer to SEGFAULT.log
   std::cout << slog.str(); //Write to terminal.
   std::cout.flush(); //Clear output
   if(m)
     Log() << "Segmentation Fault in Module " << m->name << ", please review SEGFAULT.log";
   else
     Log(LOG_SILENT) << "\033[0mSegmentation Fault, Please read SEGFAULT.log";
 }else
   throw CoreException("Segmentation Fault, cannot write backtrace!");
#else
   Log(LOG_SILENT) << "Segmentation Fault";
   printf("\033[0mOh no! A Segmentation Fault has occurred!\n");
   printf("This system does not support backtracing, please use gdb or a similar debugger!\n");
   printf("Please follow these instructions on how to file a bug report of Azuru:\n");
   printf("1) type \"gdb navn\"\n2) type \"r -n --protocoldebug\"\n3) Cause the program to crash\n4) Type \"bt full\" and copy and paste the output to http://www.pastebin.com/\n5) File a bug report at http://bugs.Azuru.net/\n");
#endif
}
/** Terminal Signal Handler
 * \fn void sigact(int sig)
 * \brief A handler which handles what to do when we receive a signal.
 * \param int the signal we received.
 */
void sigact(int sig)
{
  Flux::string sigstr;
  switch(sig)
  {
    case SIGHUP:
      signal(sig, SIG_IGN);
      Rehash();
      break;
    case SIGPIPE:
      signal(sig, SIG_IGN);
      Log(LOG_DEBUG) << "Received SIGPIPE, ignoring..";
      break;
    case SIGUSR1:
      Log(LOG_CRITICAL) << "Ran out of memory! oh noes!";
      if(ircproto)
	ircproto->quit("Out of memory!");
      throw CoreException("Ran out of memory!");
    case SIGSEGV:
      #ifdef HAVE_SETJMP_H
      // If we crashed because of a Module, unload it and restore the stack
      if(LastRunModule)
      {
	HandleSegfault(LastRunModule);
	ModuleHandler::Unload(LastRunModule);
      	Log() << "Attempting to restore Stack to before Crash";
      	longjmp(sigbuf, -1);
      	Log() << "Finished restoring Stack";
      	break;
      }
      #endif
      // Oh no, core crash, report it and exit
      HandleSegfault(NULL);
      exit(sig);
      break;
    case SIGINT:
    case SIGKILL:
    case SIGTERM:
      signal(sig, SIG_IGN);
      signal(SIGHUP, SIG_IGN);
      sigstr = siginit(randint(1,20));
      quitmsg = "Received Signal: "+sigstr;
      // Quit right now if we've hit ctrl+c at our root check
      if(!Config)
	exit(EXIT_SUCCESS);
      else
	quitting = true;
      break;
    default:
      Log() << "Received weird signal from terminal. Sig Number: " << sig;
  }
}
/** Signal Initializer
 * \fn void InitSignals()
 * \brief Initialize the signals we catch in this void
 */
void InitSignals()
{
#ifndef _WIN32
  signal(SIGUSR1, sigact);
  signal(SIGPIPE, sigact);
  signal(SIGHUP, sigact);
  signal(SIGKILL, sigact);
#endif
  signal(SIGINT, sigact);
  signal(SIGTERM, sigact);
  signal(SIGSEGV, sigact);
}
