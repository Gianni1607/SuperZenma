#ifndef PERSONNAGE_H
#define PERSONNAGE_H
#include <SDL2/SDL.h>
#include <stdbool.h>
#include "bloc.h"

typedef struct personnage
{
    SDL_Rect rect;
    int *x;
    int *y;
    bool mort;
    int orientation;
    SDL_Rect *block;
    SDL_Texture *texture;
    int indiceGoomba;
    int indiceBlock;
    int jumpCount;
    int stepCount;
    bool isTouchingBlock;
    bool isJumping;
    bool isWalkingLeft;
    bool isWalkingRight;

} Perso;

Bloc **change_size(Bloc ***tab, int *taille, int indice){
    *taille -= 1;
    Bloc **tab2 = malloc(sizeof(Bloc *) * *taille);
    int j = 0;

    for(int i = 0; i < *taille + 1; i++){
        if(!(i == indice)){
            tab2[j] = (*tab)[i];
            j++;
        }
    }

    return tab2;
}

void Jump(Perso *mario, int *nombreBlocs, Bloc ***tableauBlocs, int *nombreBlocsBriques, SDL_Renderer *renderer){
    int neg;
    int valeur;
    bool endOfJump;
    SDL_Surface *surfaceMario;
    Bloc **tabBlocsTemporaire;

    if(mario->isJumping){
        neg = 1;
        if(mario->jumpCount < 0)
            neg = -1;


        valeur = (pow(mario->jumpCount, 2))*0.5 * neg;
        *mario->y -= valeur;

        //GESTION COLLISIONS

        if(mario->jumpCount < 0){
            if(*mario->y  > 230 && !mario->mort){
                    endOfJump = true;
                    *mario->y = 230;
            }

            for(int i = 0; i < *nombreBlocs; i++){
                if(SDL_HasIntersection(&mario->rect, &(*tableauBlocs)[i]->fond) && !mario->mort){
                    endOfJump = true;
                    *mario->y = *(*tableauBlocs)[i]->y - mario->rect.h;
                    mario->block = &(*tableauBlocs)[i]->fond;
                }
            }
        }
        else{
            for(int i = 0; i < *nombreBlocs; i++){
                if(SDL_HasIntersection(&mario->rect, &((*tableauBlocs)[i]->fond)) && !mario->mort){
                    if(*mario->y <= *(*tableauBlocs)[i]->y + 35 && *mario->y >= *(*tableauBlocs)[i]->y){
                        mario->jumpCount = -1;
                        *mario->y = *(*tableauBlocs)[i]->y + 35;
                        if((*tableauBlocs)[i]->type == 2)
                            mario->indiceBlock = i;
                    }
                }
            }
        }
            
        if(!endOfJump){
            if(!mario->mort){
                if(mario->orientation == 1){
                    SDL_DestroyTexture(mario->texture);
                    surfaceMario = SDL_LoadBMP("src/mario_saut_droit.bmp");
                    mario->texture = SDL_CreateTextureFromSurface(renderer, surfaceMario);
                    SDL_FreeSurface(surfaceMario);
                }
                else if(mario->orientation == -1){
                    SDL_DestroyTexture(mario->texture);
                    surfaceMario = SDL_LoadBMP("src/mario_saut_gauche.bmp");
                    mario->texture = SDL_CreateTextureFromSurface(renderer, surfaceMario);
                    SDL_FreeSurface(surfaceMario);
                }
            }
            else{
                SDL_DestroyTexture(mario->texture);
                surfaceMario = SDL_LoadBMP("src/mario_dead.bmp");
                SDL_DestroyTexture(mario->texture);
                mario->texture = SDL_CreateTextureFromSurface(renderer, surfaceMario);
                SDL_FreeSurface(surfaceMario);
            }

            mario->jumpCount -= 1;

            //Si mario a touché un bloc pierre par le dessous <==> mario->indiceBlock != -1
            if(!(mario->indiceBlock == -1)){
                *nombreBlocsBriques -= 1;
                tabBlocsTemporaire = change_size(tableauBlocs, nombreBlocs, mario->indiceBlock);

                free((*tableauBlocs)[mario->indiceBlock]);
                free(*tableauBlocs);

                *tableauBlocs = malloc(sizeof(Bloc *) * *nombreBlocs);

                for(int i = 0; i < *nombreBlocs; i++){
                    (*tableauBlocs)[i] = tabBlocsTemporaire[i];
                }

                mario->indiceBlock = -1;
            }
        }
    }

    if(endOfJump){
        mario->isJumping = false;
        mario->jumpCount = 9;

        if(mario->orientation == 1){
            SDL_DestroyTexture(mario->texture);
            surfaceMario = SDL_LoadBMP("src/mario_droite.bmp");
            mario->texture = SDL_CreateTextureFromSurface(renderer, surfaceMario);
            SDL_FreeSurface(surfaceMario);
        }
        else if(mario->orientation == -1){
            SDL_DestroyTexture(mario->texture);
            surfaceMario = SDL_LoadBMP("src/mario_gauche.bmp");
            mario->texture = SDL_CreateTextureFromSurface(renderer, surfaceMario);
            SDL_FreeSurface(surfaceMario);
        }

        endOfJump = false;
    }
}

#endif