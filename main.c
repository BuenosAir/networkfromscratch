#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

//Easier debugging 
#include <errno.h>

#include <string.h>

//For inet_pton
#include <arpa/inet.h>

#include "arp.h"
#include "tuntap.h"

int main(int argc, char **argv)
{
    int ret;
    uint32_t ipAdress;

    //Initialize arp cache
    initializeArpCache();

    initializeEthernet();

    if(argc > 1)
    {
        printf("Using user specified IP\n");
        ret =  inet_pton(AF_INET, argv[1], &ipAdress);
        if(! ret)
        {
            printf("Ip is in wrong format");
            exit(1);
        }
    }
    else 
    {
        ret = inet_pton(AF_INET, DEFAULT_IP_ADDRESS, &ipAdress);
        if( !ret )
        {
            printf("Cannot convert default ip address to uint32_t\n");
            exit(1);
        }
    }
    
    //Convert ipAddress to hardware long
    ipAdress = ntohl(ipAdress);

    setLocalIpAddress(ipAdress);

    char * macAdress = NULL;
    if(argc > 2)
    {
        printf("Using user specified mac address\n");

        macAdress = malloc(sizeof(char) * 6);
        if(macAdress == NULL)
        {
            printf("Cannot malloc macAdress\n");
            exit(1);
        }

        ret = scanfMacAdress(argv[2], macAdress);
        if(ret != 6)
        {
            printf("Mac address is in wrong format\n");
            exit(1);
        }
    }
    else 
    {
        printf("Using a random mac address");
    }
    
    char *cardName = malloc(512);
    ret = tun_alloc(cardName, (unsigned char *) macAdress, ipAdress);

    printf("Created the tun/tap card with name : %s \n", cardName);

    //Go parse a frame 
    while(1)
    {
        handle_frame();
    }
    
    //Free some memory 
    free(macAdress);
    free(cardName);

    return 0;
}



