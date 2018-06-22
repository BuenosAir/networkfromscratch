#ifndef DEBUG_H
#define DEBUG_H

#include "tuntap.h"
#include "arp.h"

void printHexadecimal(unsigned char *frame, int length);
void printEtherFrame(eth_hdr frame);
void printArpFrame(arp_hdr arp);
void printArpRequest(arp_ipv4 arp);

#endif 
