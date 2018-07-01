#ifndef UTIL_H
#define  UTIL_H

//Convert uchar[2] to uint16_t
uint16_t ctou16(unsigned char buf[2]);
int compareMac(unsigned char fmac[16], unsigned char smac[16]);
int scanfMacAdress(char *source, char dest[6]);
uint16_t fourBitntoh(uint16_t entry);
void eightBitExchange(uint8_t *a, uint8_t *b);
uint16_t checksumData(void *data, int length);

#endif
