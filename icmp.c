#include <stdint.h>
#include <stdio.h>
//Provide htons
#include <arpa/inet.h>

#include <stdlib.h>
#include <string.h>

#include <linux/if_ether.h>
#include <linux/if_tun.h>
#include <net/if.h>

#include "ip.h"
#include "icmp.h"
#include "debug.h"
#include "tuntap.h"
#include "util.h"

void icmp_handling(ipv4_hdr *ip, eth_hdr *eth)
{
    //Cast the data as icmp header
    icmp_hdr *icmp = (icmp_hdr *) ip->data;

    //Check the type 
    switch(icmp->type)
    {
        case 8:
            printf("Handling ECHO request\n");
            answer_echo(icmp, ip, eth);
            break;
        default :
            printf("Unknow icmp request\n");
            return;
    }

}

void answer_echo(icmp_hdr *icmp, ipv4_hdr *ip, eth_hdr *eth)
{
    printIcmpFrame(icmp);

    struct ifreq *ifr = getNetworkCard();

    //Prepare the ethernet frame
    uint16_t lengthEthernetFrame = ETHERNET_HEADER_LENGTH + IPV4_HEADER_LENGTH + ntohs(ip->len);
    eth_hdr *ether = (eth_hdr *) malloc(lengthEthernetFrame);

    memcpy(ether->dmac, eth->smac, 6);
    memcpy(ether->smac, ifr->ifr_hwaddr.sa_data, 6);

    ether->ethertype  = eth->ethertype;

    ipv4_hdr *ipAnswer = (ipv4_hdr *)ether->payload;
    
    //These two are inversed
    ipAnswer->version        = 5;
    ipAnswer->ihl            = 4;

    ipAnswer->tos            = ip->tos;
    ipAnswer->len            = lengthEthernetFrame - IPV4_HEADER_LENGTH - ETHERNET_HEADER_LENGTH;

    //This is not necessary used when the packet is not fragmented
    ipAnswer->id             = ip->id ;
    ipAnswer->flagsAndOffset = ip->flagsAndOffset;
    ipAnswer->ttl            = 64;
    ipAnswer->proto          = ip->proto;

    ipAnswer->csum           = checksumIP(ipAnswer);

    ipAnswer->saddr          = htonl(getLocalIpAddress());
    ipAnswer->daddr          = ip->saddr;

    icmp_hdr *icmpAnswer = (icmp_hdr *) ipAnswer->data;
    
    icmpAnswer->type = 0;
    icmpAnswer->code = 0;

    uint16_t payloadSize = ntohs(ip->len)  - ICMP_HEADER_LENGTH;
    printf("Payload size : %d \n", payloadSize);
    memcpy(icmpAnswer->data, icmp->data, payloadSize);

    icmpAnswer->csum = 0;

    uint16_t checksum = checksumData((uint8_t *) icmpAnswer, ICMP_HEADER_LENGTH + payloadSize);
    icmpAnswer->csum = checksum;

    printf("Answer is : ");
    printIpv4Frame(ipAnswer);

    //Send the answer
    printf("Sending  response : ");
    printf("Frame length : %d \n", lengthEthernetFrame);
    int written = tun_write((char *)ether, lengthEthernetFrame);
    if(written > 0 )
    {
        printf("Ok \n");
    }
    else 
    {
        printf("Error \n");
    }
}

