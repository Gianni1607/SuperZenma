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


Chateau *initChateau(SDL_Renderer *renderer, int x, int y, int w, int h, char pathGauche[255], char pathDroit[255]){
    Chateau *chateau = malloc(sizeof(Chateau));
    chateau->xGauche = &chateau->chateauGauche.x;
    chateau->xDroit = &chateau->chateauDroit.x;

    SDL_Rect rectChateauGauche = {x, y, w, h};
    chateau->chateauGauche = rectChateauGauche;

    SDL_Rect rectChateauDroit = {x + 100, y, w, h};
    chateau->chateauDroit = rectChateauDroit;

    SDL_Surface *surfaceChateauGauche = SDL_LoadBMP(pathGauche);
    chateau->textureGauche = SDL_CreateTextureFromSurface(renderer, surfaceChateauGauche);
    SDL_FreeSurface(surfaceChateauGauche);
    
    SDL_Surface *surfaceChateauDroit = SDL_LoadBMP(pathDroit);
    chateau->textureDroite = SDL_CreateTextureFromSurface(renderer, surfaceChateauDroit);
    SDL_FreeSurface(surfaceChateauDroit);

    return chateau;
}

#endif