#include <stdint.h>
#include <stdio.h>
//Provide htons
#include <arpa/inet.h>

#include <stdlib.h>
#include <string.h>

#include "tuntap.h"
#include "arp.h"
#include "debug.h"

#define IP_DECIMAL 167772165 //10.0.0.5
#define MAX_ARP_ENTRY 256

arp_entry arpCache[MAX_ARP_ENTRY];


void initializeArpCache()
{
    printf("Nulling arpCache \n");
    memset(arpCache, 0, sizeof(arp_entry) * MAX_ARP_ENTRY);
}

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
    uint32_t localIpAddress = getLocalIpAddress();
    if(ntohl(arpIp4->dip) != localIpAddress)
    {
        printf("This is not for us\n");
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
    //TODO: check 
    memcpy(answer->smac, ifr->ifr_hwaddr.sa_data, 6);
    
    answer->ethertype = htons(ARP_FRAME);

    //Fill the arp frame
    arp_hdr *arpAnswer = (arp_hdr *) answer->payload;
    arpAnswer->hwtype  = htons(ARP_ETHERNET); 
    arpAnswer->protype = htons(ARP_IP); 
    arpAnswer->hwsize  = ARP_HWSIZE; 
    arpAnswer->prosize = ARP_PROSIZE;
    arpAnswer->opcode = htons(2); //Opcode for answer
    
    //Fill the arp data
    arp_ipv4 *arpData = (arp_ipv4 *)arpAnswer->data;
    memcpy(arpData->smac, ifr->ifr_hwaddr.sa_data, 6);
    memcpy(arpData->dmac, arpIp4->smac, 6);

    //Give our own ip address
    /*inet_pton(AF_INET, DEFAULT_IP_ADDRESS, &arpData->sip);*/
    arpData->sip = htonl(getLocalIpAddress()); 

    arpData->dip = arpIp4->sip;

    printf("Sending : ");

    int written = tun_write((char *) answer,ETHERNET_HEADER_LENGTH + ARP_HEADER_LENGTH + ARP_IPV4_LENGTH );

    if(written > 0)
    {
        printf("OK\n");
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

    arp_ipv4 *arpIp4 = (arp_ipv4 *)arpHdr->data;

    printArpRequest(*arpIp4);
    
    int ipPosition = searchIpInCache(ntohl(arpIp4->sip));
    if(ipPosition)
    {
        printf("Arp cache is full, exiting\n");
        exit(1);
    }
    
    //Update/Insert the entry in cache 
    arpCache[ipPosition].ip = ntohl(arpIp4->sip);

    //TODO TODO TODO If it crash, it comes from here TODO TODO TODO
    strncpy((char *) arpCache[ipPosition].mac, ( char *)arpIp4->smac, 6);

    printf("Entry updated for %s at position %d\n", arpCache[ipPosition].mac, ipPosition);
    return;
}

//Not efficient, but that's not the point of this programm
int searchIpInCache(uint32_t ip)
{
    int i = 0;
    for(i = 0; i < MAX_ARP_ENTRY; i++)
    {
        if(arpCache[i].ip == 0)
        {
            return i;
        }
        else if(arpCache[i].ip == ip)
        {
            return i;
        }
    }
    
    return -1;
}

