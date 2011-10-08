/* All code is licensed under GNU General Public License GPL v3 (http://www.gnu.org/licenses/gpl.html) */
#include "flux_net_irc.hpp"
/**
 * \file help.cpp Module file holding the \a help function.
 * \author Lordofsraam. Polished by Justasic.
 */

/**
 * \defgroup helpM Help Module
 * This is the module for the help_m function.
 * Returns help information about controlling Navn.
 * \section commands Commands associated with this module.
 * \subsection help Private message: help
 * Send a private message saying \a help to get information.
 * @{
 */

/**
 * \fn void help_m(Socket &sock, irc_string *reply)
 * \brief Returns help info on Navn.
 * Tells you most of the commands you can use on Navn.
 */
class CommandHelp : public Command
{
public:
  CommandHelp():Command("HELP")
  {
   this->SetDesc("Displays help messages");
  }
  void Run(CommandSource &source, const std::vector<Flux::string> &params)
  {
    int c=0;
   for(CommandMap::iterator it = Commandsmap.begin(), it_end = Commandsmap.end(); it != it_end; ++it)
   {
     if(it->second != NULL)
     {
	source.Reply("\2%-16s\2 %s", it->second->name.c_str(), it->second->GetDesc().c_str());
	++c;
     }
   }
   source.Reply("Total of \2%i\2 commands", c);
   log(LOG_NORMAL, "%s used help command", source.u->nick.c_str());
  }
};

class CommandCHelp : public Command
{
public:
  CommandCHelp():Command("!HELP")
  {
   this->SetDesc("Displays Channel help messages");
  }
  void Run(CommandSource &source, const std::vector<Flux::string> &params)
  {
    Flux::string cmds;
   for(CommandMap::iterator it = ChanCommandMap.begin(), it_end = ChanCommandMap.end(); it != it_end; ++it)
   {
     if(it->second != NULL)
     {
	cmds += it->second->name+" ";
     }
   }
   cmds.trim();
   cmds.tolower();
   source.c->SendMessage("Local %s Commands:", source.c->name.c_str());
   source.c->SendMessage(cmds);
   log(LOG_NORMAL, "%s used help command", source.u->nick.c_str());
  }
};
class help_m:public module
{
  CommandHelp help;
  CommandCHelp chelp;
public:
  help_m(const Flux::string &Name):module(Name){ 
    this->AddCommand(&help);
    this->AddChanCommand(&chelp);
    this->SetVersion(VERSION);
    this->SetPriority(PRIORITY_FIRST);
    this->SetAuthor("Justasic");
  }
};
MODULE_HOOK(help_m)
/**
 * @}
 */