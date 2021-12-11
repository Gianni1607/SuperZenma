#ifndef BLOC_H
#define BLOC_H
#include <SDL2/SDL.h>

typedef struct Bloc{
    SDL_Rect fond;
    SDL_Texture *texture;
    int type;
    int *x;
    int *y;

} Bloc;

#endif