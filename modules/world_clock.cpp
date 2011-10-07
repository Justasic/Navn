/* All code is licensed under GNU General Public License GPL v3 (http://www.gnu.org/licenses/gpl.html) */
#define MST (-6)
#define UTC (0)
#define CCT (+8)
#define EST (-4)
#define PST (-7)
#define AUS (+10)
#include "flux_net_irc.hpp"

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
class CommandCWClock: public Command
{
public:
  CommandCWClock():Command("!TIME", 0, 2)
  {
   this->SetDesc("Shows the time in the channel");
   this->SetSyntax("\37location\37");
  }
  void Run(CommandSource &source, const std::vector<Flux::string> &params)
  {
    Channel *c = source.c;
    Flux::string location = params.empty()?"":params[1];
    location.trim();
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
      log(LOG_NORMAL, "%s requested !time command in %s", source.u->nick.c_str(), c->name.c_str());
      return;
    }else{
      Flux::string wget, tmpfile = TempFile(Config->Binary_Dir+"/runtime/navn_xml.tmp.XXXXXX");
      	if(location.is_number_only())
	  wget = "wget -q -O "+tmpfile+" - http://www.google.com/ig/api?weather="+location;
	else
	  wget = "wget -q -O "+tmpfile+" - http://www.google.com/ig/api?weather="+urlify(removeCommand(this->name.ci_str(), source.message.ci_str()));
	system(wget.c_str());
	
	if(!xmlToString(tmpfile).search("problem_cause")){
	  Flux::string ff = xmlToString(tmpfile);
	  ff.trim();
	  if(ff.empty()){
	   c->SendMessage("Could not download/read %s", tmpfile.c_str());
	   log(LOG_NORMAL, "%s attempted to use !time but downloading/reading the file '%s' failed.", tmpfile.c_str());
	   return;
	  }
	  Flux::string loc = findInXML("city","data",ff);
	  Flux::string time = findInXML("current_date_time","data",ff);
	  c->SendMessage("The current time in %s is %s", loc.c_str(), time.c_str());
	  Delete(tmpfile.c_str());
	  log(LOG_NORMAL, "%s used !time to get time for %s", source.u->nick.c_str(), location.c_str());
	  }
	}
  }
};
class world_clock:public module{
  CommandCWClock clock;
public:
  world_clock():module("World Clock", PRIORITY_DONTCARE){ 
    this->SetAuthor("Lordofsraam");
    this->SetVersion(VERSION);
    this->AddChanCommand(&clock);
  }
};
MODULE_HOOK(world_clock)
/**
 * @}
 */

