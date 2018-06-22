#ifndef ARP_H
#define ARP_H

#include <stdint.h>
#include "tuntap.h"

#define ARP_ETHERNET 0x01
#define ARP_IP 0x0800 //PROTYPE
#define ARP_HWSIZE 0x06
#define ARP_PROSIZE 0x04

//Lenght of the adress
//TODO: Use ARP_PROSIZE
#define ARP_IPV4 0x04

#define ARP_HEADER_LENGTH (2 * 2 + 2 * 1 + 2)
#define ARP_IPV4_LENGTH (6 + 4 + 6 + 4)

struct arp_hdr
{
    uint16_t hwtype;
    uint16_t protype;
    unsigned char hwsize;
    unsigned char prosize;
    uint16_t opcode;
    unsigned char data[];
} __attribute__((packed));

typedef struct arp_hdr arp_hdr;

struct arp_ipv4
{
    unsigned char smac[6];
    uint32_t sip;
    unsigned char dmac[6];
    uint32_t dip;
} __attribute__((packed));

typedef struct arp_ipv4 arp_ipv4;

struct arp_entry
{
    unsigned char mac[6];
    unsigned char ip[4];
} __attribute__((packed));

void arp_handling(eth_hdr *hdr);
void handle_arp_answer(arp_hdr *arpHdr);
void handle_arp_request(arp_hdr * arpHdr, eth_hdr *hdr);

#endif
