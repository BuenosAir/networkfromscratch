#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

//Easier debugging 
#include <errno.h>

#include <string.h>

#include "arp.h"
#include "tuntap.h"

int main(int argc, char **argv)
{
    char *cardName = malloc(512);
    int ret = tun_alloc(cardName);
    
    printf("Created the tun/tap card with name : %s \n", cardName);
    //Go parse a frame 
    while(1)
    {
        handle_frame();
    }

    return 0;
}



