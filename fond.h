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

Fond *initFond(SDL_Renderer *renderer, int x, int y, int w, int h, char path[255]){
    Fond *background = malloc(sizeof(Fond));

    SDL_Rect fond = {x, y, w, h};
    background->fond = fond;
    background->x = &background->fond.x;
    background->y = &background->fond.y;
    background->w = &background->fond.w;
    background->h = &background->fond.h;

    SDL_Surface *surfaceFond = SDL_LoadBMP(path);
    background->texture = SDL_CreateTextureFromSurface(renderer, surfaceFond);
    SDL_free(surfaceFond);

    background->compteur = 0;

    return background;
}

#endif