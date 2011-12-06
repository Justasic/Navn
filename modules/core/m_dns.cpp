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
class CommandCDNS : public Command
{
public:
  CommandCDNS():Command("!DNS", 1, 1)
  {
    this->SetDesc("Displays a resolved hostname/domain");
    this->SetSyntax("hostname"); 
  }
  void Run(CommandSource &source, const Flux::vector &params)
  {
    struct hostent *he;
    he = gethostbyname(params[1].c_str());
    if (he <= 0)
	source.c->SendMessage("\0034[DNS]\017 Error: %i", he);
    else
	source.c->SendMessage("\0034[DNS]\017 %s", inet_ntoa(*(struct in_addr*)he->h_addr));
  }
  bool OnHelp(CommandSource &source, const Flux::string &nill)
  {
    this->SendSyntax(source);
    source.Reply(" ");
    source.Reply("This command lets a user do a\n"
		 "DNS resolution of a Domain Name which\n"
		 "can be useful to find what a Domain name\n"
		 "Resolves to or if it resolves at all");
    return true;
  }
};
/** Reverse DNS Lookup
* Looks up an IP address to host
* @Param rdns(Socket, rawircFlux::string, Destination, ipaddress)
*/
// http://en.wikipedia.org/wiki/Getaddrinfo
class CommandCRDNS : public Command
{
public:
  CommandCRDNS():Command("!RDNS", 1, 1)
  {
    this->SetDesc("Displays a reversely resolved DNS IP Address");
    this->SetSyntax("ipaddress"); 
  }
  void Run(CommandSource &source, const Flux::vector &params)
  {
    struct addrinfo *result;
    struct addrinfo *res;
    int error;
    
    error = getaddrinfo(params[1].c_str(), NULL, NULL, &result);
    if (error != 0)
      source.c->SendMessage("\0034[rDNS]\017 Error: %s\n", gai_strerror(error)); 
    else{
    char hostname[NI_MAXHOST] = "";
    res = result;
      error = getnameinfo(res->ai_addr, res->ai_addrlen, hostname, NI_MAXHOST, NULL, 0, 0);
      if(error != 0)
	source.c->SendMessage("\0034[rDNS]\017 Error: %s", gai_strerror(error));
      if(*hostname != '\0')
	source.c->SendMessage("\0034[rDNS]\017 %s", hostname);
      freeaddrinfo(result);
    }
  }
  bool OnHelp(CommandSource &source, const Flux::string &nill)
  {
    this->SendSyntax(source);
    source.Reply(" ");
    source.Reply("This command allows a user to do a\n"
		 "Reverse DNS lookup on a Domain Name or\n"
		 "IP address and only list one return");
    return true;
  }
};
/** All hosts reverse DNS Lookup
* Looks up an IP address to all the host
* @Param rdns(SendMessage, Flux::string hostname, Flux::string destination)
*/
class CommandCARDNS : public Command
{
public:
  CommandCARDNS():Command("!ARDNS", 1, 1)
  {
    this->SetDesc("Displays ALL reverse DNS ip addresses");
    this->SetSyntax("ipaddress"); 
  }
  void Run(CommandSource &source, const Flux::vector &params)
  {
    struct addrinfo *result;
    struct addrinfo *res;
    int error;
    int count = 0;
    
    error = getaddrinfo(params[1].c_str(), NULL, NULL, &result);
    if (error != 0){ 
      source.c->SendMessage("\0034[All rDNS]\017 Error: %s\n", gai_strerror(error)); 
    }else{
    for (res = result; res != NULL; res = res->ai_next){
      char hostname[NI_MAXHOST] = "";
      error = getnameinfo(res->ai_addr, res->ai_addrlen, hostname, NI_MAXHOST, NULL, 0, 0);
      if(error != 0){ 
	source.c->SendMessage("\0034[All rDNS]\017 error in getnameinfo: %s", gai_strerror(error)); 
	continue; 
      }
      if(*hostname != '\0'){
	source.c->SendMessage("\0034[All rDNS]\017 %s", hostname);
	count++;
      }
    }
    source.c->SendMessage("\0034[All rDNS]\017 Total of %i hostnames.", count);
    freeaddrinfo(result);
    }
  }
  bool OnHelp(CommandSource &source, const Flux::string &nill)
  {
    this->SendSyntax(source);
    source.Reply(" ");
    source.Reply("This command allows a user to do a\n"
		 "Reverse DNS lookup on an IP address\n"
		 "or a Domain Name and list ALL results");
    return true;
  }
};

class dns_m:public module
{
  CommandCARDNS ardns;
  CommandCRDNS rdns;
  CommandCDNS dns;
public:
  dns_m(const Flux::string &Name):module(Name){ 
    this->SetAuthor("Justasic");
    this->SetVersion(VERSION);
    this->AddChanCommand(&ardns);
    this->AddChanCommand(&rdns);
    this->AddChanCommand(&dns);
  }
};
/**
 * @}
 */
MODULE_HOOK(dns_m)
