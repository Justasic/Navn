#include "flux_net_irc.hpp"
#include <ios>
#include <istream>

void ProcessInput(const Flux::string &str)
{
  std::vector<Flux::string> params = StringVector(str, ' ');
  if(params.empty())
    return;
  
  if(params[0].equals_ci("QUIT")){
	quitting = true;
	send_cmd("%s\n", str.c_str());
  }
  else if(str.find_first_of_ci("MSG") == Flux::string::npos)
    send_cmd("PRIVMSG %s\n", str.c_str());
  else if(str.find_first_of_ci("NICK") == Flux::string::npos)
    send_cmd("NICK %s\n", str.c_str());
  else
    send_cmd("%s\n", str.c_str());
}

/** \class InputThread
 * This thread allows for user input to be possible, this is activated when the nofork option is specified.
 */
class InputThread : public Thread
{
public:
  bool exiting;
  InputThread():Thread() {}
  ~InputThread() { Log(LOG_TERMINAL) << "Input Thread Exiting."; exiting = true; }
  void ToRun()
  {
    exiting = false;
    std::string buf;
    while(!exiting)
    {
      printf("Top of Input Loop\n");
      std::getline(std::cin, buf);
      if(!buf.empty())
	ProcessInput(buf);
    }
    SetExitState();
  }
};

class ModTerminalInput : public module
{
  Thread *t;
public:
  ModTerminalInput(const Flux::string &Name):module(Name)
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