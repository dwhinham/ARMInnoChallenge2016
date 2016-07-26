#include "MicroBit.h"
#include <cmath>
#include "gestures.hpp"
#include "images.h"

#define DETECT_THRESHOLD 550
#define SMALL_SLEEP 100
#define BIG_SLEEP 900

extern MicroBit uBit; 

int neutralX = 0, neutralY = 0, neutralZ = 0;

void setNeutral()
{
    neutralX = uBit.accelerometer.getX();
    neutralY = uBit.accelerometer.getY();
    neutralZ = uBit.accelerometer.getZ();                             
}
                         
void getGestures(unsigned int number, enum gestures * gestures_array) {

    uBit.display.clear();
    
    bool gottasleep = false;
    
    unsigned int index = 0;

    uBit.display.printChar('?');
    setNeutral();

    uBit.sleep(BIG_SLEEP);
      
    while(index < number)
    {
        int gx = uBit.accelerometer.getX() - neutralX;
        int gy = uBit.accelerometer.getY() - neutralY;
        int gz = uBit.accelerometer.getZ() - neutralZ;
        
        unsigned int agx = abs(gx);
        unsigned int agy = abs(gy);
        unsigned int agz = abs(gz);
        uBit.display.image.clear();
        if( agx >= agy && agx >= agz)
        {
            if(agx > DETECT_THRESHOLD)
            {
                gottasleep = true;
                if(gx > 0) 
                {
                    uBit.display.print(imgleft);
                    gestures_array[index] = LEFT;
                }
                else 
                {
                    uBit.display.print(imgright);
                    gestures_array[index] = RIGHT;
                }
                index++; 
            } else {
                uBit.display.clear(); 
            }
            
        } 
        else if (agy >= agx && agy >= agz)
        {
             if(agy > DETECT_THRESHOLD)
            {
                gottasleep = true;
                if(gy > 0) 
                {
                    uBit.display.print(imgup);
                    gestures_array[index] = UP;
                }
                else 
                {
                    uBit.display.print(imgdown);              
                    gestures_array[index] = DOWN;
                }
                index++; 
            } else {
                uBit.display.clear(); 
            }
        } 
        else 
        {
            if(agz > DETECT_THRESHOLD)
            {
                gottasleep = true;
                if(gz > 0) 
                {
                    uBit.display.print(imgback);
                    gestures_array[index] = BACK;
                }
                else 
                {
                    uBit.display.print(imgfront);                 
                    gestures_array[index] = FRONT;
                }
                index++; 
            } else {
                uBit.display.clear(); 
            }
        }

        uBit.sleep(SMALL_SLEEP);
        if(gottasleep)
        {   
            gottasleep = false;
            uBit.sleep(BIG_SLEEP);
        }
    }            

    uBit.display.print(imgtick);
    uBit.sleep(BIG_SLEEP);

    uBit.display.clear();
}

