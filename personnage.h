#ifndef PERSONNAGE_H
#define PERSONNAGE_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

typedef struct personnage{
    SDL_Rect fond;
    int *x;
    int *y;
    bool mort;
    bool end;
    int orientation;
    SDL_Rect rectNull;
    SDL_Rect *block;
    SDL_Texture *texture;
    int waitingCount;
    int indiceGoomba;
    int indiceBlock;
    int jumpCount;
    int stepCount;
    bool isTouchingBlock;
    bool isJumping;
    bool isWalkingLeft;
    bool isWalkingRight;

} Perso;

#include <stdbool.h>
#include "goomba.h"
#include "poteau.h"
#include "fond.h"
#include "chateau.h"
#include "vide.h"
#include "bloc.h"
#include "item.h"

Perso *initPerso(SDL_Renderer *renderer, int x, int y, int w, int h, char path[255]){
    Perso *mario = malloc(sizeof(Perso));

    mario->x = &mario->fond.x;
    mario->y = &mario->fond.y;

    SDL_Surface *surfaceMario = SDL_LoadBMP(path);
    mario->texture = SDL_CreateTextureFromSurface(renderer, surfaceMario);
    SDL_free(surfaceMario);

    SDL_Rect rectMario = {x, y, w, h};
    mario->fond = rectMario;

    mario->mort = false;
    mario->end = false;
    mario->orientation = 1;
    SDL_Rect rectNull = {-1, -1, -1, -1}; //Rect qui sert de bloc par défaut quand mario n'est pas sur un bloc (chance, brique, etc.) ; sol != bloc
    mario->rectNull = rectNull;
    mario->block = &rectNull;
    mario->waitingCount = 0;
    mario->indiceGoomba = -1;
    mario->indiceBlock = -1;
    mario->stepCount = 0;
    mario->jumpCount = 9;
    mario->isTouchingBlock = false;
    mario->isJumping = false;
    mario->isWalkingLeft = false;
    mario->isWalkingRight = false;

    return mario;
}


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


void isFallingOffTheBlock(Perso *mario){
    if(!(mario->block->x == -1)){ 
        if(!(*mario->x <= mario->block->x + 35 && *mario->x + 50 >= mario->block->x)){ //Et si mario est dehors de la colonne qui contient le bloc
            if(mario->isJumping == false){ //Et qu'il n'est pas en train de sauter
                mario->isJumping = true; //alors il saute
                mario->jumpCount = -2; //négativement <=> tomber
            }
            mario->block = &mario->rectNull; //Et on rénitialise le bloc sur lequel était mario
        }
    }
}


void moveRight(SDL_Renderer *renderer, Perso *mario, Fond *background, Vide *vide, Poteau *poteauFin, Chateau *chateau, Bloc **tableauBlocs, Goomba **tableauGoombas, int nombreBlocs, int nombreGoombas){
    *mario->x += 10; //il bouge de 10 pixels

    mario->isTouchingBlock = false;
    for(int i = 0; i < nombreBlocs; i++){
        if(SDL_HasIntersection(&mario->fond, &(tableauBlocs[i]->fond))){ //Si interaction avec un quelconque bloc du jeu :
            *mario->x -= 10; //Mario ne bouge pas
            mario->isTouchingBlock = true; 
            break;
        }
    }

    if(!(mario->isTouchingBlock)){ //Si mario peut bouger
        *mario->x -= 10;
        if(*mario->x == 500){ //On fait bouger tous les éléments de la map
            *background->x -= 10;
            *vide->x -= 10;
            *poteauFin->xPoteau -= 10;
            *poteauFin->xDrapeau -= 10;
            *chateau->xGauche -= 10;
            *chateau->xDroit -= 10;

            for(int i = 0; i < nombreGoombas; i++)
                *tableauGoombas[i]->x -= 10;

            for(int i = 0; i < nombreBlocs; i++)
                *tableauBlocs[i]->x -= 10;
        }
        else{
            if(*mario->x < 750)
                *mario->x += 10;
        }

        mario->orientation = 1;

        if(mario->isJumping == false){ //Si mario ne saute pas, animation de pas
            if(mario->stepCount%4 == 0 || mario->stepCount%4 == 1){
                SDL_DestroyTexture(mario->texture);
                SDL_Surface *surfaceMario = SDL_LoadBMP("src/mario_droite.bmp");
                mario->texture = SDL_CreateTextureFromSurface(renderer, surfaceMario);
                SDL_FreeSurface(surfaceMario);
                mario->stepCount++;
            }
            else if(mario->stepCount%4 == 2 || mario->stepCount%4 == 3){
                SDL_DestroyTexture(mario->texture);
                SDL_Surface *surfaceMario = SDL_LoadBMP("src/mario_droite1.bmp");
                mario->texture = SDL_CreateTextureFromSurface(renderer, surfaceMario);
                SDL_FreeSurface(surfaceMario);
                mario->stepCount++;
            }
        }
    }
}


