/* All code is licensed under GNU General Public License GPL v3 (http://www.gnu.org/licenses/gpl.html) */
#ifndef CTCP_H
#define CTCP_H

/**
 * \file ctcp.h Header file holding the \a ctcp functions.
 * \author Lordofsraam. Polished by Justasic.
 */

/**
 * \defgroup ctcpM CTCP Module
 * This is the module for the CTCP functions.
 * It houses the CTCP replies when someone versions it or asks for the time
 * For a better description see the function description.
 * \section commands Commands associated with this module.
 * \subsection none none
 * This module has no user interactive function
 * @{
 */

/**
 * \fn class ctcp(bool a):module("CTCP", a, PRIORITY_FIRST){ this->SetDesc("CTCP Handler"); }
 * \brief Replies to CTCPs
 * Replies to CTCP requests in IRC
 */

class ctcp:public module
{
public:
  ctcp(bool a):module("CTCP", a, PRIORITY_FIRST){ this->SetDesc("CTCP Handler"); }
  ModuleReturn run(CommandSource &source, std::vector<Flux::string> &params)
  {
    Flux::string cmd = params.empty()?"":params[0], message = source.message;
    if(message[0] == '\1' && message[message.length() -1] == '\1'){
      log(LOG_NORMAL, "\033[22;31mRecieved CTCP %s from %s\033[22;36m", Flux::Sanitize(cmd).c_str(), source.raw_source.c_str()); 
    }
    if(cmd == "\001VERSION\001"){ // for CTCP VERSION reply
      struct utsname uts;
      if(uname(&uts) < 0)
	      throw CoreException("uname() Error");

	Send->notice(source.raw_source, "\001VERSION Navn-%s %s %s\001",VERSION_LONG.c_str(), uts.sysname, uts.machine);
    }
    if(cmd == "\001TIME\001"){ // for CTCP TIME reply
	Send->notice(source.raw_source,"\001TIME %s\001", strip(os_time()).c_str());
    }
    if(cmd == "\001SOURCE\001"){
      source.Reply("\001SOURCE https://gitorious.org/navn/navn\001");
      source.Reply("\001SOURCE http://flux-net.googlecode.com/svn/branches/Navn/\001");
      source.Reply("\1SOURCE git://gitorious.org/navn/navn.git\1");
    }
    return MOD_RUN;
  }
};
/**
 * @}
 */
#endif
