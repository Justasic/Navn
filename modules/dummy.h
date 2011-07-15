#ifndef DUMMY_H
#define DUMMY_H
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
class dummy : module{

public:
  dummy(string n, bool a):module(n,a){}
  
  void run(string rply, irc_string *reply)
  {
    
    if (reply->said("testing testing"))
	{
	  Send::Message::priv(reply->channel,"I hear you!");
    }
    
  }
};

/**
 * @}
 */

#endif
