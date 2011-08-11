#ifndef CTCP_H
#define CTCP_H
#include "../includes.h"
#include "../flux_net_irc.hpp"
using namespace flux_net_irc;
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

class ctcp:module
{
public:
  ctcp(bool a):module("CTCP", a, PRIORITY_FIRST){ this->SetDesc("CTCP Handler"); }
  ModuleReturn run(SendMessage *Send, Flux::string rply, irc_string *reply)
  {
  if(reply->said("\001VERSION\001")){ // for CTCP VERSION reply
    cout << "\033[22;31mRecieved CTCP VERSION from "+unick+"\033[22;36m\r\n";
    struct utsname uts;
    if(uname(&uts) < 0)
	    //perror("uname() error");
	    throw CoreException("uname() Error");

    Send->notice(unick, "\001VERSION Navn Bot v%s %s %s\001",version.c_str(), uts.sysname, uts.machine);
    log("Recieved CTCP VERSION from %s", unick.c_str());
   }
  if(reply->said("\001TIME\001")){ // for CTCP TIME reply
    cout << "\033[22;31mRecieved CTCP TIME from "+unick+"\033[22;36m\r\n";
    Send->notice(unick, "\001TIME "+strip(os_time())+"\001");
    log("Recieved CTCP TIME from %s", unick.c_str());
  }
  if(reply->said("\001SOURCE\001")){
    cout << "\033[22;31mRecieved CTCP SOURCE from "+unick+"\033[22;36m\r\n";
    Send->notice(unick, "\001SOURCE https://gitorious.org/navn/navn\001");
    Send->notice(unick, "\001SOURCE http://flux-net.googlecode.com/svn/branches/Navn/\001");
    Send->notice(unick, "\1SOURCE git://gitorious.org/navn/navn.git\1");
    log("Recieved CTCP SOURCE from %s", unick.c_str());
  }
  return MOD_RUN;
  }
};
/**
 * @}
 */
#endif
