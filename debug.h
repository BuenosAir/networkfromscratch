#ifndef DEBUG_H
#define DEBUG_H

#include "tuntap.h"

void printHexadecimal(unsigned char *frame, int length);
void printEtherFrame(eth_hdr frame);

#endif 
