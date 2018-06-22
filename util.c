#include <stdint.h>

//Needed because of a strange bug that reverse the two bytes of 
uint16_t ctou16(unsigned char buf[2])
{
    return ((uint16_t) buf[0] << 8 ) + buf[1];
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
