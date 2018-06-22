#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

//For read/wrte
#include <unistd.h>

//O_RDWR
#include <fcntl.h>
#include <sys/stat.h>


//For tun interfaces creation
#include <linux/if_ether.h>
#include <linux/if_tun.h>

//Provide htons
#include <arpa/inet.h>

//For low level access to linux network devices
#include <sys/ioctl.h>
#include <net/if.h>

#include <net/if_arp.h>

#include <string.h>

#include <errno.h>

#include "debug.h"

#define print_error printf

//Size of the buffer
#define BUFLEN 1600

//File descriptor of our card
int tun_fd;

//Store then network card descriptor as a global variable
struct ifreq ifr;

int frameCounter = 0;

//Read data from the card and put it in buf
int tun_read(char *buf, int len)
{
    return read(tun_fd, buf, len );
}

int tun_write(char *buf, int len)
{
    return write(tun_fd, buf, len);
}

//Allocate the tun card 
int tun_alloc(char *dev)
{
    int fd, err;

    printf("Opening dev/net/tun\n");
    if( (fd = open("/dev/net/tun", O_RDWR)) < 0 ) {
        print_error("Cannot open TUN/TAP dev");
        exit(1);
    }

    //Set 0 all over ifr
    printf("Initializing the structure\n");
    memset(&ifr, 0, sizeof(ifr));

    /* Flags: IFF_TUN   - TUN device (no Ethernet headers)
     *        IFF_TAP   - TAP device
     *
     *        IFF_NO_PI - Do not provide packet information
     */
    ifr.ifr_flags = IFF_TAP | IFF_NO_PI;


    if( *dev ) {
        printf("Setting card name to %s \n", dev);
        strncpy(ifr.ifr_name, dev, IFNAMSIZ);
    }

    printf("Creating card\n");
    if( (err = ioctl(fd, TUNSETIFF, (void *) &ifr)) < 0 ){
        print_error("ERR: Could not ioctl tun: %s\n", strerror(errno));
        close(fd);
        exit(1);
    }

    printf("Copying back card name\n");
    strcpy(dev, ifr.ifr_name);

    //Store the fd as a global variable 
    tun_fd = fd;

    //Get the mac address
    if ((err = ioctl(fd, SIOCGIFHWADDR, &ifr)) < 0)  
    {
        printf("Cannot get mac address of the interface : %s \n", strerror(errno));
        close(fd);
        exit(1);
    }

    char hwaddr[6];
    hwaddr[0] = 0xCA;
    hwaddr[1] = 0xCA;
    hwaddr[2] = 0xCA;
    hwaddr[3] = 0xCA;
    hwaddr[4] = 0xCA;
    hwaddr[5] = 0xCA;

    memcpy(ifr.ifr_hwaddr.sa_data, hwaddr, 6);

    //Set the mac address
   if( (err = ioctl(fd, SIOCSIFHWADDR, &ifr)) < 0 )
   {
       printf("Cannot set mac address : %s\n", strerror(errno));
       close(fd);
       exit(1);
   }

    printf("Mac : ");
    printHexadecimal((unsigned char *) &ifr, sizeof(ifr));
    printf("\n");

    //Ifconfig up the card
    char ifconfig[256];
    sprintf(ifconfig,"ifconfig %s up", ifr.ifr_name);
    system(ifconfig);

    return fd;
}

struct ifreq * getNetworkCard()
{
    return &ifr;
}

int handle_frame()
{
    //Allocate a buffer
    unsigned char *buf = malloc(BUFLEN);
        
    //Read from the card 
    if (tun_read((char *)buf, BUFLEN) < 0) {
        print_error("ERR: Read from tun_fd: %s\n", strerror(errno));
        exit(1);
    }

    printf("\nNew frame %d \n", frameCounter);
    frameCounter++;

    //Convert the frame to ethernet header
    struct eth_hdr *hdr = (struct eth_hdr *) buf;

    //Convert the ethertype to be in the good order
    hdr->ethertype = ntohs(hdr->ethertype);

    printEtherFrame(*hdr);

    switch(hdr->ethertype)
    {
        case ARP_FRAME:
            printf("Handling arp frame\n");
            arp_handling(hdr);
            break;
    }

    return 0;
}


