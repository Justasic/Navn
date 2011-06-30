#ifndef DUMMY_H
#define DUMMY_H
#include "../includes.h"
#include "../flux_net_irc.hpp"
using namespace std;
using namespace flux_net_irc;
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
 * Say "testing testing" to see the current time, the GMT, and the times in New York, California, Beijing, and Sydney.
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
