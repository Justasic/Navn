#ifndef WEATHER_H
#define WEATHER_H
#include "../includes.h"
#include "../flux_net_irc.hpp"
using namespace std;
using namespace flux_net_irc;

char * pEnd;

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
	  sock << chanmsg("The current condition in "+ loc +" is "+ cond +" with a temperature of "+ tempf);
	}
  }
  delete reply;
}
#endif
