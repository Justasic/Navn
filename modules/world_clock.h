/* All code is licensed under GNU General Public License GPL v3 (http://www.gnu.org/licenses/gpl.html) */
#ifndef WORLD_CLOCK_H
#define WORLD_CLOCK_H
#include "includes.h"
#include "../flux_net_irc.hpp"
#define MST (-6)
#define UTC (0)
#define CCT (+8)
#define EST (-4)
#define PST (-7)
#define AUS (+10)

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
 * \fn class world_clock(bool a):module("World Clock", a, PRIORITY_DONTCARE){ this->SetDesc("Shows time for your area or for the preset ones"); }
 * \brief Tells the world time.
 * Uses C++ time functions to tell the time around the world.
 */

class world_clock:public module{
public:
  world_clock(bool a):module("World Clock", a, PRIORITY_DONTCARE){ this->SetDesc("Shows time for your area or for the preset ones"); }
  ModuleReturn run(CommandSource &source, std::vector<Flux::string> &params){
  Flux::string cmd = params.empty()?"":params[0];
  User *u = source.u;
  Channel *c = source.c;
  if (cmd == "!time"){
    Flux::string location;
    if(params.size() > 2)
      location = params[params.size() -1];
    else
      location = params.size() == 2?params[1]:"";
    if(location.empty()){
      time_t rawtime;
      tm * ptm;
      time(&rawtime);
      ptm = gmtime(&rawtime);
      int minutes = (ptm->tm_min);
      int seconds = (ptm->tm_sec);
      c->SendMessage("Current time around the World:");
      c->SendMessage("GMT == %2d:%02d:%02d", (ptm->tm_hour+UTC)%24, minutes, seconds);
      c->SendMessage("New York (USA) == %2d:%02d:%02d", (ptm->tm_hour+EST)%24, minutes, seconds);
      c->SendMessage("California (USA) == %2d:%02d:%02d", (ptm->tm_hour+PST)%24, minutes, seconds);
      c->SendMessage("Beijing (China) == %2d:%02d:%02d", (ptm->tm_hour+CCT)%24, minutes, seconds);
      c->SendMessage("Sydney (Australia) == %2d:%02d:%02d", (ptm->tm_hour+AUS)%24, minutes, seconds);	
      char buf[100];
      ptm = localtime(&rawtime);
      strftime(buf,100,"Navn's Time: %Z %c",ptm);
      c->SendMessage(buf);
      log("%s requested !time command in %s", u->nick.c_str(), c->name.c_str());
      return MOD_RUN;
    }else{
      Flux::string wget, filename;
      filename = "temp.xml";
      	if(location.is_number_only())
	  wget = "wget -q -O "+filename+" - http://www.google.com/ig/api?weather="+location;
	else
	  wget = "wget -q -O "+filename+" - http://www.google.com/ig/api?weather="+urlify(removeCommand("!time", source.raw));
	system(wget.c_str());
	if(!irc_string::said(xmlToString(filename),"problem_cause")){
	  Flux::string ff = xmlToString(filename);
	  ff.trim();
	  if(ff.empty()){
	   c->SendMessage("Could not download/read %s", filename.c_str());
	   log("%s attempted to use !time but downloading/reading the file '%s' failed.", filename.c_str());
	   return MOD_STOP;
	  }
	  Flux::string loc = findInXML("city","data",ff);
	  Flux::string time = findInXML("current_date_time","data",ff);
	  c->SendMessage("The current time in %s is %s", loc.c_str(), time.c_str());
	  remove(filename.c_str());
	  log("%s used !time to get time for %s", u->nick.c_str(), location.c_str());
	  }
	}
      }
      return MOD_RUN;
    }
};      //lol bracket stairs
/**
 * @}
 */
#endif
