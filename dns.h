#ifndef DNS_H
#define DNS_H
#include "includes.h"
#include "flux_net_irc.hpp"
using namespace std;
using namespace flux_net_irc;
ostringstream nerp;
void dns_callback (void* arg, int status, int timeouts, struct hostent* host)
{
    if(status == ARES_SUCCESS)
        nerp << host->h_name << "\n";
    else
        nerp << "lookup failed: " << status << "\n";
}
void main_loop(ares_channel &channel)
{
    int nfds, count;
    fd_set readers, writers;
    timeval tv, *tvp;
    while (1) {
        FD_ZERO(&readers);
        FD_ZERO(&writers);
        nfds = ares_fds(channel, &readers, &writers);
        if (nfds == 0)
          break;
        tvp = ares_timeout(channel, NULL, &tv);
        count = select(nfds, &readers, &writers, NULL, tvp);
        ares_process(channel, &readers, &writers);
     }
}
/** Reverse DNS Lookup
 * Looks up an IP address to host
 * @Param rdns(Socket, rawircstring, Destination, ipaddress)
 */
void rdns(Socket &sock, string rply, string dest, string argv) {
    irc_string* reply = new irc_string(rply);
    nerp.str("");//some weird ass fixes to get this to work without segmentation faults.
    struct in_addr ip;
    int res;
    inet_aton(argv.c_str(), &ip);
    ares_channel channel;
    if((res = ares_init(&channel)) != ARES_SUCCESS) {
	nerp << res;
        sock << "PRIVMSG " << dest << " :ares feiled: " << nerp.str() << "\n";
    }
    ares_gethostbyaddr(channel, &ip, sizeof ip, AF_INET, dns_callback, NULL);
    main_loop(channel);
    sock << strip("PRIVMSG "+dest+" :\0034[rDNS]\017 "+nerp.str())+"\n";
    delete reply;
}
/** Domain look up
 * Looks up a domain name to ip address
 * @Param(Socket, Destination, Domain Name)
 */
void dns(Socket &sock, string dest, string host){
	 struct hostent *he;
	 he = gethostbyname(host.c_str());
	 if (he == NULL){
	 sock << privmsg(dest, "\0034[DNS]\017 Lookup Failed.");
	}else{
	 sock << privmsg(dest, "\0034[DNS]\017 %s", inet_ntoa(*(struct in_addr*)he->h_addr));
	}
}

#endif
