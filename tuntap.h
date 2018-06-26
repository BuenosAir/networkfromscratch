#ifndef TUNTAP_H
#define TUNTAP_H

#include <stdint.h>

//For read/wrte
#include <unistd.h>

//O_RDWR
#include <fcntl.h>
#include <sys/stat.h>


//For tun interfaces creation
#include <linux/if_ether.h>
#include <linux/if_tun.h>

//For low level access to linux network devices
#include <sys/ioctl.h>
#include <net/if.h>

#include "util.h"

#define ARP_FRAME 0x0806
#define IPV6_FRAME 0x86DD

#define ETHERNET_HEADER_LENGTH (6 * 2  + 2)

//Our ip address
#define DEFAULT_IP_ADDRESS "10.0.0.5"

//Contains the header of our ethernet requests
struct eth_hdr
{
    unsigned char dmac[6];
    unsigned char smac[6];

    //If greater than 1536 -> contains the type of the payload 
    //Otherwise it is the size of the payload
    uint16_t ethertype;

    //Must be greater than 48 bytes or it is filled with zeroes
    unsigned char payload[];
} __attribute__((packed));
typedef struct eth_hdr eth_hdr;

int tun_alloc(char *dev, unsigned char *macAdress, uint32_t ipAdress);
int tun_read(char *buf, int len);
int tun_write(char *buf, int len);
int handle_frame();
void setLocalIpAddress(uint32_t ip);
uint32_t getLocalIpAddress();
struct ifreq * getNetworkCard();

#endif
