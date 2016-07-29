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

#ifndef IMAGES_H
#define IMAGES_H

class MicroBitImage;

#define MICROBIT_IMAGE(NAME, DATA...) extern MicroBitImage MICROBIT_IMAGE_##NAME;
#include "imagedata.h"
#undef MICROBIT_IMAGE

#endif // IMAGES_H
