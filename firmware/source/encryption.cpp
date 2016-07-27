#include <MicroBit.h>

#include "encryption.h"

int getShift(const gesture_t *gestures, size_t numGestures)
{
    int shift = 0;

    for (size_t i = 0; i < numGestures; i++)
        shift += gestures[i];

    return shift;
}


void encryptString(char *message, size_t len, int shift)
{
    for (size_t i = 0; i < len; i++)
        message[i] += shift;
}

void decryptString(char* message, size_t len, int shift)
{
    for (size_t i = 0; i < len; i++)
        message[i] -= shift;
}
