/* All code is licensed under GNU General Public License GPL v3 (http://www.gnu.org/licenses/gpl.html) */
#include "flux_net_irc.hpp"

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
  weather():module("Weather", PRIORITY_DONTCARE){ 
    this->SetAuthor("Lordofsraam");
    this->SetVersion(VERSION);
    ModuleHandler::Attach(I_OnPrivmsg, this);
  }
  void OnPrivmsg(User *u, Channel *c, std::vector<Flux::string> &params){
    Flux::string cmd = params.empty()?"":params[0];
    
    Flux::string msg;
    for(unsigned i=0; i < params.size(); ++i){
      msg += params[i];
      msg.AddSpace();
    }
    
    if(cmd.equals_ci("!weather")){
      if(params.size() < 2){
	u->SendMessage("Syntax: \2!weather \037area\037\2");
	return;
      }
      
      Flux::string area = params[params.size() - 1], tmpfile = TempFile(binary_dir+"/runtime/navn_xml.tmp.XXXXXX"), wget;
      if(tmpfile.empty()){
	log(LOG_NORMAL, "Failed to get a temp file.");
	return;
      }
      area.trim();
      
      if(area.is_number_only())
	wget = "wget -q -O "+tmpfile+" - http://www.google.com/ig/api?weather="+area;
      else
	wget = "wget -q -O "+tmpfile+" - http://www.google.com/ig/api?weather="+urlify(removeCommand("!weather",msg));
      system(wget.c_str());
      
      if(!irc_string::said(xmlToString(tmpfile),"problem_cause")){
	Flux::string ff = xmlToString(tmpfile);
	ff.trim();
	if(ff.empty()){
	  c->SendMessage("Could not download/read %s", tmpfile.c_str());
	  log(LOG_NORMAL, "%s attempted to use !weather but downloading/reading the file '%s' failed.", tmpfile.c_str());
	  return;
	}
	Flux::string loc = findInXML("city","data",ff);
	Flux::string cond = findInXML("condition","data",ff);
	Flux::string tempf = findInXML("temp_f","data",ff);
	Flux::string tempc = findInXML("temp_c","data",ff);
	Delete(tmpfile.c_str());
	loc.trim();
	c->SendMessage("The current condition in %s is %s with a temperature of %s °F %s °C", loc.c_str(), cond.c_str(), tempf.c_str(), tempc.c_str());
	log(LOG_NORMAL, "%s used !weather to get weather for area '%s'", u->nick.c_str(), area.c_str());
      }
    }
  }
};

/**
 * @}
 */
MODULE_HOOK(weather)
