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

//Contains the header of our ethernet requests
typedef struct eth_hdr eth_hdr;

struct eth_hdr
{
    unsigned char dmac[6];
    unsigned char smac[6];

    //If greater than 1536 -> contains the type of the payload 
    //Otherwise it is the size of the payload
    uint16_t ethertype;

    //Must be greater than 48 bytes or it is filled with zeroes
    unsigned char *payload;
} __attribute__((packed));

int tun_alloc(char *dev);
int tun_read(char *buf, int len);

#endif
