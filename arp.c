#include <stdint.h>
#include <stdio.h>
//Provide htons
#include <arpa/inet.h>

#include <stdlib.h>
#include <string.h>

#include "tuntap.h"
#include "arp.h"
#include "debug.h"


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
            handle_arp_request(arpHdr, hdr);
            break;
        case 0x02:
            handle_arp_answer(arpHdr);
            break;
        default :
            printf("Wrong opcode for arp request\n");
            break;
    }
}

void handle_arp_request(arp_hdr * arpHdr, eth_hdr *hdr)
{
    printf("Handling arp request\n");

    arp_ipv4 *arpIp4 = (arp_ipv4 *) arpHdr->data;

    //Print the frame
    printArpRequest(*arpIp4);
    
    //That's ugly, but who care ?
    struct ifreq *ifr = getNetworkCard();

    //Check if the request is for us
    if(! compareMac(arpIp4->dmac,(unsigned char *) ifr->ifr_hwaddr.sa_data))
    {
        //This request is not for us
        return;
    }

    //Answer to the network our ip address
    eth_hdr *answer = (eth_hdr *) malloc(ETHERNET_HEADER_LENGTH + ARP_HEADER_LENGTH + ARP_IPV4_LENGTH);
    if(answer == NULL)
    {
        printf("Cannot alloc arp answer \n");
        return;
    }

    //Fill the ethernet frame
    memcpy(answer->dmac, hdr->smac, 6);
    /*memcpy(answer->smac, hdr->dmac, 6);*/
    memcpy(answer->smac, ifr->ifr_hwaddr.sa_data, 6);
    
    answer->ethertype = ARP_HEADER_LENGTH + ARP_IPV4_LENGTH;

    //Fill the arp frame
    arp_hdr *arpAnswer = (arp_hdr *) answer->payload;
    arpAnswer->hwtype  = ARP_ETHERNET; //Ethernet frame
    arpAnswer->protype = ARP_IP; //IP
    arpAnswer->hwsize  = ARP_HWSIZE;
    arpAnswer->prosize = ARP_IPV4_LENGTH;

    //Fill the arp data
    arp_ipv4 *arpData = (arp_ipv4 *)arpAnswer->data;
    memcpy(arpData->smac, ifr->ifr_hwaddr.sa_data, 6);
    memcpy(arpData->dmac, arpIp4->dmac, 6);

    //Give our own ip address
    inet_pton(AF_INET, IF_IP_ADRESS, &arpData->sip);

    arpData->dip = arpIp4->sip;

    printf("Frame prepared\n");

    int written = tun_write((char *) answer,ETHERNET_HEADER_LENGTH + ARP_HEADER_LENGTH + ARP_IPV4_LENGTH );

    if(written > 0)
    {
        printf("Frame successfully sent ! \n");
    }
    else 
    {
        printf("Error while sending frame \n");
    }

    return;
}

void handle_arp_answer(arp_hdr *arpHdr)
{
    printf("Handling arp answer \n");
    return;
}

