#include "encryption.h"
#include "MicroBit.h"



int getShift(enum gestures * array, unsigned int arraylength)
{
    int shift = 0;

    for(unsigned int i = 0; i < arraylength; i++)
    {
        shift += array[i];
    }

    return shift;
}


void encryptString(char * message, unsigned int n, int shift)
{
    for(unsigned int i = 0; i < n; i++)
    {
        message[i] += shift;
    }
}

void decryptString(char * message, unsigned int n, int shift)
{
    for(unsigned int i = 0; i < n; i++)
    {
        message[i] -= shift;
    }
}
