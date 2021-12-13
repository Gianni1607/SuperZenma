#ifndef GOOMBA_H
#define GOOMBA_H
#include <SDL2/SDL.h>

typedef struct Goomba{
    SDL_Rect fond;
    SDL_Texture *texture;
    bool mort;
    int waitingCount;
    int stepCount;
    int orientation;
    int *x;
    int *y;

} Goomba;

#endif