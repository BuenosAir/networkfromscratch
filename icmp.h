#ifndef ICMP_H
#define ICMP_H

#include "ip.h"
#include "tuntap.h"

#define ICMP_FRAME 1
#define ICMP_HEADER_LENGTH 1 + 1 + 2

struct icmp_hdr{
    uint8_t type;
    uint8_t code;
    uint16_t csum;
    uint8_t data[];
} __attribute__((packed));
typedef struct icmp_hdr icmp_hdr;

struct icmp_v4_echo {
    uint16_t id;
    uint16_t seq;
    uint8_t data[];
} __attribute__((packed));

void icmp_handling(ipv4_hdr *ip, eth_hdr *eth);
void answer_echo(icmp_hdr *icmp, ipv4_hdr *ip, eth_hdr *eth);
 

#endif
