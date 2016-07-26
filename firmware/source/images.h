#ifndef IMAGES_H
#define IMAGES_H

class MicroBitImage;

#define MICROBIT_IMAGE(NAME, DATA...) extern MicroBitImage MICROBIT_IMAGE_##NAME;
#include "imagedata.h"
#undef MICROBIT_IMAGE

#endif // IMAGES_H