void moveLeft(SDL_Renderer *renderer, Perso *mario, Fond *background, Vide *vide, Poteau *poteauFin, Chateau *chateau, Bloc **tableauBlocs, Goomba **tableauGoombas, int nombreBlocs, int nombreGoombas){
    *mario->x -= 10;

    mario->isTouchingBlock = false;
    for(int i = 0; i < nombreBlocs; i++){
        if(SDL_HasIntersection(&mario->fond, &(tableauBlocs[i]->fond))){
            *mario->x += 10;
            mario->isTouchingBlock = true;
            break;
        }
    }

    if(!(mario->isTouchingBlock)){
        *mario->x += 10;

        if(*mario->x == 300){
            if(!(background->compteur == 0) || *background->x < 0){
                *background->x += 10;
                *vide->x += 10;
                *poteauFin->xPoteau += 10;
                *poteauFin->xDrapeau += 10;
                *chateau->xGauche += 10;
                *chateau->xDroit += 10;

                for(int i = 0; i < nombreGoombas; i++)
                    *tableauGoombas[i]->x += 10;

                for(int i = 0; i <  nombreBlocs; i++)
                    *tableauBlocs[i]->x += 10;
            }
            else if(*mario->x > 0)
                *mario->x -= 10;
        }
        else{
            if(*mario->x > 0)
                *mario->x -= 10;
        }
        
        mario->orientation = -1;

        if(mario->isJumping == false){
            if (mario->stepCount%2 == 0)
            {
                SDL_DestroyTexture(mario->texture);
                SDL_Surface *surfaceMario = SDL_LoadBMP("src/mario_gauche.bmp");
                mario->texture = SDL_CreateTextureFromSurface(renderer, surfaceMario);
                SDL_FreeSurface(surfaceMario);
                mario->stepCount++;
            }
            else if (mario->stepCount%2 == 1)
            {
                SDL_DestroyTexture(mario->texture);
                SDL_Surface *surfaceMario = SDL_LoadBMP("src/mario_gauche1.bmp");
                mario->texture = SDL_CreateTextureFromSurface(renderer, surfaceMario);
                SDL_FreeSurface(surfaceMario);
                mario->stepCount++;
            }
        }
    }
}


