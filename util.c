#include <stdint.h>

//Needed because of a strange bug that reverse the two bytes of 
uint16_t ctou16(unsigned char buf[2])
{
    return ((uint16_t) buf[0] << 8 ) + buf[1];
}
