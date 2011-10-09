/* All code is licensed under GNU General Public License GPL v3 (http://www.gnu.org/licenses/gpl.html) */
#include "flux_net_irc.hpp"
/**
 * \file about_me.h Header file holding the \a about_me function.
 * \author Justasic. Polished by Lordofsraam.
 */

/**
 * \defgroup about_meM About Me Module
 * This is the module for the about me function. \n
 * This module was originally a test/example module that grew. \n
 * The module just tells you about you and your hostname in IRC (nickname, channel it was said in, hostname, etc.) \n
 * For a better description see the function description.
 * \section commands Commands associated with this module.
 * \subsection about_me about me
 * Say \a about \a me to see your various host information that navn can see and decode.
 * \subsection decodehost !decodehost
 * Say \a !decodehost with a hostname and navn will decode it into ident, nickname, hostname, etc.
 * @{
 */
class CommandDecodeHost : public Command
{
public:
  CommandDecodeHost():Command("!DECODEHOST", 1, 1)
  {
   this->SetDesc("Decodes a hostname");
   this->SetSyntax("\37fullhost\37");
  }
  void Run(CommandSource &source, const std::vector<Flux::string> &params)
  {
    Channel *c = source.c;
    Flux::string host = params[1];
    IsoHost* Host = new IsoHost(host);
    c->SendMessage("Nick: %s", Host->nick.c_str());
    c->SendMessage("User: %s", Host->user.c_str());
    c->SendMessage("Host: %s", Host->host.c_str());
    c->SendMessage("Raw: %s", Host->raw.c_str());
    delete Host;
  }
};

/**
 * \fn class about_me(bool a):module("About Me", a, PRIORITY_DONTCARE){ this->SetDesc("Returns the information about yourself"); }
 * \brief Tells you about youself (ie. Host, ident, etc.).
 * Uses irc_string and IsoHost to decode the message you said.
 */
class about_me : public module
{
  CommandDecodeHost host;
public:
  about_me(const Flux::string &Name):module(Name){ 
    this->SetAuthor("Justasic");
    this->SetVersion(VERSION);
    this->SetPriority(PRIORITY_LAST);
    this->AddChanCommand(&host);
    ModuleHandler::Attach(I_OnPrivmsg, this);
  }
  
  void OnPrivmsg(User *u, Channel *c, const std::vector<Flux::string> &params)
  {
    Flux::string msg;
    for(unsigned i=0; i < params.size(); ++i)
      msg += params[i]+' ';
    
    msg.trim();
    if(msg.equals_ci("about me")){
	  //u->SendMessage("Raw: %s", source.raw.c_str()); This is broke till Justasic can find a fix.
	  u->SendMessage("Message: %s", msg.c_str());
	  u->SendMessage("Nickname: %s", u->nick.c_str());
	  u->SendMessage("Ident: %s", u->ident.c_str());
	  u->SendMessage("Host: %s", u->host.c_str());
	  u->SendMessage("Channel: %s", c->name.c_str());
	  u->SendMessage("Fullhost: %s", u->fullhost.c_str());
	  Log() << u->nick << " requested information about themself";
    }
 }
};
/**
 * @}
 */
MODULE_HOOK(about_me)