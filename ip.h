#ifndef IPV4
#define IPV4

#include "tuntap.h"

#define IPV4_FRAME 0x0800
#define IPV4_HEADER_LENGTH 1 + 1 + 2 + 2 + 2 + 1 + 1 + 2 + 4 + 4

//Only numbers (not flags) coded on >= 8 bits are to ntohs
struct ipv4_hdr{
    uint8_t version : 4;
    uint8_t ihl : 4;
    uint8_t tos;
    uint16_t len;
    uint16_t id;
    uint16_t flagsAndOffset;
    //uint16_t flags : 3;
    //uint16_t frag_offset : 13;
    uint8_t ttl;
    uint8_t proto;
    uint16_t csum;
    uint32_t saddr;
    uint32_t daddr;
    unsigned char data[];
} __attribute__((packed));
typedef struct ipv4_hdr ipv4_hdr;

void ipv4_handling(eth_hdr *hdr);
uint16_t checksumIP(ipv4_hdr *hdr);

#endif
