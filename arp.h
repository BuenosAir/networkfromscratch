#ifndef ARP_H
#define ARP_H

#include <stdint.h>
#include "tuntap.h"

#define ARP_ETHERNET 0x01
#define ARP_IP 0x0800
#define ARP_HWSIZE 0x06

//Lenght of the adress
#define ARP_IPV4 0x04

struct arp_hdr
{
    uint16_t hwtype;
    uint16_t protype;
    unsigned char hwsize;
    unsigned char prosize;
    uint16_t opcode;
    unsigned char *data;
} __attribute__((packed));

typedef struct arp_hdr arp_hdr;

struct arp_ipv4
{
    unsigned char smac[6];
    uint32_t sip;
    unsigned char dmac[6];
    uint32_t dip;
} __attribute__((packed));

struct arp_entry
{
    unsigned char mac[6];
    unsigned char ip[4];
} __attribute__((packed));

void arp_handling(eth_hdr *hdr);
void handle_arp_answer(arp_hdr *arpHdr);
void handle_arp_request(arp_hdr * arpHdr);

#endif
