/* Navn IRC bot -- Internet Control Message Protocol (ICMP) "ECHO" ping module
 *
 * (C) 2011-2012 Azuru
 * Contact us at Development@Azuru.net
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Anope by The Anope Team.
 */
#include "modules.h"
// NOTE: This module is based on source from http://cboard.cprogramming.com/networking-device-communication/41635-ping-program.html
// NOTE: This module requires the bot to run as root!

#include <linux/ip.h>
#include <linux/icmp.h>

/*
 * in_cksum --
 * Checksum routine for Internet Protocol
 * family headers (C Version)
 */
unsigned short in_cksum(unsigned short *addr, int len)
{
  register int sum = 0;
  u_short answer = 0;
  register u_short *w = addr;
  register int nleft = len;
  /*
   * Our algorithm is simple, using a 32 bit accumulator (sum), we add
   * sequential 16 bit words to it, and at the end, fold back all the
   * carry bits from the top 16 bits into the lower 16 bits.
   */
  while (nleft > 1)
  {
    sum += *w++;
    nleft -= 2;
  }
  /* mop up an odd byte, if necessary */
  if (nleft == 1)
  {
    *reinterpret_cast<u_char *>(&answer) = *reinterpret_cast<u_char *>(w);
    sum += answer;
  }
  /* add back carry outs from top 16 bits to low 16 bits */
  sum = (sum >> 16) + (sum & 0xffff);       /* add hi 16 to low 16 */
  sum += (sum >> 16);               /* add carry */
  answer = ~sum;              /* truncate to 16 bits */
  return (answer);
}

class PingThread : public Thread
{
  int times;
  CommandSource source;
  Flux::string address;
public:
  PingThread(CommandSource s, const Flux::string addr, int nums) : Thread(), times(nums), source(s), address(addr)
  {
    Log(LOG_THREAD) << "Created new Ping thread";
    this->Start();
  }

  ~PingThread()
  {
    Log(LOG_THREAD) << "Destroying ping thread";
    SetExitState();
  }

  void ToRun()
  {
    if(ForwardResolution(address).empty())
    {
      source.Reply("Cannot resolve \2%s\2", address.c_str());
      return;
    }

    for(int i = 0; i < times; ++i)
    {
      time_t sendtime = time(NULL);
      this->SendPing();
      source.c->SendMessage("Time: %is", static_cast<int>(time(NULL) - sendtime));
    }

    source.Reply("Sent \2%i\2 ping%s to \2%s\2", times, times <= 1 ? "" : "s", address.c_str());
    SetExitState();
  }
  
  void SendPing();
};

