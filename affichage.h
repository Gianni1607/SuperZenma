#ifndef AFFICHAGE_H
#define AFFICHAGE_H
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "personnage.h"
#include "fond.h"
#include "vide.h"
#include "poteau.h"
#include "chateau.h"
#include "goomba.h"
#include "bloc.h"
#include "item.h"

void affichage(SDL_Renderer *renderer,Fond *background, int WINDOW_W, int WINDOW_H, Vide *vide, Poteau *poteauFin, Chateau *chateau, Perso *mario, int nombreGoombas, int nombreBlocs, int nombreItems, Goomba **tableauGoombas, Bloc **tableauBlocs, Item **tableauItems){

    SDL_RenderCopy(renderer, background->texture, NULL, &(background->fond));
    if(*background->x + *background->w < WINDOW_W){
        *background->x += *background->w;
        SDL_RenderCopy(renderer, background->texture, NULL, &(background->fond));
        *background->x -= *background->w;

        if(*background->x + *background->w < 0){
            background->compteur++;
            fprintf(stderr, "Compteur : %i\n", background->compteur);
            *background->x = 0;
        }
    }
    else if(*background->x > 0){
        *background->x -= *background->w;
        SDL_RenderCopy(renderer, background->texture, NULL, &(background->fond));
        *background->x += *background->w;

        if(*background->x > WINDOW_W){
            background->compteur--;
            fprintf(stderr, "Compteur : %i\n", background->compteur);
            *background->x = WINDOW_W - *background->w;
        }
    }

    SDL_RenderCopy(renderer, vide->texture, NULL, &(vide->fond));
    SDL_RenderCopy(renderer, poteauFin->texturePoteau, NULL, &(poteauFin->poteau));
    SDL_RenderCopy(renderer, poteauFin->textureDrapeau, NULL, &(poteauFin->drapeau));
    SDL_RenderCopy(renderer, chateau->textureGauche, NULL, &(chateau->chateauGauche));

    for(int i = 0; i < nombreGoombas; i++)
        SDL_RenderCopy(renderer, tableauGoombas[i]->texture, NULL, &(tableauGoombas[i]->fond));

    for(int i = 0; i < nombreBlocs; i++)
        SDL_RenderCopy(renderer, tableauBlocs[i]->texture, NULL, &(tableauBlocs[i]->fond));

    for(int i = 0; i < nombreItems; i++)
        SDL_RenderCopy(renderer, tableauItems[i]->texture, NULL, &(tableauItems[i]->fond));

    SDL_RenderCopy(renderer, mario->texture, NULL, &mario->fond);

    SDL_RenderCopy(renderer, chateau->textureDroite, NULL, &(chateau->chateauDroit));

    SDL_RenderPresent(renderer);
    SDL_RenderClear(renderer);

}

#endif