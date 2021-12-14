#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "musique.h"
#include "menu.h"
#include "fin.h"
#include "win.h"
#include "poteau.h"
#include "personnage.h"
#include "goomba.h"
#include "bloc.h"

Goomba **change_size2(Goomba **tab, int *taille, int indice){
    *taille -= 1;
    Goomba **tab2 = malloc(sizeof(Goomba *) * *taille);
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

    bool endGame = false;
    bool game_over = false;
    int valeur;
    Bloc **tabBlocsTemporaire;
    Goomba **tabGoombasTemporaire;
    bool endOfJump = false;
    int neg = 1;
    int continuer = 1;
    int nextYPosition;
    int nombreBlocsChances = 6;
    int nombreBlocsBriques = 3;
    int nombreBlocsPierres = 20;
    int nombreBlocs = nombreBlocsChances + nombreBlocsBriques + nombreBlocsPierres;
    int nombreGoombas = 5;
    Goomba *goombaCollision;

    Bloc **tableauBlocs = malloc(sizeof(Bloc *) * nombreBlocs);
    Goomba **tableauGoombas = malloc(sizeof(Goomba *) * nombreGoombas);

    //Faire une boucle for qui permet de disposer les bloc_pierre en forme d'escalier gauche et droit
    int positionsBlocsChances[6] = {450, 600, 750, 785, 950, 1100};
    int positionsBlocsBriques[3] = {350, 400, 500};
    int positionsBlocsPierresX[20] = {1600, 1635, 1670, 1705, 1635, 1670, 1705, 1670, 1705, 1705, 1960, 1960, 1995, 1960, 1995, 2030, 1960, 1995, 2030, 2065};
    int positionsBlocsPierresY[20] = {240, 240, 240, 240, 205, 205, 205, 170, 170, 135, 135, 170, 170, 205, 205, 205, 240, 240, 240, 240}; 
    int positionsGoombas[5] = {200, 400, 1000, 2300, 2600};

    Perso *mario;
    mario = malloc(sizeof(Perso));

    SDL_Rect rectNull = {-1, -1, -1, -1}; //Rect qui sert de bloc par défaut quand mario n'est pas sur un bloc (chance, brique, etc.) ; sol != bloc
    mario->mort = false;
    mario->end = false;
    mario->block = &rectNull;
    mario->waitingCount = 0;
    mario->indiceGoomba = -1;
    mario->indiceBlock = -1;
    mario->orientation = 1;
    mario->stepCount = 0;
    mario->jumpCount = 9;
    mario->isTouchingBlock = false;
    mario->isJumping = false;
    mario->isWalkingLeft = false;
    mario->isWalkingRight = false;

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1) //Initialisation de l'API Mixer
    {
      printf("%s", Mix_GetError());
      return -1;
    }

    Mix_Music *musique;
    Mix_Chunk *son;
    musique = Mix_LoadMUS("src/music.wav");

    if(musique == NULL)
    {
        fprintf(stderr,"%s", Mix_GetError());
        return -1;
    }
    Mix_VolumeMusic(MIX_MAX_VOLUME / 2);
    Mix_PlayMusic(musique, -1);

    continuer = menu();

    //Initialisation de la fenetre
    window = SDL_CreateWindow(
                                "Super ZenMa Bros", SDL_WINDOWPOS_UNDEFINED,
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
    mario->texture = SDL_CreateTextureFromSurface(renderer, surfaceMario);
    SDL_Rect dest = {0, 0, 50, 50};
    mario->rect = dest;
    mario->x = &mario->rect.x;
    mario->y = &mario->rect.y;

    SDL_Surface *surfaceGoomba = SDL_LoadBMP("src/goomba_droit.bmp");

    for(int i = 0; i < nombreGoombas; i++){
        Goomba *goomba;
        goomba = malloc(sizeof(Goomba));

        goomba->x = &goomba->fond.x;
        goomba->y = &goomba->fond.y;
        goomba->mort = false;
        goomba->waitingCount = 0;
        goomba->stepCount = 0;

        if(i%2){
            goomba->orientation = 1;
        }
        else{
            goomba->orientation = -1;
        }

        goomba->texture = SDL_CreateTextureFromSurface(renderer, surfaceGoomba);
        SDL_Rect rectGoomba = {positionsGoombas[i], 235, 40, 40};
        goomba->fond = rectGoomba;
        tableauGoombas[i] = goomba;
    }

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

    Poteau *poteauFin = malloc(sizeof(Poteau));
    poteauFin->xPoteau = &poteauFin->poteau.x;
    poteauFin->yPoteau = &poteauFin->poteau.y;
    poteauFin->xDrapeau = &poteauFin->drapeau.x;
    poteauFin->yDrapeau = &poteauFin->drapeau.y;

    SDL_Rect poteau = {2900, 35, 30, 240};
    SDL_Rect drapeau = {2875, 50, 40, 35};
    poteauFin->poteau = poteau;
    poteauFin->drapeau = drapeau;

    SDL_Surface *surfacePoteau = SDL_LoadBMP("src/poteau.bmp");
    poteauFin->texturePoteau = SDL_CreateTextureFromSurface(renderer, surfacePoteau);
    SDL_FreeSurface(surfacePoteau);

    SDL_Surface *surfaceDrapeau = SDL_LoadBMP("src/drapeau.bmp");
    poteauFin->textureDrapeau = SDL_CreateTextureFromSurface(renderer, surfaceDrapeau);
    SDL_FreeSurface(surfaceDrapeau);

    SDL_Surface *surfaceChateauGauche = SDL_LoadBMP("src/chateau_gauche.bmp");
    SDL_Texture *textureChateauGauche = SDL_CreateTextureFromSurface(renderer, surfaceChateauGauche);
    SDL_FreeSurface(surfaceChateauGauche);
    SDL_Rect rectChateauGauche = {3100, 107, 100, 170};

    SDL_Surface *surfaceChateauDroit = SDL_LoadBMP("src/chateau_droit.bmp");
    SDL_Texture *textureChateauDroit = SDL_CreateTextureFromSurface(renderer, surfaceChateauDroit);
    SDL_FreeSurface(surfaceChateauDroit);
    SDL_Rect rectChateauDroit = {3200, 107, 100, 170};


    while (continuer)
    {
        //AFFICHAGE ELEMENTS

        SDL_RenderCopy(renderer, textureFond, NULL, &fond);
        SDL_RenderCopy(renderer, textureFond, NULL, &fond2);
        SDL_RenderCopy(renderer, textureFond, NULL, &fond3);
        SDL_RenderCopy(renderer, textureFondBleu, NULL, &rectFondBleu);
        SDL_RenderCopy(renderer, poteauFin->texturePoteau, NULL, &(poteauFin->poteau));
        SDL_RenderCopy(renderer, poteauFin->textureDrapeau, NULL, &(poteauFin->drapeau));
        SDL_RenderCopy(renderer, textureChateauGauche, NULL, &rectChateauGauche);

        for(int i = 0; i < nombreGoombas; i++)
            SDL_RenderCopy(renderer, tableauGoombas[i]->texture, NULL, &(tableauGoombas[i]->fond));

        for(int i = 0; i < nombreBlocs; i++)
            SDL_RenderCopy(renderer, tableauBlocs[i]->texture, NULL, &(tableauBlocs[i]->fond));

        SDL_RenderCopy(renderer, mario->texture, NULL, &mario->rect);

        SDL_RenderCopy(renderer, textureChateauDroit, NULL, &rectChateauDroit);

        SDL_RenderPresent(renderer);
        SDL_RenderClear(renderer);

        SDL_PollEvent(&event);

        //GESTION EVENEMENTS

        switch(event.type)
        {
            case SDL_QUIT:
                continuer = 0;
                break;

            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                    case SDLK_RIGHT:
                        if(!mario->end)
                            mario->isWalkingRight = true;
                        break;
                    
                    case SDLK_LEFT:
                        if(!mario->end)
                            mario->isWalkingLeft = true;
                        break;
                    
                    case SDLK_ESCAPE:
                        continuer = 0;
                        break;
                    
                    case SDLK_UP:
                        if(mario->isJumping == false && !mario->end)
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

        //DEPLACEMENT MARIO DROITE

        if(mario->isWalkingRight == true && !mario->mort){
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
                    rectFondBleu.x -= 10;
                    *poteauFin->xPoteau -= 10;
                    *poteauFin->xDrapeau -= 10;
                    rectChateauDroit.x -= 10;
                    rectChateauGauche.x -= 10;

                    for(int i = 0; i < nombreGoombas; i++){
                        *tableauGoombas[i]->x -= 10;
                    }

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
                    if(mario->stepCount%4 == 0 || mario->stepCount%4 == 1){
                        SDL_DestroyTexture(mario->texture);
                        surfaceMario = SDL_LoadBMP("src/mario_droite.bmp");
                        mario->texture = SDL_CreateTextureFromSurface(renderer, surfaceMario);
                        SDL_FreeSurface(surfaceMario);
                        mario->stepCount++;
                    }
                    else if(mario->stepCount%4 == 2 || mario->stepCount%4 == 3){
                        SDL_DestroyTexture(mario->texture);
                        surfaceMario = SDL_LoadBMP("src/mario_droite1.bmp");
                        mario->texture = SDL_CreateTextureFromSurface(renderer, surfaceMario);
                        SDL_FreeSurface(surfaceMario);
                        mario->stepCount++;
                    }
                }
            }        
        }

        //DEPLACEMENT MARIO GAUCHE

        if(mario->isWalkingLeft == true && !mario->mort){
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
                    rectFondBleu.x += 10;
                    *poteauFin->xPoteau += 10;
                    *poteauFin->xDrapeau += 10;
                    rectChateauDroit.x += 10;
                    rectChateauGauche.x += 10;

                    for(int i = 0; i < nombreGoombas; i++)
                        *tableauGoombas[i]->x += 10;

                    for(int i = 0; i <  nombreBlocs; i++)
                        *tableauBlocs[i]->x += 10;
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
                        surfaceMario = SDL_LoadBMP("src/mario_gauche.bmp");
                        mario->texture = SDL_CreateTextureFromSurface(renderer, surfaceMario);
                        SDL_FreeSurface(surfaceMario);
                        mario->stepCount++;
                    }
                    else if (mario->stepCount%2 == 1)
                    {
                        SDL_DestroyTexture(mario->texture);
                        surfaceMario = SDL_LoadBMP("src/mario_gauche1.bmp");
                        mario->texture = SDL_CreateTextureFromSurface(renderer, surfaceMario);
                        SDL_FreeSurface(surfaceMario);
                        mario->stepCount++;
                    }
                }
            }
        }

        //Si le bloc sur lequel était mario est un bloc avec lequel il peut interagir, donc != de rectNull
        if(!(mario->block->x == -1)){ 
            if(!(*mario->x <= mario->block->x + 35 && *mario->x + 50 >= mario->block->x)){
                if(mario->isJumping == false){
                    mario->isJumping = true;
                    mario->jumpCount = -2; 
                }
                mario->block = &rectNull;
            }
        }

        //GESTION MORT DANS LE VIDE ET COLLISIONS GOOMBA

        if(SDL_HasIntersection(&(mario->rect), &rectFondBleu) && !mario->mort && !mario->end)
            mario->mort = true;

        if(!mario->mort && !mario->end){
            for(int i = 0; i < nombreGoombas; i++){
                goombaCollision = tableauGoombas[i];
                if(SDL_HasIntersection(&(mario->rect), &(goombaCollision->fond)) && !(goombaCollision->mort)){
                    if(mario->isJumping && mario->jumpCount < 0){
                        mario->jumpCount = 5;
                        goombaCollision->mort = true;
                        son = Mix_LoadWAV("src/kick.wav");
                        Mix_VolumeChunk(son, MIX_MAX_VOLUME);
                        Mix_PlayChannel(-1, son, 0);
                        //Mix_FreeChunk(son);
                        //run_music(&musique, "src/kick.wav", false);


                        SDL_DestroyTexture(goombaCollision->texture);
                        surfaceGoomba = SDL_LoadBMP("src/goomba_mort.bmp");
                        goombaCollision->texture = SDL_CreateTextureFromSurface(renderer, surfaceGoomba);
                        goombaCollision->fond.h = 20;
                        *goombaCollision->y += 20;
                        SDL_FreeSurface(surfaceGoomba);
                    }
                    else{
                        mario->isJumping = true;
                        mario->mort = true;
                    }

                }
            }
        }


        //GESTION COLLISION DRAPEAU
        if(SDL_HasIntersection(&(mario->rect), &(poteauFin->poteau)) && !mario->end){
            if(mario->isJumping){
                mario->isWalkingLeft = false;
                mario->isWalkingRight = false;
                mario->isJumping = false;
                mario->end = true;
                run_music(&musique, "src/stage_clear.wav", true);
            }
            else if(mario->isWalkingRight){
                *mario->x -= 10;
            }
            else if(mario->isWalkingLeft){
                *mario->x += 10;
            }
        }

        if(mario->end){
            if(poteauFin->waitingCount < 55){
                poteauFin->drapeau.y += 3;
                poteauFin->waitingCount++;
            }
            if(*mario->y < 210)
                *mario->y += 3;
            else if(poteauFin->waitingCount >= 55){
                if(poteauFin->waitingCount == 55){
                    mario->jumpCount = 10;
                    mario->isJumping = true;
                    mario->isWalkingRight = true;
                }
                poteauFin->waitingCount++;
            }

            if(*mario->x > rectChateauGauche.x + rectChateauGauche.w + 40)
                mario->isWalkingRight = false;

            if(poteauFin->waitingCount == 150){
                son = Mix_LoadWAV("src/fireworks.wav");
                Mix_VolumeChunk(son, MIX_MAX_VOLUME/2);
                Mix_PlayChannel(-1, son, 3);
            }
            else if(poteauFin->waitingCount == 200){
                continuer = false;
                endGame = true;
            }

        }

        //GESTION SAUT

        Jump(mario, &nombreBlocs, &tableauBlocs, &nombreBlocsBriques, renderer);
        
        //DEPLACEMENTS GOOMBA

        for(int i = 0; i < nombreGoombas; i++){
            if(!tableauGoombas[i]->mort){
                *tableauGoombas[i]->x += tableauGoombas[i]->orientation * 3;
                if(tableauGoombas[i]->stepCount%4 == 0 || tableauGoombas[i]->stepCount%4 == 1){
                    SDL_DestroyTexture(tableauGoombas[i]->texture);
                    surfaceGoomba = SDL_LoadBMP("src/goomba_gauche.bmp");
                    tableauGoombas[i]->texture = SDL_CreateTextureFromSurface(renderer, surfaceGoomba);
                    SDL_FreeSurface(surfaceGoomba);
                    tableauGoombas[i]->stepCount++;
                }
                else if(tableauGoombas[i]->stepCount%4 == 2 || tableauGoombas[i]->stepCount%4 == 3){
                SDL_DestroyTexture(tableauGoombas[i]->texture);
                surfaceGoomba = SDL_LoadBMP("src/goomba_droit.bmp");
                tableauGoombas[i]->texture = SDL_CreateTextureFromSurface(renderer, surfaceGoomba);
                SDL_FreeSurface(surfaceGoomba);
                tableauGoombas[i]->stepCount++;
                }
            }
            else{
                tableauGoombas[i]->waitingCount++;
                if(tableauGoombas[i]->waitingCount == 20){
                    mario->indiceGoomba = i;
                    Mix_FreeChunk(son);
                }
            }

            for(int j = 0; j < nombreBlocsPierres; j++){
                if(SDL_HasIntersection(&(tableauGoombas[i]->fond), &(tableauBlocs[j + nombreBlocsChances + nombreBlocsBriques]->fond))){
                    tableauGoombas[i]->orientation *= -1;
                }
                else if(*tableauGoombas[i]->x < fond.x){
                    tableauGoombas[i]->orientation *= -1;
                    *tableauGoombas[i]->x += tableauGoombas[i]->orientation * 3;
                }
                else if(SDL_HasIntersection((&tableauGoombas[i]->fond), &poteauFin->poteau)){
                    tableauGoombas[i]->orientation *= -1;
                    *tableauGoombas[i]->x += tableauGoombas[i]->orientation * 3;
                }
            }
        }

        if(!(mario->indiceGoomba == -1)){
            tabGoombasTemporaire = change_size2(tableauGoombas, &nombreGoombas, mario->indiceGoomba);

            free(tableauGoombas[mario->indiceGoomba]);
            free(tableauGoombas);

            tableauGoombas = malloc(sizeof(Goomba *) * nombreGoombas);

            for(int i = 0; i < nombreGoombas; i++)
                tableauGoombas[i] = tabGoombasTemporaire[i];

            free(tabGoombasTemporaire);
            mario->indiceGoomba = -1;

        }

        //GESTION MORT

        if(mario->mort){
            if(mario->waitingCount == 0)
                run_music(&musique, "src/mario_dead.wav", true);
            mario->waitingCount++;
            if(mario->waitingCount == 90){
                continuer = false;
                game_over = true;
            }
        }

        SDL_Delay(30);
    }

    //LIBERATION DE LA MEMOIRE

    for(int i = 0; i < nombreBlocs; i++){
        free(tableauBlocs[i]);
    }
    free(tableauBlocs);

    for(int i = 0; i < nombreGoombas; i++){
        free(tableauGoombas[i]);
    }
    free(tableauGoombas);

    SDL_DestroyTexture(mario->texture);
    free(mario);
    SDL_DestroyTexture(textureFond);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    if(endGame){
        win();
    }

    //ECRAN DE GAME OVER

    if(game_over){
        run_music(&musique, "src/gameover.wav", true);
        fin();
    }

    Mix_FreeMusic(musique);
    Mix_CloseAudio();
    SDL_Quit();

    return 0;
}

//Sous macos, saut de 10, sous window saut de 5