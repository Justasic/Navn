#include "flux_net_irc.hpp"
#include <ios>
#include <istream>

/** \class InputThread
 * This thread allows for user input to be possible, this is activated when the nofork option is specified.
 */
class InputThread : public Thread
{
public:
  bool exiting;
  InputThread():Thread() {}
  ~InputThread() { log(LOG_TERMINAL, "Input Thread Exiting."); exiting = true; }
  void ToRun()
  {
    exiting = false;
    std::string buf;
    while(!exiting)
    {
      printf("Top of Input Loop\n");
      std::getline(std::cin, buf);
      if(Flux::string(buf).find_first_of_ci("QUIT") == Flux::string::npos)
	quitting = true;
      send_cmd("%s\n", buf.c_str());
    }
    SetExitState();
  }
};

class ModTerminalInput : public module
{
  Thread *t;
public:
  ModTerminalInput():module("TerminalInput", PRIORITY_DONTCARE)
  {
    this->SetVersion(VERSION);
    this->SetAuthor("Justasic");
    ModuleHandler::Attach(I_OnStart, this);
  }
  ~ModTerminalInput() { if(t) delete t; }
  void OnStart(int, char**)
  {
    t = new InputThread();
    t->Start();
  }
};

MODULE_HOOK(ModTerminalInput)