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
  CommandHelp():Command("HELP", 0, 0)
  {
   this->SetDesc("Displays help messages");
  }
  void Run(CommandSource &source, const std::vector<Flux::string> &params)
  {
    int c=0;
   for(CommandMap::iterator it = Commandsmap.begin(), it_end = Commandsmap.end(); it != it_end; ++it)
   {
     source.Reply("\2%-16s\2 %s", it->second->name.c_str(), it->second->GetDesc().c_str());
     ++c;
   }
   source.Reply("Total of %i commands", c);
   log(LOG_NORMAL, "%s used help command", source.u->nick.c_str());
  }
};
class help_m:public module
{
  CommandHelp help;
public:
  help_m():module("Help", PRIORITY_DONTCARE){ 
    this->AddCommand(&help);
    this->SetAuthor("Justasic");
  }
};
MODULE_HOOK(help_m)
/**
 * @}
 */