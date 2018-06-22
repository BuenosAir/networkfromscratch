#ifndef UTIL_H
#define  UTIL_H

//Convert uchar[2] to uint16_t
uint16_t ctou16(unsigned char buf[2]);
int compareMac(unsigned char fmac[16], unsigned char smac[16]);
int scanfMacAdress(char *source, char dest[6]);

#endif
