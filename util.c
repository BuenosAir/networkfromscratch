#include <stdint.h>
#include <stdio.h>

//Needed because of a strange bug that reverse the two bytes of 
uint16_t ctou16(unsigned char buf[2])
{
    return ((uint16_t) buf[0] << 8 ) + buf[1];
}

void eightBitExchange(uint8_t *a, uint8_t *b)
{
    uint8_t c = *a;
    *a = *b;
    *b = c;
}

int compareMac(unsigned char fmac[16], unsigned char smac[16])
{
    int i;
    for(i = 0; i < 6; i++)
    {
        if(fmac[i] != smac[i])
        {
            return 0;
        }
    }
    return 1;
}

uint16_t fourBitntoh(uint16_t entry)
{
    int i;
    uint16_t output = 0;
    for(i = 0; i < 4; i++)
    {
        printf("%d : ", ((entry >> i) & 0x0001) * (4 - i));
        output += ((entry >> i) & 0x0001) * (4 - i);
    }
    return output;
}

int scanfMacAdress(char *source, char dest[6])
{
    return sscanf(
            source, 
            "%x:%x:%x:%x:%x:%x",
           (unsigned int *) &dest[0], 
           (unsigned int *) &dest[1], 
           (unsigned int *) &dest[2], 
           (unsigned int *) &dest[3], 
           (unsigned int *) &dest[4], 
           (unsigned int *) &dest[5]
            );
}

