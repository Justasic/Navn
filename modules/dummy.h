#ifndef DUMMY_H
#define DUMMY_H
#include "../includes.h"
#include "../flux_net_irc.hpp"
using namespace std;
using namespace flux_net_irc;
/**
 * \example dummy.h 
 * This is an example module showing a simple if-said-than-reply-this situation.
 * \file dummy.h Header file holding the \a dummy function.
 * \author Lordofsraam
 */

/**
 * \defgroup dummykM Dummy (Example) Module
 * This is a template or example module.
 * \section commands Commands associated with this module.
 * \subsection test testing testing
 * Say \a testing \a testing to see a test message that navn replies with.
 * @{
 */

/**
 * \fn void dummy(Socket &sock, irc_string *reply)
 * \brief Replies to a test
 * We will try to put as many examples here in the future as we can.
 */
void dummy(Socket &sock, irc_string *reply){

  if (reply->said("testing testing")){
    sock << privmsg(reply->channel,"I hear you!");
  }

}
/**
 * @}
 */
#endif
