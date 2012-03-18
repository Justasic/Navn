/* Navn IRC bot -- DNS lookup tools for IRC
 * 
 * (C) 2011-2012 Flux-Net
 * Contact us at Development@Azuru.net
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Anope by The Anope Team.
 */
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
  CommandCDNS(module *m):Command(m, "!DNS", C_CHANNEL, 1, 1)
  {
    this->SetDesc("Displays a resolved hostname");
    this->SetSyntax("hostname"); 
  }
  void Run(CommandSource &source, const Flux::vector &params)
  {
    Flux::string ip = ForwardResolution(params[1]);
    
    if (ip.empty())
	source.c->SendMessage("\0034[DNS]\017 Error");
    else
	source.c->SendMessage("\0034[DNS]\017 %s", ip.c_str());
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
class CommandCADNS : public Command
{
public:
  CommandCADNS(module *m):Command(m, "!ADNS", C_CHANNEL, 1, 1)
  {
    this->SetDesc("Displays ALL resolved ip addresses from hostnames");
    this->SetSyntax("hostname");
  }
  void Run(CommandSource &source, const Flux::vector &params)
  {
    Flux::string hostname = params[1];
    struct addrinfo *result, *res;
    int err = getaddrinfo(hostname.c_str(), NULL, NULL, &result);
    
    if(err != 0)
    {
      source.c->SendMessage("Failed to resolve %s: %s", hostname.c_str(), gai_strerror(err));
      return;
    }
    
    Flux::string ret = hostname, laststr;
    int c = 0;
    
    for(res = result; res != NULL; res = res->ai_next)
    {
      struct sockaddr *haddr = res->ai_addr;
      char address[INET6_ADDRSTRLEN + 1] = "";
      
      switch(haddr->sa_family)
      {
	case AF_INET:
	  struct sockaddr_in *v4;
	  v4 = reinterpret_cast<struct sockaddr_in*>(haddr);
	  if (!inet_ntop(AF_INET, &v4->sin_addr, address, sizeof(address))){
	    source.c->SendMessage("DNS: %s", strerror(errno));
	    return;
	  }
	  break;
	case AF_INET6:
	  struct sockaddr_in6 *v6;
	  v6 = reinterpret_cast<struct sockaddr_in6*>(haddr);
	  if (!inet_ntop(AF_INET6, &v6->sin6_addr, address, sizeof(address))){
	    source.c->SendMessage("DNS: %s", strerror(errno));
	    return;
	  }
	  break;
      }
      
      ret = address;
      if(!laststr.equals_cs(ret))
      {
	source.c->SendMessage("\0034[ADNS]\017 %s", ret.c_str());
	laststr = ret;
	c++; //LOL!
      }
    }
    source.c->SendMessage("\0034[ADNS]\017 Total of %i IP address%s", c, c > 1?"es":"");
    freeaddrinfo(result);
  }
  bool OnHelp(CommandSource &source, const Flux::string&)
  {
    this->SendSyntax(source);
    source.Reply(" ");
    source.Reply("This command lets a user resolve\n"
		 "all IP addresses associated with a\n"
		 "given hostname.");
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
  CommandCRDNS(module *m):Command(m, "!RDNS", C_CHANNEL, 1, 1)
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
    else
    {
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
  CommandCARDNS(module *m):Command(m, "!ARDNS", C_CHANNEL, 1, 1)
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
    if (error != 0) 
      source.c->SendMessage("\0034[All rDNS]\017 Error: %s\n", gai_strerror(error));
    else
    {
      Flux::string laststr, ret;
      for (res = result; res != NULL; res = res->ai_next)
      {
	char hostname[NI_MAXHOST] = "";
	error = getnameinfo(res->ai_addr, res->ai_addrlen, hostname, NI_MAXHOST, NULL, 0, 0);

	if(error != 0){
	  source.c->SendMessage("\0034[All rDNS]\017 error in getnameinfo: %s", gai_strerror(error));
	  continue;
	}

	ret = hostname;
	if(!laststr.equals_cs(ret))
	{
	  source.c->SendMessage("\0034[All rDNS]\017 %s", hostname);
	  laststr = ret;
	  count++;
	}
      }
    
      source.c->SendMessage("\0034[All rDNS]\017 Total of %i hostname%s.", count, count > 1?"s":"");
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
  CommandCADNS adns;
public:
  dns_m(const Flux::string &Name):module(Name), ardns(this), rdns(this), dns(this), adns(this)
  { 
    this->SetAuthor("Justasic");
    this->SetVersion(VERSION);
  }
};
/**
 * @}
 */
MODULE_HOOK(dns_m)
