#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <SDL2/SDL.h>
#include "personnage.h"
#include "bloc.h"

Bloc **change_size(Bloc **tab, int *taille, int indice){
    *taille -= 1;
    Bloc **tab2 = malloc(sizeof(Bloc *) * *taille);
    int j = 0;

    for(int i = 0; i < *taille + 1; i++){
        if(!(i == indice)){
            tab2[j] = tab[i];
            j++;
        }
    }

    return tab2;
}

int main(int argc, char** argv){

    SDL_Window *window = NULL;

    int valeur;
    Bloc **tabTemporaire;
    bool endOfJump = false;
    int neg = 1;
    int j = 0;
    int continuer = 1;
    int nextYPosition;
    int nombreBlocsChances = 6;
    int nombreBlocsBriques = 3;
    int nombreBlocsPierres = 20;
    int nombreBlocs = nombreBlocsChances + nombreBlocsBriques + nombreBlocsPierres;

    Bloc **tableauBlocs = malloc(sizeof(Bloc *) * nombreBlocs);

    //Faire une boucle for qui permet de disposer les bloc_pierre en forme d'escalier gauche et droit
    int positionsBlocsChances[6] = {450, 600, 750, 785, 950, 1100};
    int positionsBlocsBriques[3] = {350, 400, 500};
    int positionsBlocsPierresX[20] = {1600, 1635, 1670, 1705, 1635, 1670, 1705, 1670, 1705, 1705, 1960, 1960, 1995, 1960, 1995, 2030, 1960, 1995, 2030, 2065};
    int positionsBlocsPierresY[20] = {240, 240, 240, 240, 205, 205, 205, 170, 170, 135, 135, 170, 170, 205, 205, 205, 240, 240, 240, 240}; 

    Perso *mario;
    mario = malloc(sizeof(Perso));

    SDL_Rect rectNull = {-1, -1, -1, -1}; //Rect qui sert de bloc par défaut quand mario n'est pas sur un bloc (chance, brique, etc.) ; sol != bloc
    mario->block = &rectNull;
    mario->indiceBlock = -1;
    mario->orientation = 1;
    mario->stepCount = 0;
    mario->jumpCount = 9;
    mario->isTouchingBlock = false;
    mario->isJumping = false;
    mario->isWalkingLeft = false;
    mario->isWalkingRight = false;

    SDL_Init(SDL_INIT_VIDEO);

    //Initialisation de la fenetre
    window = SDL_CreateWindow(
                                "Super Brechi", SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED,
                                800,
                                300,
                                SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
                                );


    if (window == NULL) // Si l'ouverture a échoué, on le note et on arrête
    {
        fprintf(stderr, "Impossible de charger le mode vidéo : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_Event event;

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Surface *surfaceFond = SDL_LoadBMP("src/fond.bmp");
    SDL_Texture *textureFond = SDL_CreateTextureFromSurface(renderer, surfaceFond);
    SDL_free(surfaceFond);

    SDL_Rect fond;
    fond.x = 0;
    fond.y = 0;
    fond.h = 300;
    fond.w = 1200;

    SDL_Rect fond2;
    fond2.x = 1200;
    fond2.y = 0;
    fond2.h = 300;
    fond2.w = 1200;

    SDL_Rect fond3;
    fond3.x = 2400;
    fond3.y = 0;
    fond3.h = 300;
    fond3.w = 1200;

    SDL_Surface *surfaceMario = SDL_LoadBMP("src/mario_droite.bmp");
    SDL_Texture *textureMario = SDL_CreateTextureFromSurface(renderer, surfaceMario);
    SDL_Rect dest = {0, 0, 50, 50}; //un SDL_Rect qui sert de destination à l'image
    mario->rect = dest; //Est-ce que ça va surcharger la ram ?
    mario->x = &mario->rect.x;
    mario->y = &mario->rect.y;

    SDL_Surface *surfaceGoomba = SDL_LoadBMP("src/goomba.bmp");
    SDL_Texture *textureGoomba = SDL_CreateTextureFromSurface(renderer, surfaceGoomba);
    SDL_free(surfaceGoomba);
    SDL_Rect goomba = {0, 0, 40, 40};

    goomba.x = 200;
    goomba.y = 235;

    SDL_free(surfaceMario);
 
    *mario->x = 10;
    *mario->y = 230;

    SDL_Surface *surfaceBlocChance = SDL_LoadBMP("src/bloc_chance.bmp");

    for(int i = 0; i < nombreBlocsChances; i++){
        Bloc *blocChance;
        blocChance = malloc(sizeof(Bloc));

        blocChance->x = &blocChance->fond.x;
        blocChance->y = &blocChance->fond.y;
        blocChance->type = 1;
        blocChance->texture = SDL_CreateTextureFromSurface(renderer, surfaceBlocChance);
        SDL_Rect rectBlocChance = {positionsBlocsChances[i], 150, 35, 35};
        blocChance->fond = rectBlocChance;
        tableauBlocs[i] = blocChance;

    }

    SDL_FreeSurface(surfaceBlocChance);

    SDL_Surface *surfaceBlocBrique = SDL_LoadBMP("src/bloc_brique.bmp");

    for(int i = 0; i < nombreBlocsBriques; i++){
        Bloc *blocBrique;
        blocBrique = malloc(sizeof(Bloc));

        blocBrique->x = &blocBrique->fond.x;
        blocBrique->y = &blocBrique->fond.y;
        blocBrique->type = 2;
        blocBrique->texture = SDL_CreateTextureFromSurface(renderer, surfaceBlocBrique);
        SDL_Rect rectBlocBrique = {positionsBlocsBriques[i], 150, 35, 35};
        blocBrique->fond = rectBlocBrique;
        tableauBlocs[i + nombreBlocsChances] = blocBrique;

    }

    SDL_FreeSurface(surfaceBlocBrique);


    SDL_Surface *surfaceBlocPierre = SDL_LoadBMP("src/bloc_pierre.bmp");

    for(int i = 0; i < nombreBlocsPierres; i++){
        Bloc *blocPierre;
        blocPierre = malloc(sizeof(Bloc));

        blocPierre->x = &blocPierre->fond.x;
        blocPierre->y = &blocPierre->fond.y;
        blocPierre->type = 3;
        blocPierre->texture = SDL_CreateTextureFromSurface(renderer, surfaceBlocPierre);
        SDL_Rect rectBlocPierre = {positionsBlocsPierresX[i], positionsBlocsPierresY[i], 35, 35};
        blocPierre->fond = rectBlocPierre;
        tableauBlocs[i + nombreBlocsChances + nombreBlocsBriques] = blocPierre;
    }
    SDL_FreeSurface(surfaceBlocPierre);

    SDL_Surface *surfaceFondBleu = SDL_LoadBMP("src/fond_bleu.bmp");
    SDL_Texture *textureFondBleu = SDL_CreateTextureFromSurface(renderer, surfaceFondBleu);
    SDL_FreeSurface(surfaceFondBleu);
    SDL_Rect rectFondBleu = {1740, 240, 220, 70};


    while (continuer)
    {

        SDL_RenderCopy(renderer, textureFond, NULL, &fond);
        SDL_RenderCopy(renderer, textureFond, NULL, &fond2);
        SDL_RenderCopy(renderer, textureFond, NULL, &fond3);
        SDL_RenderCopy(renderer, textureFondBleu, NULL, &rectFondBleu);
        SDL_RenderCopy(renderer, textureMario, NULL, &mario->rect);
        SDL_RenderCopy(renderer, textureGoomba, NULL, &goomba);

        for(int i = 0; i < nombreBlocs; i++){
            SDL_RenderCopy(renderer, tableauBlocs[i]->texture, NULL, &(tableauBlocs[i]->fond));
        }

        SDL_RenderPresent(renderer);
        SDL_RenderClear(renderer);


        if(SDL_HasIntersection(&mario->rect, &goomba)){
            fprintf(stderr, "Personnage touché\n");
        }

        SDL_PollEvent(&event);

        switch(event.type)
        {
            case SDL_QUIT:
                continuer = 0;
                break;

            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                    case SDLK_RIGHT:
                        mario->isWalkingRight = true;
                        break;
                    
                    case SDLK_LEFT:
                        mario->isWalkingLeft = true;
                        break;
                    
                    case SDLK_ESCAPE:
                        continuer = 0;
                        break;
                    
                    case SDLK_UP:
                        if(mario->isJumping == false)
                            mario->isJumping = true;
                        break;
                    
                }
                break;
            
            case SDL_KEYUP:
                switch(event.key.keysym.sym)
                {
                    case SDLK_RIGHT:
                        mario->isWalkingRight = false;
                        break;
                    case SDLK_LEFT:
                        mario->isWalkingLeft = false;
                        break;                    
                }
                break;

        }

        if(mario->isWalkingRight == true){
            *mario->x += 10;

            mario->isTouchingBlock = false;
            for(int i = 0; i < nombreBlocs; i++){
                if(SDL_HasIntersection(&mario->rect, &(tableauBlocs[i]->fond))){
                    *mario->x -= 10;
                    mario->isTouchingBlock = true;
                    break;
                }
            }

            if(!(mario->isTouchingBlock)){
                *mario->x -= 10;
                if(*mario->x == 500 && fond.x > -2500){
                    fond.x -= 10;
                    fond2.x -= 10;
                    fond3.x -= 10;
                    goomba.x -= 10;
                    rectFondBleu.x -= 10;
                    for(int i = 0; i < nombreBlocs; i++){
                        *tableauBlocs[i]->x -= 10;
                    }
                }
                else{
                    if(*mario->x < 750)
                        *mario->x += 10;
                }
                    

                mario->orientation = 1;

                if(mario->isJumping == false){
                    if(mario->stepCount%2 == 0)
                    {
                        SDL_DestroyTexture(textureMario);
                        surfaceMario = SDL_LoadBMP("src/mario_droite.bmp");
                        textureMario = SDL_CreateTextureFromSurface(renderer, surfaceMario);
                        SDL_FreeSurface(surfaceMario);
                        mario->stepCount++;
                    }
                    else if (mario->stepCount%2 == 1)
                    {
                        SDL_DestroyTexture(textureMario);
                        surfaceMario = SDL_LoadBMP("src/mario_droite1.bmp");
                        textureMario = SDL_CreateTextureFromSurface(renderer, surfaceMario);
                        SDL_FreeSurface(surfaceMario);
                        mario->stepCount++;
                    }
                }
            }        
        }

        if(mario->isWalkingLeft == true){
            *mario->x -= 10;

            mario->isTouchingBlock = false;
            for(int i = 0; i < nombreBlocs; i++){
                if(SDL_HasIntersection(&mario->rect, &(tableauBlocs[i]->fond))){
                    *mario->x += 10;
                    mario->isTouchingBlock = true;
                    break;
                }
            }

            if(!(mario->isTouchingBlock)){
                *mario->x += 10;

                if(*mario->x == 300 && fond.x < 0){
                    fond.x += 10;
                    fond2.x += 10;
                    fond3.x += 10;
                    goomba.x += 10;
                    rectFondBleu.x += 10;
                    for(int i = 0; i <  nombreBlocs; i++){
                        *tableauBlocs[i]->x += 10;
                    }
                }
                else{
                    if(*mario->x > 0)
                        *mario->x -= 10;
                }
                
                mario->orientation = -1;

                if(mario->isJumping == false){
                    if (mario->stepCount%2 == 0)
                    {
                        SDL_DestroyTexture(textureMario);
                        surfaceMario = SDL_LoadBMP("src/mario_gauche.bmp");
                        textureMario = SDL_CreateTextureFromSurface(renderer, surfaceMario);
                        SDL_FreeSurface(surfaceMario);
                        mario->stepCount++;
                    }
                    else if (mario->stepCount%2 == 1)
                    {
                        SDL_DestroyTexture(textureMario);
                        surfaceMario = SDL_LoadBMP("src/mario_gauche1.bmp");
                        textureMario = SDL_CreateTextureFromSurface(renderer, surfaceMario);
                        SDL_FreeSurface(surfaceMario);
                        mario->stepCount++;
                    }
                }
            }
        }

        if(!(mario->block->x == -1)){ //Si le bloc sur lequel était mario est un bloc avec lequel il peut interagir, donc != de rectNull
            if(!(*mario->x <= mario->block->x + 35 && *mario->x + 50 >= mario->block->x)){
                if(mario->isJumping == false){
                    mario->isJumping = true;
                    mario->jumpCount = -2; 
                }
                mario->block = &rectNull;
            }
        }

        if(mario->isJumping == true){
            neg = 1;
            if(mario->jumpCount < 0)
                neg = -1;


            valeur = (pow(mario->jumpCount, 2))*0.5 * neg;
            *mario->y -= valeur;

            //Gestions collisions

            if(mario->jumpCount < 0){
                if(*mario->y  > 230){
                    endOfJump = true;
                    *mario->y = 230;
                }

                for(int i = 0; i < nombreBlocs; i++){
                    if(SDL_HasIntersection(&mario->rect, &(tableauBlocs[i]->fond))){
                        endOfJump = true;
                        *mario->y = *tableauBlocs[i]->y - mario->rect.h;
                        mario->block = &tableauBlocs[i]->fond;
                    }
                }
            }
            else{
                for(int i = 0; i < nombreBlocs; i++){
                    if(SDL_HasIntersection(&mario->rect, &(tableauBlocs[i]->fond))){
                        if(*mario->y <= *tableauBlocs[i]->y + 35 && *mario->y >= *tableauBlocs[i]->y){
                            mario->jumpCount = -1;
                            *mario->y = *tableauBlocs[i]->y + 35;
                            if(tableauBlocs[i]->type == 2)
                                mario->indiceBlock = i;
                        }
                    }
                }
            }
                
            if(!endOfJump){
                if(mario->orientation == 1){
                    SDL_DestroyTexture(textureMario);
                    surfaceMario = SDL_LoadBMP("src/mario_saut_droit.bmp");
                    textureMario = SDL_CreateTextureFromSurface(renderer, surfaceMario);
                    SDL_FreeSurface(surfaceMario);
                }
                else if(mario->orientation == -1){
                    SDL_DestroyTexture(textureMario);
                    surfaceMario = SDL_LoadBMP("src/mario_saut_gauche.bmp");
                    textureMario = SDL_CreateTextureFromSurface(renderer, surfaceMario);
                    SDL_FreeSurface(surfaceMario);
                }

                mario->jumpCount -= 1;

                if(!(mario->indiceBlock == -1)){
                    tabTemporaire = change_size(tableauBlocs, &nombreBlocs, mario->indiceBlock);

                    free(tableauBlocs[mario->indiceBlock]);
                    free(tableauBlocs);

                    tableauBlocs = malloc(sizeof(Bloc *) * nombreBlocs);

                    for(int i = 0; i < nombreBlocs; i++)
                        tableauBlocs[i] = tabTemporaire[i];

                    free(tabTemporaire);
                    mario->indiceBlock = -1;
                }
            }
        }

        if(endOfJump){
            mario->isJumping = false;
            mario->jumpCount = 9;

            if(mario->orientation == 1){
                SDL_DestroyTexture(textureMario);
                surfaceMario = SDL_LoadBMP("src/mario_droite.bmp");
                textureMario = SDL_CreateTextureFromSurface(renderer, surfaceMario);
                SDL_FreeSurface(surfaceMario);
            }
            else if(mario->orientation == -1){
                SDL_DestroyTexture(textureMario);
                surfaceMario = SDL_LoadBMP("src/mario_gauche.bmp");
                textureMario = SDL_CreateTextureFromSurface(renderer, surfaceMario);
                SDL_FreeSurface(surfaceMario);
            }

            endOfJump = false;
        }

        SDL_Delay(30);
        
    }

    for(int i = 0; i < nombreBlocs; i++){
        free(tableauBlocs[i]);
    }
    free(tableauBlocs);

    SDL_DestroyTexture(textureMario);
    free(mario);
    SDL_DestroyTexture(textureFond);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

//Sous macos, saut de 10, sous window saut de 5