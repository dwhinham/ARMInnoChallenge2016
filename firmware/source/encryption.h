#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <MicroBit.h>

#include "gestures.h"

int getShift(const gesture_t *gestures, size_t numGestures);

void encryptString(char* message, size_t len, int shift);
void decryptString(char* message, size_t len, int shift);

#endif // ENCRYPTION_H
