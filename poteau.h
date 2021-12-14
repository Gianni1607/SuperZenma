#ifndef POTEAU_H
#define POTEAU_H
#include <SDL2/SDL.h>

typedef struct Poteau{
    SDL_Texture *texturePoteau;
    SDL_Rect poteau;
    SDL_Texture *textureDrapeau;
    SDL_Rect drapeau;
    int waitingCount;
    int *xPoteau;
    int *yPoteau;
    int *xDrapeau;
    int *yDrapeau;


} Poteau;

#endif