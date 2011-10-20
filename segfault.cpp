#include "includes.h"
#include "module.h"
#ifdef HAVE_BACKTRACE
#include <execinfo.h>
#endif

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
   std::cout << slog.str() << std::endl; //Write to terminal.
   if(m)
      Log() << "Segmentation Fault in module " << m->name << " please review SEGFAULT.log";
   else
      Log(LOG_SILENT) << "\033[22;37mSegmentation Fault, Please read SEGFAULT.log";
 }else
   throw CoreException("Segmentation Fault, cannot write backtrace!");
#else
   Log(LOG_SILENT) << "Segmentation Fault";
   printf("\033[22;37mOh no! A Segmentation Fault has occured!\n");
   printf("This system does not support backtracing, please use gdb or a similar debugger!\n");
   printf("Please follow these instructions on how to file a bug report of Flux-Net:\n");
   printf("1) type \"gdb navn\"\n2) type \"r -n --protocoldebug\"\n3) Cause the program to crash\n4) Type \"bt full\" and copy and paste the output to http://www.pastebin.com/\n5) File a bug report at http://flux-net.net/bugs/\n");
#endif
}
void handle_sigsegv(int)
{
  static int running = 0;
  if((running = 1)){ //Stop segv bombs
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
  HandleSegfault(NULL);
  exit(EXIT_FAILURE);
}