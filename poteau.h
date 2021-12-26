#ifndef POTEAU_H
#define POTEAU_H
#include <SDL2/SDL.h>
#include "personnage.h"

typedef struct Poteau{ //définition struct de poteau
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


void collisionPoteau(Perso *mario, Poteau *poteauFin, Mix_Music **musique, Mix_Chunk *son, SDL_Rect rectChateauGauche, bool *continuer, bool *endGame){

    if(SDL_HasIntersection(&(mario->rect), &(poteauFin->poteau)) && !mario->end){
        if(mario->isJumping && *mario->y < 225){
            mario->isWalkingLeft = false;
            mario->isWalkingRight = false;
            mario->isJumping = false;
            mario->end = true;
            run_music(musique, "src/stage_clear.wav", true);
        }
        else if(mario->isWalkingRight){
            *mario->x -= 10;
        }
        else if(mario->isWalkingLeft){
            *mario->x += 10;
        }
    }

    if(mario->end){
        if(poteauFin->waitingCount < 55)
            poteauFin->drapeau.y += 3;
        if(*mario->y < 210)
            *mario->y += 3;
        else if(poteauFin->waitingCount >= 60 && mario->waitingCount == 0){
            mario->jumpCount = 10;
            mario->isJumping = true;
            mario->isWalkingRight = true;
            mario->waitingCount++;
            }

        if(*mario->x > rectChateauGauche.x + rectChateauGauche.w + 40)
            mario->isWalkingRight = false;

        if(poteauFin->waitingCount == 170){
            son = Mix_LoadWAV("src/fireworks.wav");
            Mix_VolumeChunk(son, MIX_MAX_VOLUME/2);
            Mix_PlayChannel(-1, son, 3);
        }
        else if(poteauFin->waitingCount == 230){
            *continuer = false;
            *endGame = true;
        }
        poteauFin->waitingCount++;

    }
}

#endif