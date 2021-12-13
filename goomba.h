#ifndef GOOMBA_H
#define GOOMBA_H
#include <SDL2/SDL.h>

typedef struct Goomba{
    SDL_Rect fond;
    SDL_Texture *texture;
    int orientation;
    int *x;
    int *y;

} Goomba;

#endif