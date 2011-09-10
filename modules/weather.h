/* All code is licensed under GNU General Public License GPL v3 (http://www.gnu.org/licenses/gpl.html) */
#ifndef WEATHER_H
#define WEATHER_H
#include "includes.h"
#include "../flux_net_irc.hpp"

/**
 * \file weather.h Header file holding the \a weather function.
 * \author Lordofsraam (very proud of his xml pseudo-parsing). Polished by Justasic.
 */

/**
 * \defgroup weatherM Weather Module
 * This is the module for the weather function.
 * For a better description see the function description.
 * \section commands Commands associated with this module.
 * \subsection weather !weather
 * Follow \a !weather with either a U.S. postal code or a city name and country/state name.
 * @{
 */



/**
 * \fn weather(bool act):module("Weather", act, PRIORITY_DONTCARE){ this->SetDesc("Shows the weather for your location"); }
 * \brief Says the weather.
 * This is a good example of how to use the xml pseudo-parser we have in the \a flux_net_irc namespace.
 * If \a !weather is said then it takes whatever follows that and uses wget to get an xml file from 
 * the google weather api. From there it pseudo-parses the file using functions from \a flux_net_irc 
 * and throws the needed contents out of the main socket as a private message (using \a privmsg).
 */
class weather:public module{
public:
  weather(bool act):module("Weather", act, PRIORITY_DONTCARE){ this->SetDesc("Shows the weather for your location"); }
  ModuleReturn run(CommandSource &source, std::vector<Flux::string> &params){
    Flux::string cmd = params.empty()?"":params[0];
    User *u = source.u;
    if(cmd == "!weather"){
      if(params.size() < 2){
	source.Reply("Syntax: \2!weather \037area\037\2");
	return MOD_STOP;
      }
      Flux::string area = params[params.size() - 1];
      Flux::string filename = "temp.xml";
      Flux::string wget;
      area.trim();
      if(area.is_number_only())
	wget = "wget -q -O "+filename+" - http://www.google.com/ig/api?weather="+area;
      else
	wget = "wget -q -O "+filename+" - http://www.google.com/ig/api?weather="+urlify(removeCommand("!weather",source.raw));
      system(wget.c_str());
      if(!irc_string::said(xmlToString(filename),"problem_cause")){
	Flux::string ff = xmlToString(filename);
	ff.trim();
	if(ff.empty()){
	  source.c->SendMessage("Could not download/read %s", filename.c_str());
	  log("%s attempted to use !weather but downloading/reading the file '%s' failed.", filename.c_str());
	  return MOD_STOP;
	}
	Flux::string loc = findInXML("city","data",ff);
	Flux::string cond = findInXML("condition","data",ff);
	Flux::string tempf = findInXML("temp_f","data",ff);
	Flux::string tempc = findInXML("temp_c","data",ff);
	remove(filename.c_str());
	loc.trim();
	source.c->SendMessage("The current condition in %s is %s with a temperature of %s °F %s °C", loc.c_str(), cond.c_str(), tempf.c_str(), tempc.c_str());
	log("%s used !weather to get weather for area '%s'", u->nick.c_str(), area.c_str());
      }
    }
    return MOD_RUN;
  }
};

/**
 * @}
 */
#endif
