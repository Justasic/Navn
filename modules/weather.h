#ifndef WEATHER_H
#define WEATHER_H
#include "../includes.h"
#include "../flux_net_irc.hpp"
using namespace std;
using namespace flux_net_irc;

void get_google_xml(string area){
 try{
  Socket socket(string("google.com"),string("80")); 
  socket << "GET /ig/api?weather="+area+" http/1.1" << nl;
  socket << "HOST: google.com" << nl;
  string buf;
  socket >> buf;
  fstream html;
  html.open("temp.xml", fstream::in | fstream::out | fstream::app);
  if(!html.is_open())
     throw LogException("Failed to open log file.");
  html << buf << endl;
  html.close();
  }catch (LogException &e){
   cerr << "HTML write Exception Caught: " << e.GetReason() << nl;
   log("HTML write Exception Caught: %s", e.GetReason());
  }catch (SocketException &e){
   cerr << "Socket Exception caught: " <<  e.description() << nl;
   log("Socket Exception Caught: %s",  e.description().c_str());
  }
}

string get_weather(string area){
 //string wget = "wget -q -O temp.xml - http://www.google.com/ig/api?weather="+area;
 //system(wget.c_str());
 ifstream file("temp.xml");
 get_google_xml(area);
 string line;
 string value;
 string city;
 string postalcode;
 string date;
 string system;
 string condition;
 string temp_f;
 string humidity;
 string wind;
 while(getline(file,line)){
 irc_string* wreply = new irc_string(line);

 if(wreply->said("<city")){
	if(line[11] = '='){
		city=trim(line.substr(11,line.find('=')-1));
		continue;
	}
 }
	if(wreply->said("<postal_code")){
	if(line[18] = '='){
		postalcode=trim(line.substr(18,line.find('=')-1));
		continue;
	}
	}
	if(wreply->said("<forcast_date")){
	if(line[11] = '='){
		date=trim(line.substr(11,line.find('=')-1));
		continue;
	}
 }
if(wreply->said("unit_system")){
	if(line[18] = '='){
		system=trim(line.substr(18,line.find('=')-1));
		continue;
	}
 }
if(wreply->said("condition")){
	if(line[16] = '='){
		condition=trim(line.substr(16,line.find('=')-1));
		continue;
	}
 }
 if(wreply->said("temp_f")){
	if(line[13] = '='){
		temp_f=trim(line.substr(13,line.find('=')-1));
		continue;
	}
 }
 if(wreply->said("humidity")){
	if(line[18] = '='){
		humidity=trim(line.substr(18,line.find('=')-1));
		continue;
	}
 }
 if(wreply->said("wind_condition")){
	if(line[22] = '='){
		wind=trim(line.substr(22,line.find('=')-1));
		continue;
	}
 }
 delete wreply;
 }
 remove("temp.xml");
 return "Weather for "+city+" "+postalcode+" "+date+" "+system+" "+condition+" "+wind+" "+humidity+" "+temp_f;
}

void weather(Socket &sock, irc_string *reply){
/*if(reply->said("!weather")){
	string area = reply->params(2);
	if(area.empty()){
	  sock << privmsg(unick, "Please enter a postal area code.");
	}else{
	  sock << privmsg(chan, get_weather(area));
	  log("%s used weather command to get the weather for %s in %s", unick.c_str(), area.c_str(), chan.c_str());
        }
    }*/
}
#endif
