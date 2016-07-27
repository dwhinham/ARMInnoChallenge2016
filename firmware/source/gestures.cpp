#include <cmath>
#include <MicroBit.h>

#include "gestures.h"
#include "images.h"

#define DETECT_THRESHOLD    550
#define SMALL_SLEEP         100
#define BIG_SLEEP           900

extern MicroBit uBit;

int neutralX = 0, neutralY = 0, neutralZ = 0;

void setNeutral()
{
    neutralX = uBit.accelerometer.getX();
    neutralY = uBit.accelerometer.getY();
    neutralZ = uBit.accelerometer.getZ();
}

void getGestures(size_t numGestures, gesture_t *gestures)
{
    bool gottaSleep = false;
    unsigned int index = 0;

    uBit.display.clear();
    uBit.display.printChar('?');
    setNeutral();

    uBit.sleep(BIG_SLEEP);

    while (index < numGestures)
    {
        if (uBit.buttonB.isPressed() && index > 0)
        {
            uBit.display.print(MICROBIT_IMAGE_CROSS);
            uBit.sleep(BIG_SLEEP);
            --index;
            continue;
        }

        int gx = uBit.accelerometer.getX() - neutralX;
        int gy = uBit.accelerometer.getY() - neutralY;
        int gz = uBit.accelerometer.getZ() - neutralZ;

        unsigned int agx = abs(gx);
        unsigned int agy = abs(gy);
        unsigned int agz = abs(gz);

        uBit.display.image.clear();

        if (agx >= agy && agx >= agz)
        {
            if (agx > DETECT_THRESHOLD)
            {
                gottaSleep = true;
                if (gx > 0)
                {
                    uBit.display.print(MICROBIT_IMAGE_LEFT);
                    gestures[index] = LEFT;
                }
                else
                {
                    uBit.display.print(MICROBIT_IMAGE_RIGHT);
                    gestures[index] = RIGHT;
                }
                index++;
            }
            else
                uBit.display.clear();
        }
        else if (agy >= agx && agy >= agz)
        {
            if (agy > DETECT_THRESHOLD)
            {
                gottaSleep = true;
                if (gy > 0)
                {
                    uBit.display.print(MICROBIT_IMAGE_UP);
                    gestures[index] = UP;
                }
                else
                {
                    uBit.display.print(MICROBIT_IMAGE_DOWN);
                    gestures[index] = DOWN;
                }
                index++;
            }
            else
                uBit.display.clear();
        }
        else
        {
            if (agz > DETECT_THRESHOLD)
            {
                gottaSleep = true;
                if (gz > 0)
                {
                    uBit.display.print(MICROBIT_IMAGE_BACK);
                    gestures[index] = BACK;
                }
                else
                {
                    uBit.display.print(MICROBIT_IMAGE_FRONT);
                    gestures[index] = FRONT;
                }
                index++;
            }
            else
                uBit.display.clear();
        }

        uBit.sleep(SMALL_SLEEP);
        if (gottaSleep)
        {
            gottaSleep = false;
            uBit.sleep(BIG_SLEEP);
        }
    }

    uBit.display.print(MICROBIT_IMAGE_TICK);
    uBit.sleep(BIG_SLEEP);

    uBit.display.clear();
}
