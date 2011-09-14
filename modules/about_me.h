/* All code is licensed under GNU General Public License GPL v3 (http://www.gnu.org/licenses/gpl.html) */
#ifndef ABOUT_ME_H
#define ABOUT_ME_H

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

/**
 * \fn class about_me(bool a):module("About Me", a, PRIORITY_DONTCARE){ this->SetDesc("Returns the information about yourself"); }
 * \brief Tells you about youself (ie. Host, ident, etc.).
 * Uses irc_string and IsoHost to decode the message you said.
 */
class about_me : public module
{
  
public:
  about_me(bool a):module("About Me", a, PRIORITY_DONTCARE){ this->SetDesc("Returns the information about yourself"); }
  
  ModuleReturn run(CommandSource &source, std::vector<Flux::string> &params)
  {
    Flux::string cmd = params.empty()?"":params[0];
    User *u = source.u;
    Channel *c = source.c;
  if(source.message == "about me"){
	source.Reply("Raw: %s", source.raw.c_str());
	source.Reply("message: %s", source.message.c_str());
	source.Reply("Nickname: %s", u->nick.c_str());
	source.Reply("Ident: %s", u->ident.c_str());
	source.Reply("Host: %s", u->host.c_str());
	source.Reply("Channel: %s", c->name.c_str());
	source.Reply("Fullhost: %s", u->fullhost.c_str());
	log(LOG_NORMAL, "%s requested information about themself.", u->nick.c_str());
  }
  if(cmd.equals_ci("!decodehost")){
    if(params.size() != 2){
     source.Reply("Syntax: \2!decodehost \37hostname\15"); 
     return MOD_STOP;
    }
	Flux::string host = params[1];
	IsoHost* Host = new IsoHost(host);
	c->SendMessage("Nick: %s", Host->nick.c_str());
	c->SendMessage("User: %s", Host->user.c_str());
	c->SendMessage("Host: %s", Host->host.c_str());
	c->SendMessage("Raw: %s", Host->raw.c_str());
	delete Host;
  }
  return MOD_RUN;
 }
};
/**
 * @}
 */
#endif