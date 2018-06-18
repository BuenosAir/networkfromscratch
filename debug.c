#include <stdio.h>
#include "tuntap.h"

void printHexadecimal(unsigned char *frame, int length)
{
    int i = 0;
    for(i = 0; i < length; i++)
    {
        printf("%02x ", frame[i]);
    }
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
