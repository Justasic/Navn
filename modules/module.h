#ifndef MODULE_H
#define MODULE_H
#include "../includes.h"
#include "../flux_net_irc.hpp"

using namespace std;
using namespace flux_net_irc;

class module{
  
  string name;
  
  bool activated;
  
  module(string n, bool a){
    name = n;
    activated = a;
  }

void run(Socket &sock, string rply, irc_string *reply) =0;
  
}

#endif