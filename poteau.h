#ifndef POTEAU_H
#define POTEAU_H
#include <SDL2/SDL.h>

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

#include "personnage.h"

Poteau *initPoteau(SDL_Renderer *renderer, int x, int y, int w, int h, char pathPoteau[255], char pathDrapeau[255]){
    Poteau *poteauFin = malloc(sizeof(Poteau));
    poteauFin->xPoteau = &poteauFin->poteau.x;
    poteauFin->yPoteau = &poteauFin->poteau.y;
    poteauFin->xDrapeau = &poteauFin->drapeau.x;
    poteauFin->yDrapeau = &poteauFin->drapeau.y;

    SDL_Rect poteau = {x, y, w, h};
    SDL_Rect drapeau = {x - 25, y + 15, (w / 30) * 40, (h / 240) * 35};
    poteauFin->poteau = poteau;
    poteauFin->drapeau = drapeau;

    SDL_Surface *surfacePoteau = SDL_LoadBMP(pathPoteau);
    poteauFin->texturePoteau = SDL_CreateTextureFromSurface(renderer, surfacePoteau);
    SDL_FreeSurface(surfacePoteau);

    SDL_Surface *surfaceDrapeau = SDL_LoadBMP(pathDrapeau);
    poteauFin->textureDrapeau = SDL_CreateTextureFromSurface(renderer, surfaceDrapeau);
    SDL_FreeSurface(surfaceDrapeau);

    return poteauFin;
}


void collisionPoteau(Perso *mario, Poteau *poteauFin, Mix_Music **musique, Mix_Chunk *son, SDL_Rect rectChateauGauche, bool *continuer, bool *endGame){

    if(SDL_HasIntersection(&(mario->fond), &(poteauFin->poteau)) && !mario->end){
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