void jump(Perso *mario, int *nombreBlocs, Bloc ***tableauBlocs, int *nombreBlocsBriques, SDL_Renderer *renderer, int *nbr, Item ***tableauItems){
    int neg;
    int valeur;
    bool endOfJump;
    SDL_Surface *surfaceMario;
    Bloc **tabBlocsTemporaire;


    //neg sert à definir si le saut est positif ou négatif <=> chute
    neg = 1; 
    if(mario->jumpCount < 0) 
        neg = -1;


    valeur = (pow(mario->jumpCount, 2))*0.5 * neg; //calcul à partir de formule chute libre
    *mario->y -= valeur; //on applique la translation à mario sur l'axe y

    //GESTION COLLISIONS

    if(mario->jumpCount < 0){ //si mario tombe
        if(*mario->y  > 230 && !mario->mort){ //et qu'il est au dessus du niveau du sol et qu'il n'est pas mort
                endOfJump = true; //fin du saut
                *mario->y = 230; //on place mario au niveau du sol
        }

        for(int i = 0; i < *nombreBlocs; i++){
            if(SDL_HasIntersection(&mario->fond, &(*tableauBlocs)[i]->fond) && !mario->mort){ //Si mario entre en contact avec un bloc et qu'il nest pas mort
                endOfJump = true; //fin du saut
                *mario->y = *(*tableauBlocs)[i]->y - mario->fond.h; //on place mario sur le bloc
                mario->block = &(*tableauBlocs)[i]->fond; //on stocke le bloc sur lequel mario est posé
            }
        }
    }
    else{ //s'il ne tombe pas <=> il monte
        for(int i = 0; i < *nombreBlocs; i++){
            if(SDL_HasIntersection(&mario->fond, &((*tableauBlocs)[i]->fond)) && !mario->mort){ //et qu'il touche un bloc
                if(*mario->y <= *(*tableauBlocs)[i]->y + 35 && *mario->y >= *(*tableauBlocs)[i]->y){
                    mario->jumpCount = -1; //on le fait faire un saut négatif <=> chute
                    *mario->y = *(*tableauBlocs)[i]->y + 35; //on le place juste en dessous du bloc
                    if((*tableauBlocs)[i]->type == 1){
                        SDL_DestroyTexture((*tableauBlocs)[i]->texture);
                        SDL_Surface *surfaceBlocChanceVide = SDL_LoadBMP("src/bloc_chance_vide.bmp");
                        (*tableauBlocs)[i]->texture = SDL_CreateTextureFromSurface(renderer, surfaceBlocChanceVide);
                        SDL_FreeSurface(surfaceBlocChanceVide);

                        /*
                        *nbr += 1;
                        Item *champignon = malloc(sizeof(Item));

                        SDL_Surface *surfaceChampignon = SDL_LoadBMP("src/champignon.bmp");
                        champignon->texture = SDL_CreateTextureFromSurface(renderer, surfaceChampignon);
                        SDL_FreeSurface(surfaceChampignon);

                        SDL_Rect fondChampignon = {*(*tableauBlocs)[i]->x, *(*tableauBlocs)[i]->y, 35, 35};
                        champignon->fond = fondChampignon;

                        champignon->x = &champignon->fond.x;
                        champignon->y = &champignon->fond.y;

                        *tableauItems[0] = champignon;
                        */

                    }
                    else if((*tableauBlocs)[i]->type == 2) //si le bloc touché est un bloc de type 2 <=> cassable
                        mario->indiceBlock = i; //on stocke le bloc touché
                }
            }
        }
    }
        
    if(!endOfJump){ //si ce n'est pas la fin du saut
        if(!mario->mort){ //et que mario n'est pas mort
            if(mario->orientation == 1){ //si mario est tourné à droite
                SDL_DestroyTexture(mario->texture);
                surfaceMario = SDL_LoadBMP("src/mario_saut_droit.bmp");
                mario->texture = SDL_CreateTextureFromSurface(renderer, surfaceMario);
                SDL_FreeSurface(surfaceMario);
            }
            else if(mario->orientation == -1){ //s'il est tourné à gauche
                SDL_DestroyTexture(mario->texture);
                surfaceMario = SDL_LoadBMP("src/mario_saut_gauche.bmp");
                mario->texture = SDL_CreateTextureFromSurface(renderer, surfaceMario);
                SDL_FreeSurface(surfaceMario);
            }
        }
        else{ //si mario est mort
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
            *tableauBlocs = realloc(*tableauBlocs, sizeof(Bloc *) * *nombreBlocs);

            for(int i = 0; i < *nombreBlocs; i++)
                (*tableauBlocs)[i] = tabBlocsTemporaire[i];

            mario->indiceBlock = -1;
        }
    }
    else if(endOfJump){
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

void marioDies(Perso *mario, Mix_Music **musique, bool *continuer, bool *game_over){
    if(mario->waitingCount == 0)
        run_music(musique, "src/mario_dead.wav", true);
    mario->waitingCount++;
    if(mario->waitingCount == 90){
        *continuer = false;
        *game_over = true;
    }
}


bool isMarioFallingInTheVoid(Perso *mario, Vide *vide){
    if(SDL_HasIntersection(&(mario->fond), &(vide->fond)) && !mario->mort && !mario->end) //Si mario entre en contact avec le fondBleu <=> vide et qu'il n'est pas déjà mort et qu'il n'a pas gagné le jeu
        return true;
    return false;
}


void isMarioHittingAGoomba(SDL_Renderer *renderer, Perso *mario, Goomba **tableauGoombas, int nombreGoombas, Mix_Chunk *son){
    Goomba *goomba;

    if(!mario->mort && !mario->end){ //Si mario n'est pas mort et qu'il n'a pas fini le jeu
        for(int i = 0; i < nombreGoombas; i++){
            goomba = tableauGoombas[i];
            if(SDL_HasIntersection(&(mario->fond), &(goomba->fond)) && !(goomba->mort)){ //et qu'il rentre en contact avec un goomba vivant
                if(mario->isJumping && mario->jumpCount < 0){ //si mario saute négativement <=> tombe
                    mario->jumpCount = 5; //il fait un petit saut
                    goomba->mort = true; //le goomba meurt
                    son = Mix_LoadWAV("src/kick.wav"); //et on joue un son
                    Mix_VolumeChunk(son, MIX_MAX_VOLUME);
                    Mix_PlayChannel(-1, son, 0);

                    SDL_DestroyTexture(tableauGoombas[i]->texture);
                    SDL_Surface *surfaceGoomba = SDL_LoadBMP("src/goomba_mort.bmp"); //on charge l'image du goomba écrasé
                    tableauGoombas[i]->texture = SDL_CreateTextureFromSurface(renderer, surfaceGoomba);
                    SDL_FreeSurface(surfaceGoomba);
                    goomba->fond.h = 20;
                    *(goomba->y) += 20;
                }
                else{ //sinon mario meurt
                    mario->isJumping = true;
                    mario->mort = true;
                }
            }
        }
    }
}

#endif