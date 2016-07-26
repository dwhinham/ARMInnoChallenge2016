#ifndef __GESTURESH__
#define __GESTURESH__

enum gestures {
    LEFT,
    RIGHT,
    FRONT,
    BACK,
    UP,
    DOWN
};


void getGestures(unsigned int number, enum gestures * gestures_array);



#endif
