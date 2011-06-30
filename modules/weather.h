#ifndef WEATHER_H
#define WEATHER_H
#include "../includes.h"
#include "../flux_net_irc.hpp"
using namespace std;
using namespace flux_net_irc;

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

char * pEnd;

/**
 * \fn void weather(Socket &sock, string rply)
 * \brief Says the weather.
 * This is a good example of how to use the xml pseudo-parser we have in the \a flux_net_irc namespace.
 * If \a !weather is said then it takes whatever follows that and uses wget to get an xml file from 
 * the google weather api. From there it pseudo-parses the file using functions from \a flux_net_irc 
 * and throws the needed contents out of the main socket as a private message (using \a privmsg).
 */

void weather(Socket &sock, string rply){
  irc_string* reply = new irc_string(rply);
  if(reply->said("!weather")){
	string area = reply->params(1);
	string wget = "";
	if(strtol(area.c_str(),&pEnd,10) != 0){
	  wget = "wget -q -O temp.xml - http://www.google.com/ig/api?weather="+area;
	}else{
	  wget = "wget -q -O temp.xml - http://www.google.com/ig/api?weather="+makeSearchString(removeCommand("!weather",rply));
	}
	cout << "Reply: " << makeSearchString(removeCommand("!weather",rply)) << endl;
	system(wget.c_str());
	if(!irc_string::said(xmlToString("temp.xml"),"problem_cause")){
	  string ff = xmlToString("temp.xml");
	  string loc = findInXML("city","data",ff);
	  string cond = findInXML("condition","data",ff);
	  string tempf = findInXML("temp_f","data",ff);
	  cout << "Area: " << area << endl;
	  cout << "Reply: " << removeCommand("!weather",rply) << endl;
	  sock << privmsg(chan, "The current condition in %s is %s with a temperature of %s", loc.c_str(), cond.c_str(), tempf.c_str());
	}
  }
  delete reply;
}

/**
 * @}
 */
#endif
