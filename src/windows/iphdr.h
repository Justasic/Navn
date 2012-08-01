/* Navn IRC bot -- Protocol header definitions used by trace route (ICMP raw sockets)
 *
 * (C) 2011-2012 Azuru
 * Contact us at Development@Azuru.net
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Anope by The Anope Team.
 */

#ifdef _WIN32
// Set alignment boundary to 1 byte
#include <pshpack1.h>

// IPv4 header
typedef struct iphdr
{
#if __BYTE_ORDER == __LITTLE_ENDIAN
	unsigned char  ihl:4;         //
    unsigned char  version:4;     // 4-bit IPv4 version
                                  // 4-bit header length (in 32-bit words)
#elif __BYTE_ORDER == __BIG_ENDIAN
	unsigned char  version:4;     // 4-bit IPv4 version
                                  // 4-bit header length (in 32-bit words)
	unsigned char  ihl:4;         //
#else
# error "Your systems ENDIANNESS is broken, please fix!"
#endif

    unsigned char  tos;           // IP type of service
    unsigned short tot_len;       // Total length
    unsigned short id;            // Unique identifier 
    unsigned short offset;        // Fragment offset field
    unsigned char  ttl;           // Time to live
    unsigned char  protocol;      // Protocol(TCP,UDP etc)
    unsigned short check;         // IP checksum
    unsigned int   saddr;         // Source address
    unsigned int   daddr;         // Source address
} IPV4_HDR, *PIPV4_HDR, FAR * LPIPV4_HDR;

// IPv4 option header
typedef struct ipv4_option_hdr
{
    unsigned char   opt_code;           // option type
    unsigned char   opt_len;            // length of the option header
    unsigned char   opt_ptr;            // offset into options
    unsigned long   opt_addr[9];        // list of IPv4 addresses
} IPV4_OPTION_HDR, *PIPV4_OPTION_HDR, FAR *LPIPV4_OPTION_HDR;

// ICMPv6 echo request body
typedef struct icmpv6_echo_request
{
	struct
	{
		unsigned short  id;
		unsigned short  sequence;
	} echo;
} ICMPV6_ECHO_REQUEST;

// ICMP header
typedef struct icmphdr
{
    unsigned char   type;
    unsigned char   code;
    unsigned short  checksum;
    unsigned short  id;
    unsigned short  sequence;
    unsigned long   timestamp;
	icmpv6_echo_request un;
} ICMP_HDR, *PICMP_HDR, FAR *LPICMP_HDR;

// IPv6 protocol header
typedef struct ipv6_hdr
{
    unsigned long   ipv6_vertcflow;        // 4-bit IPv6 version
                                           // 8-bit traffic class
                                           // 20-bit flow label
    unsigned short  ipv6_payloadlen;       // payload length
    unsigned char   ipv6_nexthdr;          // next header protocol value
    unsigned char   ipv6_hoplimit;         // TTL 
    struct in6_addr ipv6_srcaddr;          // Source address
    struct in6_addr ipv6_destaddr;         // Destination address
} IPV6_HDR, *PIPV6_HDR, FAR * LPIPV6_HDR;

// IPv6 fragment header
typedef struct ipv6_fragment_hdr
{
    unsigned char   ipv6_frag_nexthdr;
    unsigned char   ipv6_frag_reserved;
    unsigned short  ipv6_frag_offset;
    unsigned long   ipv6_frag_id;
} IPV6_FRAGMENT_HDR, *PIPV6_FRAGMENT_HDR, FAR * LPIPV6_FRAGMENT_HDR;

// ICMPv6 header
typedef struct icmpv6_hdr {
    unsigned char   icmp6_type;
    unsigned char   icmp6_code;
    unsigned short  icmp6_checksum;
} ICMPV6_HDR;

// Define the UDP header 
typedef struct udp_hdr
{
    unsigned short src_portno;       // Source port no.
    unsigned short dst_portno;       // Dest. port no.
    unsigned short udp_length;       // Udp packet length
    unsigned short udp_checksum;     // Udp checksum (optional)
} UDP_HDR, *PUDP_HDR;

#define IP_RECORD_ROUTE     0x7

// ICMP6 protocol value
#define IPPROTO_ICMP6       58

// ICMP types and codes
#define ICMPV4_ECHO_REQUEST_TYPE   8
#define ICMPV4_ECHO_REQUEST_CODE   0
#define ICMPV4_ECHO_REPLY_TYPE     0
#define ICMPV4_ECHO_REPLY_CODE     0

#define ICMPV4_DESTUNREACH    3
#define ICMPV4_SRCQUENCH      4
#define ICMPV4_REDIRECT       5
#define ICMP_ECHO           8
#define ICMPV4_TIMEOUT       11
#define ICMPV4_PARMERR       12

// ICMP6 types and codes
#define ICMPV6_ECHO_REQUEST_TYPE   128
#define ICMPV6_ECHO_REQUEST_CODE   0
#define ICMPV6_ECHO_REPLY_TYPE     129
#define ICMPV6_ECHO_REPLY_CODE     0
#define ICMPV6_TIME_EXCEEDED_TYPE  3
#define ICMPV6_TIME_EXCEEDED_CODE  0

// Restore byte alignment to compile default
#include <poppack.h>
#endif