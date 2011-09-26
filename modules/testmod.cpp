#include "flux_net_irc.hpp"

/**
 * \example dummy.cpp
 * This is an example module showing a simple if-said-that-reply-this situation.
 * \file dummy.cpp Source file holding the \a dummy function.
 * \author Lordofsraam
 */

/**
 * \defgroup dummyM Dummy (Example) Module
 * This is a template or example module.
 * \section commands Commands associated with this module.
 * \subsection test testing testing
 * Say \a test to see if the bot replies :P
 * @{
 */

/**
 * \fn class commanddummy() : Command("TEST", 0, 0)
 * \brief Replies to a test
 * We will try to put as many examples here in the future as we can.
 */
class commanddummy : public Command
{
public:
  commanddummy() : Command("TEST", 0, 0)
  {
   this->SetDesc("Test for the modules");
   this->SetSyntax("\2TEST\2");
  }
  void Run(CommandSource &source, const std::vector<Flux::string> &params)
  {
    source.Reply("YAY!");
  }
  
};
class dummy : public module
{
  commanddummy cmddmy; //Declare our command
public:
  dummy():module("Dummy", PRIORITY_LAST)
  { 
    this->AddCommand(&cmddmy); //Add our command to teh bot
    this->SetAuthor("Lordofsraam"); //Set the author
    this->SetVersion(VERSION);
    
    Implementation i[] = { I_OnReload }; //Add that we have a module hook, this can be done in 2 ways
    ModuleHandler::Attach(i, this, sizeof(i) / sizeof(Implementation));
    /*or you can do the easy way
     * ModuleHandler::Attach(I_OnReload, this);
     */
  }
  void OnReload(bool)
  {
   log(LOG_TERMINAL, "RELOAD! YAAY!\n"); 
  }
};

MODULE_HOOK(dummy)
/**
 * @}
 */