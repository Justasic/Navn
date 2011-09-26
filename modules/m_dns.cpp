/* All code is licensed under GNU General Public License GPL v3 (http://www.gnu.org/licenses/gpl.html) */
#ifndef NI_MAXHOST
#define NI_MAXHOST 1025
#endif
#include "flux_net_irc.hpp"

/**
 * \file dns.h Header file holding the \a DNS functions.
 * \author Justasic.
 */

/**
 * \defgroup dnsM DNS module
 * This is the module for the DNS functions.
 * Resolves a DNS hostname or Reverse DNS resolve a ip address to a hostname
 * For a better description see the function description.
 * \section commands Commands associated with this module.
 * \subsection dns !dns
 * Say \a !dns with a hostname and it will return the resolved IP address
 * \subsection rdns !rdns
 * Say \a !rdns with an IP address or hostname and it will return the reverse DNS hostname
 * \subsection ardns !ardns
 * Say \a !ardns with an IP address or hostname and it will return all the reverse DNS hostnames
 * @{
 */

/**
 * \fn void dns(SendMessage *Send, Flux::string dest, Flux::string host){
 * \fn void rdns(SendMessage *Send, Flux::string dest, Flux::string host){
 * \fn void alldns(SendMessage *Send, Flux::string host, Flux::string dest){
 * \fn class dns_m(bool a):module("DNS Resolver", a, PRIORITY_DONTCARE){ this->SetDesc("Reverse DNS or Forward DNS resolve an address"); }
 * \brief Resolve DNS queries
 * Resolves a DNS hostname or Reverse DNS resolve a ip address to a hostname
 */

/** Domain look up
* Looks up a domain name to ip address
* @Param(Socket, Destination, Domain Name)
*/
void dns(Flux::string dest, Flux::string host){
  struct hostent *he;
  he = gethostbyname(host.c_str());
  if (he <= 0){
  Send->privmsg(dest, "\0034[DNS]\017 Error: %i", he);
  }else{
  Send->privmsg(dest, "\0034[DNS]\017 %s", inet_ntoa(*(struct in_addr*)he->h_addr));
  }
}
/** Reverse DNS Lookup
* Looks up an IP address to host
* @Param rdns(Socket, rawircFlux::string, Destination, ipaddress)
*/
// http://en.wikipedia.org/wiki/Getaddrinfo
void rdns(Flux::string dest, Flux::string host){
  struct addrinfo *result;
  struct addrinfo *res;
  int error;
  
  error = getaddrinfo(host.c_str(), NULL, NULL, &result);
  if (error != 0){ 
    Send->privmsg(dest, "\0034[rDNS]\017 Error: %s\n", gai_strerror(error)); 
  }else{
   char hostname[NI_MAXHOST] = "";
   res = result;
    error = getnameinfo(res->ai_addr, res->ai_addrlen, hostname, NI_MAXHOST, NULL, 0, 0);
    if(error != 0){ 
      Send->privmsg(dest, "\0034[rDNS]\017 Error: %s", gai_strerror(error));
    }
    if(*hostname != '\0')
      Send->privmsg(dest, "\0034[rDNS]\017 %s", hostname);
    freeaddrinfo(result);
  }
}
/** All hosts reverse DNS Lookup
* Looks up an IP address to all the host
* @Param rdns(SendMessage, Flux::string hostname, Flux::string destination)
*/
void alldns(Flux::string host, Flux::string dest){
  struct addrinfo *result;
  struct addrinfo *res;
  int error;
  int count = 0;
  
  error = getaddrinfo(host.c_str(), NULL, NULL, &result);
  if (error != 0){ 
    Send->privmsg(dest, "\0034[All rDNS]\017 Error: %s\n", gai_strerror(error)); 
  }else{
  for (res = result; res != NULL; res = res->ai_next){
    char hostname[NI_MAXHOST] = "";
    error = getnameinfo(res->ai_addr, res->ai_addrlen, hostname, NI_MAXHOST, NULL, 0, 0);
    if(error != 0){ 
      Send->privmsg(dest, "\0034[All rDNS]\017 error in getnameinfo: %s", gai_strerror(error)); 
      continue; 
    }
    if(*hostname != '\0'){
      Send->privmsg(dest, "\0034[All rDNS]\017 %s", hostname);
      count++;
    }
  }
  Send->privmsg(dest, "\0034[All rDNS]\017 Total of %i hostnames.", count);
  freeaddrinfo(result);
  }
}

class dns_m:public module
{
public:
  dns_m():module("DNS Resolver", PRIORITY_DONTCARE){ 
    this->SetAuthor("Justasic");
    this->SetVersion(VERSION);
    ModuleHandler::Attach(I_OnPrivmsg, this);
  }
  void OnPrivmsg(User *u, Channel *c, const std::vector<Flux::string> &params){
    Flux::string cmd = params.empty()?"":params[0];
    
    if(cmd.equals_ci("!rdns")){
     if(params.size() < 2){
       u->SendMessage("Syntax: \2!RDNS \37ipaddress\15");
       return;
     }
      Flux::string ip = params[params.size() -1];
      rdns(c->name, ip);
    }
    if(cmd.equals_ci("!dns")){
     if(params.size() < 2){
       u->SendMessage("Syntax: \2!DNS \37hostname\15");
       return;
     }
      Flux::string host = params[params.size() -1];
      dns(c->name, host);
    }
    if(cmd.equals_ci("!ardns")){
     if(params.size() < 2){
       u->SendMessage("Syntax: \2!ARDNS \37hostname\15");
       return;
     }
     Flux::string host = params[params.size() -1];
     alldns(host, c->name);
      
    }
  }
};
/**
 * @}
 */
MODULE_HOOK(dns_m)
