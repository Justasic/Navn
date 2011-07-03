#ifndef MODULE_H
#define MODULE_H
#include "../includes.h"
#include "../flux_net_irc.hpp"

using namespace std;
using namespace flux_net_irc;

class module{

public:
  string name;
  bool activated;

  module (string , bool);
  
  virtual void run(Socket &sock, string rply, irc_string *reply) =0;
  
};

vector<module*> moduleList;

module::module(string n, bool a){
  name = n;
  activated = a;
  moduleList.push_back(this);
}

#endif