#ifndef ICMP_H
#define ICMP_H

#include "ip.h"

#define ICMP_FRAME 1

void icmp_handling(ipv4_hdr *ip);

#endif
