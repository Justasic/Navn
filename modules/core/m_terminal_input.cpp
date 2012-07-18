/* Navn IRC bot -- Commandline interface
 *
 * (C) 2011-2012 Azuru
 * Contact us at Development@Azuru.net
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Anope by The Anope Team.
 */

#include "modules.h"

// Fake user so we can use any command loaded into Navn
class CliUser : public User
{
  inline bool IsEven(unsigned int num) { return !(num & 1); }

  // Correct color codes when sending to the terminal
  Flux::string FixColors(const Flux::string &string)
  {
    Flux::string actualstring = string;
    unsigned found_bold = 0;
    unsigned found_under = 0;
    for(unsigned i = 0; i < actualstring.size(); ++i)
    {
      char c = actualstring[i];

      if(c == '\002')
      {
	i++;
	if(IsEven(++found_bold))
	  actualstring.insert(i, "\033[0m"+Config->LogColor);
	else
	  actualstring.insert(i, "\033[1m");
      }

      if(c == '\037')
      {
	i++;
	if(IsEven(++found_under))
	  actualstring.insert(i, "\033[0m"+Config->LogColor);
	else
	  actualstring.insert(i, "\033[4m");
      }
    }
    return Flux::Sanitize(actualstring);
  }

public:
  CliUser() : User("Console", "Konsole", "magic.location", "Touch my body, feel me on the floor", "127.0.0.1")
  {
    // is there anything I am forgetting?
  }

  // We're the console, no Access Denied crap
  bool IsOwner()
  {
    return true;
  }

  // forward messages to the console, not IRC
  void SendMessage(const Flux::string &message)
  {
    Log(LOG_TERMINAL) << FixColors(message);
  }
};

static CliUser *DeathBlade;

// Parse the command entered.
void ProcessInput(const Flux::string &str)
{
  Flux::string Justasic = str;
  Justasic.trim();
  Flux::vector Lordofsraam = ParamitizeString(Justasic, ' ');
  Flux::string command = Lordofsraam.size() > 0 ? Lordofsraam[0] : "";
  command.trim();

  if(command[0] == '!')
  {
    DeathBlade->SendMessage("Commands cannot be prefixed with !");
    return;
  }

  CommandSource source;
  source.u = DeathBlade;
  source.c = NULL;
  source.params = Lordofsraam;
  source.raw = Justasic;

  // The core can handle it now!
  ProcessCommand(source, Lordofsraam, "Console", "PRIVMSG");
}

/** \class InputThread
 * This thread allows for user input to be possible, this is activated when the nofork option is specified.
 */
class InputThread : public Thread
{
public:
  bool exiting;
  InputThread():Thread(), exiting(false)
  {
    Log(LOG_THREAD) << "Input Thread Initializing.";
    this->Start();
  }

  ~InputThread()
  {
    Log(LOG_THREAD) << "Input Thread Exiting.";
    exiting = true;
    SetExitState();
  }

  void ToRun()
  {
    int nobomb = 0, last_run = time(NULL);
    while(!exiting)
    {
      if(nobomb++ >= 50)
	raise(SIGSEGV);
      if(last_run - time(NULL) > 5)
      {
	nobomb = 0;
	last_run = time(NULL);
      }
      Log(LOG_THREAD) << "Top of Input Loop";
      Flux::string buf;
      std::getline(std::cin, buf.std_str());
      buf.trim();

      if(!buf.empty())
	ProcessInput(buf);
      else
	DeathBlade->SendMessage("Please enter a command.");
    }
    SetExitState();
  }
};

// Module class
class ModTerminalInput : public Module
{
  Thread *t;
public:
  ModTerminalInput(const Flux::string &Name):Module(Name)
  {
    this->SetVersion(VERSION);
    this->SetAuthor("Justasic");
    if(nofork && InTerm())
    {
      DeathBlade = new CliUser();
      t = new InputThread();
    }
    else
      throw ModuleException("Cannot run m_terminal_input when fork'ed");
  }

  ~ModTerminalInput()
  {
    if(t)
      delete t;

    if(DeathBlade)
      delete DeathBlade;
  }
};

MODULE_HOOK(ModTerminalInput)
