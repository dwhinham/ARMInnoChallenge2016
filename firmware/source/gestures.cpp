#include "MicroBit.h"
#include <cmath>
#include "gestures.hpp"

#define DETECT_THRESHOLD 550
#define SMALL_SLEEP 100
#define BIG_SLEEP 500

extern MicroBit uBit; 

int neutralX = 0, neutralY = 0, neutralZ = 0;

const uint8_t left[] = {
                      0, 0, 1, 0, 0,
                      0, 1, 0, 0, 0,
                      1, 1, 1, 1, 1,
                      0, 1, 0, 0, 0,
                      0, 0, 1, 0, 0, };
const uint8_t right[] = {
                      0, 0, 1, 0, 0,
                      0, 0, 0, 1, 0,
                      1, 1, 1, 1, 1,
                      0, 0, 0, 1, 0,
                      0, 0, 1, 0, 0, };                          
const uint8_t up[] = {
                      0, 0, 1, 0, 0,
                      0, 1, 1, 1, 0,
                      1, 0, 1, 0, 1,
                      0, 0, 1, 0, 0,
                      0, 0, 1, 0, 0, };              
const uint8_t down[] = {
                      0, 0, 1, 0, 0,
                      0, 0, 1, 0, 0,
                      1, 0, 1, 0, 1,
                      0, 1, 1, 1, 0,
                      0, 0, 1, 0, 0, };     
                      
const uint8_t back[] = {
                      0, 0, 0, 0, 0,
                      0, 0, 0, 0, 0,
                      0, 0, 1, 0, 0,
                      0, 0, 0, 0, 0,
                      0, 0, 0, 0, 0, };                          
const uint8_t front[] = {
                      0, 1, 1, 1, 0,
                      1, 0, 0, 0, 1,
                      1, 0, 1, 0, 1,
                      1, 0, 0, 0, 1,
                      0, 1, 1, 1, 0, };    

void setNeutral(MicroBitEvent)
{
    neutralX = uBit.accelerometer.getX();
    neutralY = uBit.accelerometer.getY();
    neutralZ = uBit.accelerometer.getZ();                             
}
                         
void getGestures(unsigned int number, enum gestures * gestures_array) {
                    
    MicroBitImage imgup(5,5,up);
    MicroBitImage imgdown(5,5,down); 
    MicroBitImage imgleft(5,5,left); 
    MicroBitImage imgright(5,5,right);  
    MicroBitImage imgfront(5,5,front); 
    MicroBitImage imgback(5,5,back); 

    uBit.display.clear();
    
    bool gottasleep = false;
    
    unsigned int index = 0;
    
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_CLICK, setNeutral);
      
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

    uBit.messageBus.ignore(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_CLICK, setNeutral);
}

