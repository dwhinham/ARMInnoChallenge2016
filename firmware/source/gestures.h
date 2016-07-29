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

#ifndef GESTURES_H
#define GESTURES_H

typedef enum {
    LEFT,
    RIGHT,
    FRONT,
    BACK,
    UP,
    DOWN
} gesture_t;

void getGestures(unsigned int numGestures, gesture_t *gestures);

#endif // GESTURES_H
