#include <stdio.h>
#include "tuntap.h"

void printHexadecimal(char *frame, int length)
{
    int i = 0;
    for(i = 0; i < length; i++)
    {
        printf("%08x ", frame[i]);
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
}
