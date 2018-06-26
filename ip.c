#include <stdint.h>
#include <stdio.h>
//Provide htons
#include <arpa/inet.h>

#include <stdlib.h>
#include <string.h>

#include "tuntap.h"
#include "ip.h"
#include "debug.h"
#include "icmp.h"

void ipv4_handling(eth_hdr *hdr)
{
    //Convert to ipv4 header
    ipv4_hdr *ip = (ipv4_hdr *)hdr->payload;
    

    //Do the conversion of certain values
    uint8_t swapVersionIhl;
    swapVersionIhl = ip->version;
    ip->version = ip->ihl;
    ip->ihl = swapVersionIhl;

    printIpv4Frame(ip);

    //Check the version
    if(ip->version != 4)
    {
        printf("\n%d ", fourBitntoh(ip->version));
        printf("This is not an ipv4 packet \n");
        return;
    }

    //Check the source address
    if(ntohl(ip->daddr) != getLocalIpAddress())
    {
        printf("This packet is not for our IP \n");
        return;
    }

    if(ip->ttl == 0)
    {
        printf("This packet ended his life, let him die in peace \n");
        return;
    }

    //Check the prototype
    switch(ip->proto)
    {
        case ICMP_FRAME : 
            printf("Handling ICMP frame\n");
            icmp_handling(ip);
            break;
    }
    
}
