#ifndef DUMMY_H
#define DUMMY_H

/**
 * \example dummy.h
 * This is an example module showing a simple if-said-that-reply-this situation.
 * \file dummy.h Header file holding the \a dummy function.
 * \author Lordofsraam
 */

/**
 * \defgroup dummyM Dummy (Example) Module
 * This is a template or example module.
 * \section commands Commands associated with this module.
 * \subsection test testing testing
 * Say \a testing \a testing to see if the bot replies with \a I \a hear \a you!
 * @{
 */

/**
 * \fn class dummy(bool a):module("Dummy", a, PRIORITY_LAST){ this->SetDesc("Example Dummy module"); }
 * \brief Replies to a test
 * We will try to put as many examples here in the future as we can.
 */
/*class CommandDummy : public Command
{
public: 
  CommandDummy() : Command("test", 0,0)
  {
   this->SetDesc("Test command for new command class");
   this->SetSyntax("\37test\37");
  }
  void Execute(CommandSource &source, const std::vector<Flux::string> &params){
   source.Reply("YAAY!");
   return;
  }
};*/

/*class Bacon:public Thread
{
public:
   void ToRun()
  {
    int i = 0;
    while(true)
    {
      i++;
    }
  }
};*/

class dummy : public module
{
//CommandDummy cmddummy;
public:
  dummy(bool a):module("Dummy", a, PRIORITY_LAST)
  { 
    this->SetDesc("Example Dummy module"); 
    //this->AddCommand(&cmddummy);
  }
  
  ModuleReturn run(CommandSource &source, std::vector<Flux::string> &params)
  {
    User *u = source.u;
    Channel *c = source.c;
    if(!u)
    {
      return MOD_STOP;
    }
    if(source.message == "usertest")
    {
     source.Reply("TEST!");
    }
    if(source.message == "chantest")
    {
     c->SendMessage("TADA!");
    }
    if (source.message == "testing testing")
    {
      //Send->privmsg(chan,"I hear you!\nYES I DO!");
      source.Reply("I hear you!\nYES I DO!");
    }
    if(source.message == "!startthread")
    {
      //Bacon strips;
      //ThreadHandler::RunThread(strips);
    }
   return MOD_RUN; 
  }
};
//MODULE_HOOK(dummy)
/**
 * @}
 */

#endif
