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


Bloc *initBloc(SDL_Renderer *renderer, int x, int y, int w, int h, int type, char path[255]){
    Bloc *bloc;
    bloc = malloc(sizeof(Bloc));

    bloc->x = &bloc->fond.x;
    bloc->y = &bloc->fond.y;

    SDL_Rect rectBloc = {x, y, w, h};
    bloc->fond = rectBloc;

    //1 = Bloc chance, 2 = bloc brique, 3 = bloc pierre
    bloc->type = type;

    SDL_Surface *surfaceBloc = SDL_LoadBMP(path);
    bloc->texture = SDL_CreateTextureFromSurface(renderer, surfaceBloc);
    SDL_FreeSurface(surfaceBloc);

    return bloc;
}

#endif