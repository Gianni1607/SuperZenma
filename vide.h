#ifndef VIDE_H
#define VIDE_H
#include <SDL2/SDL.h>

typedef struct Vide{
    int *x;
    int *y;
    SDL_Rect fond;
    SDL_Texture *texture;

} Vide;

#endif