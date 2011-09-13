/* All code is licensed under GNU General Public License GPL v3 (http://www.gnu.org/licenses/gpl.html) */
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
  void run(CommandSource &source, const std::vector<Flux::string> &params){
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
    Flux::string cmd = params.empty()?"":params[0];
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
    if(cmd == "!finduser"){
     Flux::string username = params.size() == 2?params[1]:"";
     if(username.empty()){
       source.Reply("Syntax: \2!finduser \37user\15");
       return MOD_STOP;
     }
     User *u2 = finduser(username);
     if(!u2)
       source.Reply("User \2%s\2 does not exist.", username.c_str());
     else
       source.Reply("User: %s", u2->nick.c_str());
      
    }
    if(irc_string::said(source.message, "enc test")){
     source.Reply("test passed"); 
    }
   return MOD_RUN; 
  }
};
//MODULE_HOOK(dummy)
/**
 * @}
 */

#endif
