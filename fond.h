#ifndef FOND_H
#define FOND_H

#include <SDL2/SDL.h>

typedef struct Fond{
    int *x;
    int *y;
    int *w;
    int *h;
    SDL_Texture *texture;
    SDL_Rect fond;
    int compteur;

} Fond;

#endif