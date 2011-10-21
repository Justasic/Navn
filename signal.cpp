#include "includes.h"
#include "module.h"
#ifdef HAVE_BACKTRACE
#include <execinfo.h>
#endif

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

void HandleSegfault(module *m)
{
#ifdef HAVE_BACKTRACE
 void *array[10];
 char **strings;
 char tbuf[256];
 Flux::string mbuf;
 size_t size;
 time_t now = time(NULL);
 
 size = backtrace(array, 10);
 if(IsFile("SEGFAULT.log"))
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
   slog << '(' << tbuf << ')' << std::endl;
   slog << "Navn version: " << VERSION_LONG << std::endl;
   slog << "System info: " << uts.sysname << " " << uts.nodename << " " <<  uts.release << " " << uts.machine << std::endl;
   slog << "System version: " << uts.version << std::endl;
   if(m){
     slog << "Module: " << m->name << std::endl;
     slog << "Module Version: " << m->GetVersion() << std::endl;
     slog << "Module Author: " << m->GetAuthor() << std::endl;
   }
   for(Flux::insensitive_map<module*>::iterator it = Modules.begin(); it != Modules.end(); ++it)
     mbuf += it->second->name+" ";
   mbuf.trim();
   slog << "Modules Loaded: " << (mbuf.empty()?"None":mbuf) << std::endl;
   slog << "Location: " << segv_location << std::endl;
   //slog << "recbuf: " << sock->getlastbuf() << std::endl; //Fucking C++ cant dereference a Flux::string, so this is broke till it can do that.
   strings = backtrace_symbols(array, size);
   for(unsigned i=1; i < size; i++)
     slog << "BackTrace(" << (i - 1) << "): " << strings[i] << std::endl;
   free(strings);
   slog << "======================== END OF REPORT ==========================" << std::endl;
   sslog << slog.str() << std::endl; //Write to SEGFAULT.log
   sslog.close();
   Log(LOG_TERMINAL) << slog.str(); //Write to terminal.
   if(m)
      Log() << "Segmentation Fault in module " << m->name << " please review SEGFAULT.log";
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
 * Come here for weird signals
 * @param sigact(integer)
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
      static int running = 0;
      if((running = 1)){ //Stop segv bombs
	HandleSegfault(NULL);
	exit(EXIT_FAILURE);
      }else{
	running = 1;
      }
      /* this is where the module stack needs to be 
      * 
      * #ifdef HAVE_SETJMP_H
      * if(somethingHappened){
      * 	module *m = FindLastModule;
      * 	HandleSegfault(m);
      * 	ModuleHandler::Unload(m);
      * 	Log() << "Attempting to restore Stack to before Crash";
      * 	longjmp(segbuf, -1);
      * 	Log() << "Finished restoring Stack";
      * 	return;
      *  }
      * #endif
      */
      //HandleSegfault(NULL);
      exit(EXIT_FAILURE);
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
      quitting = true;
      break;
    default:
      Log() << "Recieved weird signal from terminal. Sig Number: " << sig;
  }
}