// Send pings to some server
void PingThread::SendPing()
{
  Flux::string someaddress = address;
  Flux::string ipaddress = ForwardResolution(someaddress);
  Channel *c = source.c;
  struct iphdr* ip;
  struct iphdr* ip_reply;
  struct icmphdr* icmp;
  struct protoent *proto;
  struct sockaddr_in connection;
  char* packet;
  char* buffer;
  int sockfd = 0;
  int optval = 0;
  socklen_t addrlen;

  // Get hostname and such
  char ipbuffer[256];
  gethostname(ipbuffer, 256);
  Flux::string hostnameip = ForwardResolution(ipbuffer);
  
  Log(LOG_DEBUG) << "Source address: " << ipbuffer << " (" << hostnameip << ')';
  Log(LOG_DEBUG) << "Destination address: " << address << " (" << ipaddress << ')';
  
  /*
   * allocate all necessary memory
   */
  ip = reinterpret_cast<struct iphdr*>(malloc(sizeof(struct iphdr)));
  ip_reply = reinterpret_cast<struct iphdr*>(malloc(sizeof(struct iphdr)));
  icmp = reinterpret_cast<struct icmphdr*>(malloc(sizeof(struct icmphdr)));
  packet = reinterpret_cast<char*>(malloc(sizeof(struct iphdr) + sizeof(struct icmphdr)));
  buffer = reinterpret_cast<char*>(malloc(sizeof(struct iphdr) + sizeof(struct icmphdr)));
  /****************************************************************/
  
  ip = reinterpret_cast<struct iphdr*>(packet);
  icmp = reinterpret_cast<struct icmphdr*>(packet + sizeof(struct iphdr));

  if ((proto = getprotobyname("icmp")) == NULL) {
    Log(LOG_WARN) << "[m_icmp_ping]: unknown protocol \"ICMP\"";
    source.Reply("Internal Error: ICMP protocol is unknown");
    return;
  }
  
  /*
   *  here the ip packet is set up except checksum
   */
  ip->ihl          = 5;
  ip->version      = 4;
  ip->tos          = 0;
  ip->tot_len      = sizeof(struct iphdr) + sizeof(struct icmphdr);
  ip->id           = htons(random());
  ip->ttl          = 255;
  ip->protocol     = proto->p_proto;
  ip->saddr        = inet_addr(hostnameip.c_str());
  ip->daddr        = inet_addr(address.c_str());


  
  
  if ((sockfd = socket(AF_INET, SOCK_RAW, proto->p_proto)) < 0)
  {
    source.Reply("Failed ping: %s", strerror(errno));
    return;
  }
  
  /*
   *  IP_HDRINCL must be set on the socket so that
   *  the kernel does not attempt to automatically add
   *  a default ip header to the packet
   */
  
  setsockopt(sockfd, IPPROTO_IP, IP_HDRINCL, &optval, sizeof(int));
  
  /*
   *  here the icmp packet is created
   *  also the ip checksum is generated
   */
  icmp->type           = ICMP_ECHO;
  icmp->code           = 0;
  icmp->un.echo.id     = 0;
  icmp->un.echo.sequence   = 0;
  icmp->checksum       = 0;
  icmp-> checksum      = in_cksum(reinterpret_cast<unsigned short *>(icmp), sizeof(struct icmphdr));
  
  ip->check            = in_cksum(reinterpret_cast<unsigned short *>(ip), sizeof(struct iphdr));
  
  connection.sin_family = AF_INET;
  connection.sin_addr.s_addr = inet_addr(address.c_str());
  
  /*
   *  now the packet is sent
   */
  
  sendto(sockfd, packet, ip->tot_len, 0, reinterpret_cast<struct sockaddr *>(&connection), sizeof(struct sockaddr));
  Log(LOG_DEBUG) << "Sent " << sizeof(packet) << " byte packet to " << address;
  c->SendMessage("Sent %i byte packet to %s (%s)", sizeof(packet), address.c_str(), ipaddress.c_str());
  
  /*
   *  now we listen for responses
   */
  addrlen = sizeof(connection);
  if (recvfrom(sockfd, buffer, sizeof(struct iphdr) + sizeof(struct icmphdr), 0, reinterpret_cast<struct sockaddr *>(&connection), &addrlen) == -1)
    source.Reply("Failed to ping %s: %s", address.c_str(), strerror(errno));
  else
  {
    c->SendMessage("Received %d byte reply from %s:\n", sizeof(buffer), address.c_str());
    ip_reply = reinterpret_cast<struct iphdr*>(buffer);
    c->SendMessage("ID: %d\n", ntohs(ip_reply->id));
    c->SendMessage("TTL: %d\n", ip_reply->ttl);
  }
  close(sockfd);
}

class CommandPing : public Command
{
  PingThread *thread;
public:
  CommandPing(module *m) : Command(m, "!PING", C_CHANNEL, 1, 2)
  {
    this->SetDesc("Sends an ICMP ping to a hostname or ipaddress");
    this->SetSyntax("\37[hostname|ipaddress]\37 times");
    thread = 0;
  }

  ~CommandPing()
  {
    if(thread)
      delete thread;
  }

  void Run(CommandSource &source, const Flux::vector &params)
  {
    Flux::string destination = params[0];
    Flux::string numbers = params.size() < 2 ? "1" : params[1];
    int times = static_cast<int>(numbers);
    thread = new PingThread(source, destination, times);
  }

  bool OnHelp(CommandSource &source, const Flux::string&)
  {
    this->SendSyntax(source);
    source.Reply(" ");
    source.Reply("Send an ICMP ping to a hostname to check if they're online or not");
    source.Reply("If \002times\002 is null, the number of pings will be assumed as 1\n"
      "\002NOTE\002: pings must be sent as root!\n"
      "\002NOTE\002: sending pings to addresses that do not reply will hang the thread.");
    return true;
  }
};

class m_icmp_ping : public module
{
  CommandPing commandping;
public:
  m_icmp_ping(const Flux::string &Name) : module(Name), commandping(this)
  {
    this->SetAuthor("Lordofsraam");
    this->SetVersion(VERSION);
  }
};

MODULE_HOOK(m_icmp_ping)