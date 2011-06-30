#ifndef WORLD_CLOCK_H
#define WORLD_CLOCK_H
#include "../includes.h"
#include "../flux_net_irc.hpp"
#define MST (-6)
#define UTC (0)
#define CCT (+8)
#define EST (-4)
#define PST (-7)
#define AUS (+10)
using namespace std;
using namespace flux_net_irc;

/**
 * \file world_clock.h Header file holding the \a world_clock function.
 * \author Lordofsraam. Polished by Justasic.
 */

/**
 * \defgroup world_clockM World Clock Module
 * This is the module for the world clock function.
 * For a better description see the function description.
 * \section commands Commands associated with this module.
 * \subsection time !time
 * Say \a !time to see the current time, the GMT, and the times in New York, California, Beijing, and Sydney.
 * @{
 */

/**
 * \fn void world_clock(Socket &sock, irc_string *reply)
 * \brief Tells the world time.
 * Uses C++ time functions to tell the time around the world.
 */
void world_clock(Socket &sock, irc_string *reply){
  if (reply->said("!time") && in_channel){
    time_t rawtime;
    tm * ptm;
    time ( &rawtime );
    ptm = gmtime ( &rawtime );
    int minutes = (ptm->tm_min);
    int seconds = (ptm->tm_sec);
    sock << privmsg(chan, "Current time around the World:");
    sock << privmsg(chan, "GMT == %2d:%02d:%02d", (ptm->tm_hour+UTC)%24, minutes, seconds);
    sock << privmsg(chan, "New York (USA) == %2d:%02d:%02d", (ptm->tm_hour+EST)%24, minutes, seconds);
    sock << privmsg(chan, "California (USA) == %2d:%02d:%02d", (ptm->tm_hour+PST)%24, minutes, seconds);
    sock << privmsg(chan, "Beijing (China) == %2d:%02d:%02d", (ptm->tm_hour+CCT)%24, minutes, seconds);
    sock << privmsg(chan, "Sydney (Australia) == %2d:%02d:%02d", (ptm->tm_hour+AUS)%24, minutes, seconds);	
    char buf[100];
    ptm = localtime(&rawtime);
    strftime(buf,100,"Navn's Time: %Z %c",ptm);
    sock << privmsg(chan, buf);
    log("%s requested !time command in %s", unick.c_str(), chan.c_str());
  }

}
/**
 * @}
 */
#endif
