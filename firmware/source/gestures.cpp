#include "MicroBit.h"
#include <cmath>
#define DETECT_THRESHOLD 550
 
MicroBit    uBit;
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
                         
int main() {
                    
    MicroBitImage imgup(5,5,up);
    MicroBitImage imgdown(5,5,down); 
    MicroBitImage imgleft(5,5,left); 
    MicroBitImage imgright(5,5,right);  
    MicroBitImage imgfront(5,5,front); 
    MicroBitImage imgback(5,5,back); 
    uBit.init();                      
    //uBit.display.print(empty_heart);          
    uBit.display.clear();
    
    bool gottasleep = false;
    
    
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_CLICK, setNeutral);
      
    while(1)
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
                    uBit.display.print(imgleft);
                else 
                    uBit.display.print(imgright);
                
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
                    uBit.display.print(imgup);
                else 
                    uBit.display.print(imgdown);              
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
                    uBit.display.print(imgback);
                else 
                    uBit.display.print(imgfront);                 
            } else {
                uBit.display.clear(); 
            }
        }

        
        
        uBit.sleep(100);
        if(gottasleep)
        {   
            gottasleep = false;
            uBit.sleep(500);
        }
    }            
}

