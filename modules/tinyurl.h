/* All code is licensed under GNU General Public License GPL v3 (http://www.gnu.org/licenses/gpl.html) */
#ifndef TinyURL_H
#define TinyURL_H

/* This module was not yet finished, check Navn version 2.0.1 to see if it was completed or removed */
class TinyURL:public module{
public:
  TinyURL(bool a):module("TinyURL", a, PRIORITY_LAST){ 
    this->SetDesc("The tinyurl minifier"); 
  }
  ModuleReturn run(CommandSource &source, std::vector<Flux::string> &params){
    Flux::string cmd = params.empty()?"":params[0];
    
  if (irc_string::said(source.message, ci::string("http://")) || irc_string::said(source.message, ci::string("https://"))){
    Flux::string http = "http://";
    Flux::string https = "https://";
    Flux::string url;
    
  }
  return MOD_RUN;
 }
};
#endif
