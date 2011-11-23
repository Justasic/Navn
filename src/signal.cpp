#include "includes.h"
#include "module.h"
#ifdef HAVE_BACKTRACE
#include <execinfo.h>
#endif

/** Random Quit message selector
 * \fn Flux::string siginit(int sigstring)
 * \brief this selects a quit message out of 20, this should be used with a random number between 1 and 20.
 * \param int the string number to return.
 */
Flux::string siginit(int sigstring)
{
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
/** Segmentation Fault Handler
 * \fn void HandleSegfault(module *m)
 * \brief A segfault handler to report what happened and where it happened.
 * \param module the module class in which the segfault happened include
 */
void HandleSegfault(module *m)
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
   slog << "Please report this bug to " << PACKAGE_BUGREPORT << " and submit a bug report." << std::endl;
   slog << "Please note that the Flux-Net developers may ask you to re-run this under gdb!" << std::endl;
   slog << "Time of crash: " << tbuf << std::endl;
   slog << "Navn version: " << VERSION_LONG << std::endl;
   slog << "System info: " << uts.sysname << " " << uts.nodename << " " <<  uts.release << " " << uts.machine << std::endl;
   slog << "System version: " << uts.version << std::endl;
   slog << "C++ Version: " << __VERSION__ " C++11 support: " << (havecxx11?"true":"false") << std::endl;
   slog << "Socket Buffer: " << LastBuf << std::endl;
   slog << "Location: " << segv_location << std::endl;
   if(m){
     slog << "Module: " << m->name << std::endl;
     slog << "Module Version: " << m->GetVersion() << std::endl;
     slog << "Module Author: " << m->GetAuthor() << std::endl;
   }
   for(Flux::insensitive_map<module*>::iterator it = Modules.begin(); it != Modules.end(); ++it)
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
     Log() << "Segmentation Fault in module " << m->name << ", please review SEGFAULT.log";
   else
     Log(LOG_SILENT) << "\033[0mSegmentation Fault, Please read SEGFAULT.log";
 }else
   throw CoreException("Segmentation Fault, cannot write backtrace!");
#else
   Log(LOG_SILENT) << "Segmentation Fault";
   printf("\033[0mOh no! A Segmentation Fault has occured!\n");
   printf("This system does not support backtracing, please use gdb or a similar debugger!\n");
   printf("Please follow these instructions on how to file a bug report of Flux-Net:\n");
   printf("1) type \"gdb navn\"\n2) type \"r -n --protocoldebug\"\n3) Cause the program to crash\n4) Type \"bt full\" and copy and paste the output to http://www.pastebin.com/\n5) File a bug report at http://flux-net.net/bugs/\n");
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
  switch(sig){
    case SIGHUP:
      signal(sig, SIG_IGN);
      Rehash();
      break;
    case SIGSEGV:
      /* this is where the module stack needs to be */
      #ifdef HAVE_SETJMP_H
      if(LastRunModule){
	HandleSegfault(LastRunModule);
	ModuleHandler::Unload(LastRunModule);
      	Log() << "Attempting to restore Stack to before Crash";
      	longjmp(sigbuf, -1);
      	Log() << "Finished restoring Stack";
      	break;
      }
      #endif
      HandleSegfault(NULL);
      exit(sig);
      break;
    case SIGINT:
    case SIGKILL:
    case SIGTERM:
      signal(sig, SIG_IGN);
      signal(SIGHUP, SIG_IGN);
      sigstr = siginit(randint(1,20));
      quitmsg = "Recieved Signal: "+sigstr;
      if(ircproto)
	ircproto->quit(quitmsg);
      quitting = true;
      break;
    default:
      Log() << "Recieved weird signal from terminal. Sig Number: " << sig;
  }
}
/** Signal Initializer
 * \fn void InitSignals()
 * \brief Initialize the signals we catch in this void
 */
void InitSignals()
{
  signal(SIGTERM, sigact);
  signal(SIGINT, sigact);
  signal(SIGHUP, sigact);
  signal(SIGSEGV, sigact);
}