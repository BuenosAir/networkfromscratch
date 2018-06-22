#include <stdio.h>
#include <arpa/inet.h>
#include "tuntap.h"
#include "arp.h"

void printHexadecimal(unsigned char *frame, int length)
{
    int i = 0;
    for(i = 0; i < length; i++)
    {
        printf("%02x ", frame[i]);
    }
}

void printArpRequest(arp_ipv4 arp)
{
    struct in_addr tmp_addr;
    tmp_addr.s_addr = arp.sip;

    printf("Source ip : %s \n", inet_ntoa(tmp_addr));

    tmp_addr.s_addr = arp.dip;
    printf("Destination ip : %s\n", inet_ntoa(tmp_addr));
}

void printEtherFrame(eth_hdr frame)
{
    printf("Dmac : ");
    printHexadecimal(frame.dmac, 6);
    printf("\n");
    printf("Smac : " );
    printHexadecimal(frame.smac, 6);
    printf("\n");
    printf("Type : " );

    unsigned char ethertype[2];
    switch(frame.ethertype)
    {
        case ARP_FRAME: 
            printf("ARP"); break;
        case IPV6_FRAME : 
            printf("IPV6"); break;

        default :
            //Convert the ethertype to unsigned char
            printf("%02x", frame.ethertype);
            break;
    }
    printf("\n");
}

void printArpFrame(arp_hdr arp)
{
    printf("hwtype : %x\n", arp.hwtype);
    printf("protype : %x\n", arp.protype);
    printf("hwsize : %x\n", arp.hwsize);
    printf("prosize : %x\n", arp.prosize);
    printf("opcode : %x\n", arp.opcode);
}
