/* All code is licensed under GNU General Public License GPL v3 (http://www.gnu.org/licenses/gpl.html) */
#ifndef TinyURL_H
#define TinyURL_H

/* This module was not yet finished, check Navn version 2.0.1 to see if it was completed or removed */
class TinyURL:public module{
public:
  TinyURL(bool a):module("TinyURL", a, PRIORITY_LAST){ this->SetDesc("The tinyurl minifier"); }
  ModuleReturn run(SendMessage *Send, Flux::string rply, irc_string *reply){

  if (reply->said("http://") || reply->said("https://")){
    Flux::string http = "http://";
    Flux::string https = "https://";
    Flux::string url;
    size_t message;
    
      message = msg.find(http);
      if(message!=Flux::string::npos){
	url = reply->params(int(message));
	cout << int(message) << endl;
	cout << url << endl;
	
      }
      message = msg.find(https);
      if(message!=Flux::string::npos){
	url = reply->params(int(message));
	cout << int(message) << endl;
	cout << url << endl;
	
      }
  }
  return MOD_RUN;
 }
};
#endif
