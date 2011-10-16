#include "includes.h"
#include "module.h"
#ifdef HAVE_BACKTRACE
#include <execinfo.h>
#endif

void handle_sigsegv(int)
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
 std::fstream slog("SEGFAULT.log", std::ifstream::out | std::ifstream::app);
 if(slog.is_open())
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
   for(Flux::insensitive_map<module*>::iterator it = Modules.begin(); it != Modules.end(); ++it)
     mbuf += it->second->name+" ";
   mbuf.trim();
   slog << "Modules Loaded: " << (mbuf.empty()?"None":mbuf) << std::endl;
   //slog << "recbuf: " << sock->getlastbuf() << std::endl; //Fucking C++ cant dereference a Flux::string, so this is broke till it can do that.
   strings = backtrace_symbols(array, size);
   for(unsigned i=1; i < size; i++)
     slog << "BackTrace(" << (i - 1) << "): " << strings[i] << std::endl;
   free(strings);
   slog << "======================== END OF REPORT ==========================" << std::endl;
   slog.close();
   printf("\033[22;37mSegmentation Fault, Please read SEGFAULT.log\n");
 }else
   throw CoreException("Segmentation Fault, cannot write backtrace!");
#else
   printf("\033[22;37mOh no! A Segmentation Fault has occured!\n");
   printf("This system does not support backtracing, please use gdb or a similar debugger!\n");
   printf("Please follow these instructions on how to file a bug report of Flux-Net:\n");
   printf("1) type \"gdb navn\"\n2) type \"r -n --protocoldebug\"\n3) Cause the program to crash\n4) Type \"bt full\" and copy and paste the output to http://www.pastebin.com/\n5) File a bug report at http://flux-net.net/bugs/\n");
#endif
   exit(1);
}