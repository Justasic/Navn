#ifndef TinyURL_H
#define TinyURL_H
#include "../includes.h"
#include "../flux_net_irc.hpp"
using namespace std;
using namespace flux_net_irc;
    
void TinyURL(Socket &sock, irc_string *reply){

  if (reply->said("http://") || reply->said("https://")){
    Flux::string http = "http://";
    Flux::string https = "https://";
    Flux::string url;
    size_t nerp;
    
      nerp = msg.find(http);
      if(nerp!=Flux::string::npos){
	url = reply->params(int(nerp));
	cout << int(nerp) << endl;
	cout << url << endl;
	
      }
      nerp = msg.find(https);
      if(nerp!=Flux::string::npos){
	url = reply->params(int(nerp));
	cout << int(nerp) << endl;
	cout << url << endl;
	
      }
  }

}
#endif
