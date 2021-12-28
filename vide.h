#ifndef VIDE_H
#define VIDE_H
#include <SDL2/SDL.h>

typedef struct Vide{
    int *x;
    int *y;
    SDL_Rect fond;
    SDL_Texture *texture;

} Vide;


Vide *initVide(SDL_Renderer *renderer, int x, int y, int w, int h, char path[255]){
    Vide *vide = malloc(sizeof(Vide));

    vide->x = &vide->fond.x;
    vide->y = &vide->fond.y;

    SDL_Rect rectVide = {1740, 240, 220, 70};
    vide->fond = rectVide;

    SDL_Surface *surfaceVide = SDL_LoadBMP(path);
    vide->texture = SDL_CreateTextureFromSurface(renderer, surfaceVide);
    SDL_FreeSurface(surfaceVide);

    return vide;
}

#endif