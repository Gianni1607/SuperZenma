#ifndef CHATEAU_H
#define CHATEAU_H

#include <SDL2/SDL.h>

typedef struct Chateau{
    int *xGauche;
    int *xDroit;
    SDL_Rect chateauGauche;
    SDL_Texture *textureGauche;
    SDL_Rect chateauDroit;
    SDL_Texture *textureDroite;

} Chateau;

#endif