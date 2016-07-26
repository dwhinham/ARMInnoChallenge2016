#ifndef __ENCRYPTIONH_
#define __ENCRYPTIONH_

#include "gestures.hpp"
#include "MicroBit.h"

int getShift(enum gestures * array, unsigned int arraylength);

void encryptString(char * message, unsigned int n, int shift);
void decryptString(unsigned char * message, unsigned int n, int shift);


#endif
