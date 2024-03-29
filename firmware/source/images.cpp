/**
 * BBC micro:bit Message Encryption Demo
 * =====================================
 *
 * Entry to the ARM Intern Innovation Challenge 2016
 *
 * by Team Gotta Cache 'Em All:
 *   * Dale Whinham
 *   * Alex Gilday
 *   * Gaetan Bahl
**/

#include <MicroBit.h>

#define MICROBIT_IMAGE(NAME, DATA...) \
    static const uint8_t MICROBIT_IMAGE_DATA_##NAME[] = { DATA }; \
    MicroBitImage MICROBIT_IMAGE_##NAME(5, 5, MICROBIT_IMAGE_DATA_##NAME);
#include "imagedata.h"
#undef MICROBIT_IMAGE
