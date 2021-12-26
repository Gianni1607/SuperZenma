#ifndef GOOMBA_H
#define GOOMBA_H
#include <SDL2/SDL.h>
#include "bloc.h"

typedef struct Goomba{ //définition struct de goomba
    SDL_Rect fond;
    SDL_Texture *texture;
    bool mort;
    int waitingCount;
    int stepCount;
    int orientation;
    int *x;
    int *y;

} Goomba;

Goomba **change_size2(Goomba ***tab, int *taille, int indice){
    *taille -= 1;
    Goomba **tab2 = malloc(sizeof(Goomba *) * *taille);
    int j = 0;

    for(int i = 0; i < *taille + 1; i++){
        if(!(i == indice)){
            tab2[j] = (*tab)[i];
            j++;
        }
    }

    return tab2;
}


void moveGoomba(Goomba ***tableauGoombas, Bloc ***tableauBlocs, int *nombreGoombas, Perso *mario, Poteau *poteauFin, SDL_Renderer *renderer, SDL_Rect fond, Mix_Chunk *son, int *nombreBlocsPierres, int *nombreBlocsChances, int *nombreBlocsBriques){
    Goomba **tabGoombasTemporaire;

    for(int i = 0; i < *nombreGoombas; i++){
        if(!(*tableauGoombas)[i]->mort){
            *(*tableauGoombas)[i]->x += (*tableauGoombas)[i]->orientation * 3;
            if((*tableauGoombas)[i]->stepCount%4 == 0 || (*tableauGoombas)[i]->stepCount%4 == 1){
                SDL_DestroyTexture((*tableauGoombas)[i]->texture);
                SDL_Surface *surfaceGoomba = SDL_LoadBMP("src/goomba_gauche.bmp");
                (*tableauGoombas)[i]->texture = SDL_CreateTextureFromSurface(renderer, surfaceGoomba);
                SDL_FreeSurface(surfaceGoomba);
                (*tableauGoombas)[i]->stepCount++;
            }
            else if((*tableauGoombas)[i]->stepCount%4 == 2 || (*tableauGoombas)[i]->stepCount%4 == 3){
            SDL_DestroyTexture((*tableauGoombas)[i]->texture);
            SDL_Surface *surfaceGoomba = SDL_LoadBMP("src/goomba_droit.bmp");
            (*tableauGoombas)[i]->texture = SDL_CreateTextureFromSurface(renderer, surfaceGoomba);
            SDL_FreeSurface(surfaceGoomba);
            (*tableauGoombas)[i]->stepCount++;
            }
        }
        else{
            (*tableauGoombas)[i]->waitingCount++;
            if((*tableauGoombas)[i]->waitingCount == 20){
                mario->indiceGoomba = i;
                Mix_FreeChunk(son);
            }
        }

        for(int j = 0; j < *nombreBlocsPierres; j++){
            if(SDL_HasIntersection(&((*tableauGoombas)[i]->fond), &((*tableauBlocs)[j + *nombreBlocsChances + *nombreBlocsBriques]->fond))){
                (*tableauGoombas)[i]->orientation *= -1;
            }
            else if(*(*tableauGoombas)[i]->x < fond.x){
                (*tableauGoombas)[i]->orientation *= -1;
                *(*tableauGoombas)[i]->x += (*tableauGoombas)[i]->orientation * 3;
            }
            else if(SDL_HasIntersection((&(*tableauGoombas)[i]->fond), &poteauFin->poteau)){
                (*tableauGoombas)[i]->orientation *= -1;
                *(*tableauGoombas)[i]->x += (*tableauGoombas)[i]->orientation * 3;
            }
        }
    }

    if(!(mario->indiceGoomba == -1)){
        tabGoombasTemporaire = change_size2(tableauGoombas, &(*nombreGoombas), mario->indiceGoomba);

        free((*tableauGoombas)[mario->indiceGoomba]);
        *tableauGoombas = realloc(*tableauGoombas, sizeof(Goomba *) * *nombreGoombas);

        for(int i = 0; i < *nombreGoombas; i++)
            (*tableauGoombas)[i] = tabGoombasTemporaire[i];

        mario->indiceGoomba = -1;

    }

}

#endif