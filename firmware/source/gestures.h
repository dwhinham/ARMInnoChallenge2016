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
