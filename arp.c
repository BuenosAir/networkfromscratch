#include <stdint.h>
#include <stdio.h>
//Provide htons
#include <arpa/inet.h>

#include "tuntap.h"
#include "arp.h"
#include "debug.h"


//Reminder of the struct 
/*struct arp_hdr*/
/*{*/
    /*uint16_t hwtype;*/
    /*uint16_t protype;*/
    /*unsigned char hwsize;*/
    /*unsigned char prosize;*/
    /*uint16_t opcode;*/
    /*unsigned char data[];*/
/*} __attribute__((packed));*/



void arp_handling(eth_hdr *hdr)
{
    //Convert the arp headers 
    arp_hdr *arpHdr;
    arpHdr = (arp_hdr *) hdr->payload;

    //Force the encoding
    arpHdr->hwtype = ntohs(arpHdr->hwtype);
    arpHdr->protype = ntohs(arpHdr->protype);
    arpHdr->opcode = ntohs(arpHdr->opcode);

    //Print the basics informations
    printArpFrame(*arpHdr);

    //Check the types 
    if(arpHdr->hwtype != ARP_ETHERNET)
    {
        printf("Wrong type of arp hardware type\n");
        return;
    }

    //Check the protocol
    if(arpHdr->protype != ARP_IP)
    {
        printf("Wrong type of arp protype\n");
        return;
    }

    if(arpHdr->hwsize != ARP_HWSIZE)
    {
        printf("Wrong type of ip type\n");
        return;
    }

    if(arpHdr->prosize != ARP_IPV4)
    {
        printf("Wrong type of ip type\n");
        return;
    }

    switch(arpHdr->opcode)
    {
        //Request
        case 0x01: 
            handle_arp_request(arpHdr);
            break;
        case 0x02:
            handle_arp_answer(arpHdr);
            break;
        default : 
            printf("Wrong opcode for arp request\n");
            break;
    }
}

void handle_arp_request(arp_hdr * arpHdr)
{
    printf("Handling arp request\n");
    return;
}

void handle_arp_answer(arp_hdr *arpHdr)
{
    printf("Handling arp answer \n");
    return;
}

