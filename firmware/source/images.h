#ifndef __IMAGESH__
#define __IMAGESH__

#include "MicroBit.h"

const uint8_t UBIT_IMAGE_ARRAY_left[] = {
                      0, 0, 1, 0, 0,
                      0, 1, 0, 0, 0,
                      1, 1, 1, 1, 1,
                      0, 1, 0, 0, 0,
                      0, 0, 1, 0, 0, };
const uint8_t UBIT_IMAGE_ARRAY_right[] = {
                      0, 0, 1, 0, 0,
                      0, 0, 0, 1, 0,
                      1, 1, 1, 1, 1,
                      0, 0, 0, 1, 0,
                      0, 0, 1, 0, 0, };                          
const uint8_t UBIT_IMAGE_ARRAY_up[] = {
                      0, 0, 1, 0, 0,
                      0, 1, 1, 1, 0,
                      1, 0, 1, 0, 1,
                      0, 0, 1, 0, 0,
                      0, 0, 1, 0, 0, };              
const uint8_t UBIT_IMAGE_ARRAY_down[] = {
                      0, 0, 1, 0, 0,
                      0, 0, 1, 0, 0,
                      1, 0, 1, 0, 1,
                      0, 1, 1, 1, 0,
                      0, 0, 1, 0, 0, };     
                      
const uint8_t UBIT_IMAGE_ARRAY_back[] = {
                      0, 0, 0, 0, 0,
                      0, 0, 0, 0, 0,
                      0, 0, 1, 0, 0,
                      0, 0, 0, 0, 0,
                      0, 0, 0, 0, 0, };                          
const uint8_t UBIT_IMAGE_ARRAY_front[] = {
                      0, 1, 1, 1, 0,
                      1, 0, 0, 0, 1,
                      1, 0, 1, 0, 1,
                      1, 0, 0, 0, 1,
                      0, 1, 1, 1, 0, };    

const uint8_t UBIT_IMAGE_ARRAY_tick[] = {
                      0, 0, 0, 0, 0,
                      0, 0, 0, 0, 1,
                      0, 0, 0, 1, 0,
                      1, 0, 1, 0, 0,
                      0, 1, 0, 0, 0, };    

const uint8_t UBIT_IMAGE_ARRAY_key[] = {
                      0, 0, 0, 0, 0,
                      0, 1, 0, 0, 0,
                      1, 0, 1, 1, 1,
                      0, 1, 0, 0, 1,
                      0, 0, 0, 0, 0, };    
                
MicroBitImage imgup(5,5,UBIT_IMAGE_ARRAY_up);
MicroBitImage imgdown(5,5,UBIT_IMAGE_ARRAY_down); 
MicroBitImage imgleft(5,5,UBIT_IMAGE_ARRAY_left); 
MicroBitImage imgright(5,5,UBIT_IMAGE_ARRAY_right);  
MicroBitImage imgfront(5,5,UBIT_IMAGE_ARRAY_front); 
MicroBitImage imgback(5,5,UBIT_IMAGE_ARRAY_back); 
MicroBitImage imgtick(5,5,UBIT_IMAGE_ARRAY_tick); 
MicroBitImage imgkey(5,5,UBIT_IMAGE_ARRAY_key); 




#endif
