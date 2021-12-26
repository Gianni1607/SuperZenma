#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "musique.h"
#include "affichage.h"
#include "fond.h"
#include "vide.h"
#include "menu.h"
#include "fin.h"
#include "win.h"
#include "poteau.h"
#include "chateau.h"
#include "personnage.h"
#include "goomba.h"
#include "bloc.h"
#include "item.h"

int main(int argc, char** argv){

    SDL_Window *window = NULL;

    //Initialisation variables
    int WINDOW_H = 300;
    int WINDOW_W = 800;
    bool endGame = false;
    bool game_over = false;
    Item **tabItemsTemporaire;
    bool continuer = true;
    int nextYPosition;
    int nombreBlocsChances = 7;
    int nombreBlocsBriques = 10;
    int nombreBlocsPierres = 26;
    int nombreBlocs = nombreBlocsChances + nombreBlocsBriques + nombreBlocsPierres;
    int nombreGoombas = 5;
    int nombreItems = 0;
    Goomba *goombaCollision;

    //Initialisation tableaux dynamiques
    Bloc **tableauBlocs = malloc(sizeof(Bloc *) * nombreBlocs);
    Goomba **tableauGoombas = malloc(sizeof(Goomba *) * nombreGoombas);
    Item **tableauItems = malloc(sizeof(Item *) * 1);

    //Initialisation listes statiques
    int positionsBlocsBriques[10] = {350, 420, 750, 785, 990, 1085, 1150, 2300, 2370, 2600};
    int positionsBlocsChances[7] = {385, 820, 940, 1050, 1185, 2335, 2635};
    int positionsBlocsPierresX[26] = {1600, 1635, 1670, 1705, 1635, 1670, 1705, 1670, 1705, 1705, 1960, 1960, 1995, 1960, 1995, 2030, 1960, 1995, 2030, 2065, 2850, 2885, 2920, 2885, 2920, 2920};
    int positionsBlocsPierresY[26] = {240, 240, 240, 240, 205, 205, 205, 170, 170, 135, 135, 170, 170, 205, 205, 205, 240, 240, 240, 240, 240, 240, 240, 205, 205, 170}; 
    int positionsGoombas[5] = {200, 400, 1000, 2300, 2600};

    //Initialisation struct Perso dynamique
    Perso *mario;
    mario = malloc(sizeof(Perso));

    //Initialisation différents attributs mario
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

    //Initialisation audio, video
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

    //Gestion audio
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
    //Mix_PlayMusic(musique, -1);

    //Appel fenetre menu
    continuer = menu();

    //Initialisation de la fenetre
    window = SDL_CreateWindow(
                                "Super ZenMa Bros", SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED,
                                WINDOW_W,
                                WINDOW_H,
                                SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
                                );


    if (window == NULL) // Si l'ouverture a échoué, on le note et on arrête
    {
        fprintf(stderr, "Impossible de charger le mode vidéo : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    //Variable qui stocke les évenements de la fenêtre
    SDL_Event event;

    //Initialisation renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    //Initialisation du fond

    SDL_Rect fond = {0, 0, 1200, 300};

    Fond *background = malloc(sizeof(Fond));
    background->fond = fond;
    background->x = &background->fond.x;
    background->y = &background->fond.y;
    background->w = &background->fond.w;
    background->h = &background->fond.h;

    SDL_Surface *surfaceFond = SDL_LoadBMP("src/fond.bmp");
    background->texture = SDL_CreateTextureFromSurface(renderer, surfaceFond);
    SDL_free(surfaceFond);

    background->compteur = 0;

    //Initialisation texture mario
    SDL_Surface *surfaceMario = SDL_LoadBMP("src/mario_droite.bmp");
    mario->texture = SDL_CreateTextureFromSurface(renderer, surfaceMario);
    SDL_free(surfaceMario);
    SDL_Rect dest = {10, 230, 50, 50};
    mario->rect = dest;
    mario->x = &mario->rect.x;
    mario->y = &mario->rect.y;

    //Création goombas
    SDL_Surface *surfaceGoomba = SDL_LoadBMP("src/goomba_droit.bmp");

    for(int i = 0; i < nombreGoombas; i++){
        Goomba *goomba;
        goomba = malloc(sizeof(Goomba));

        goomba->x = &goomba->fond.x;
        goomba->y = &goomba->fond.y;
        goomba->mort = false;
        goomba->waitingCount = 0;
        goomba->stepCount = 0;

        if(i%2)
            goomba->orientation = 1;
        else
            goomba->orientation = -1;

        goomba->texture = SDL_CreateTextureFromSurface(renderer, surfaceGoomba);
        SDL_Rect rectGoomba = {positionsGoombas[i], 235, 40, 40};
        goomba->fond = rectGoomba;
        tableauGoombas[i] = goomba;
    }
    SDL_FreeSurface(surfaceGoomba);

    //Création blocs chances
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

    //Création blocs briques
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

    //Création blocs pierre
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

    //Création fond bleu (vide)
    Vide *vide = malloc(sizeof(Vide));

    vide->x = &vide->fond.x;
    vide->y = &vide->fond.y;

    SDL_Rect rectVide = {1740, 240, 220, 70};
    vide->fond = rectVide;

    SDL_Surface *surfaceVide = SDL_LoadBMP("src/fond_bleu.bmp");
    vide->texture = SDL_CreateTextureFromSurface(renderer, surfaceVide);
    SDL_FreeSurface(surfaceVide);
    

    //Création poteau fin jeu
    Poteau *poteauFin = malloc(sizeof(Poteau));
    poteauFin->xPoteau = &poteauFin->poteau.x;
    poteauFin->yPoteau = &poteauFin->poteau.y;
    poteauFin->xDrapeau = &poteauFin->drapeau.x;
    poteauFin->yDrapeau = &poteauFin->drapeau.y;

    SDL_Rect poteau = {3100, 35, 30, 240};
    SDL_Rect drapeau = {3075, 50, 40, 35};
    poteauFin->poteau = poteau;
    poteauFin->drapeau = drapeau;

    SDL_Surface *surfacePoteau = SDL_LoadBMP("src/poteau.bmp");
    poteauFin->texturePoteau = SDL_CreateTextureFromSurface(renderer, surfacePoteau);
    SDL_FreeSurface(surfacePoteau);

    SDL_Surface *surfaceDrapeau = SDL_LoadBMP("src/drapeau.bmp");
    poteauFin->textureDrapeau = SDL_CreateTextureFromSurface(renderer, surfaceDrapeau);
    SDL_FreeSurface(surfaceDrapeau);

    //Création château

    Chateau *chateau = malloc(sizeof(Chateau));
    chateau->xGauche = &chateau->chateauGauche.x;
    chateau->xDroit = &chateau->chateauDroit.x;

    SDL_Rect rectChateauGauche = {3300, 107, 100, 170};
    chateau->chateauGauche = rectChateauGauche;
    SDL_Surface *surfaceChateauGauche = SDL_LoadBMP("src/chateau_gauche.bmp");
    chateau->textureGauche = SDL_CreateTextureFromSurface(renderer, surfaceChateauGauche);
    SDL_FreeSurface(surfaceChateauGauche);

    SDL_Rect rectChateauDroit = {3400, 107, 100, 170};
    chateau->chateauDroit = rectChateauDroit;
    SDL_Surface *surfaceChateauDroit = SDL_LoadBMP("src/chateau_droit.bmp");
    chateau->textureDroite = SDL_CreateTextureFromSurface(renderer, surfaceChateauDroit);
    SDL_FreeSurface(surfaceChateauDroit);

    //BOUCLE PRINCIPALE
    while (continuer)
    {
        //AFFICHAGE ELEMENTS

        affichage(renderer, background, WINDOW_W, WINDOW_H, vide, poteauFin, chateau, mario, nombreGoombas, nombreBlocs, nombreItems, tableauGoombas, tableauBlocs, tableauItems);

        /*
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

        SDL_RenderCopy(renderer, mario->texture, NULL, &mario->rect);

        SDL_RenderCopy(renderer, chateau->textureDroite, NULL, &(chateau->chateauDroit));

        SDL_RenderPresent(renderer);
        SDL_RenderClear(renderer);
        */

        SDL_PollEvent(&event);

        //GESTION EVENEMENTS

        switch(event.type)
        {
            case SDL_QUIT:
                continuer = 0;
                break;

            case SDL_KEYDOWN: //APPUI TOUCHE
                switch(event.key.keysym.sym)
                {
                    case SDLK_RIGHT: //DROITE
                        if(!mario->end)
                            mario->isWalkingRight = true;
                        break;
                    
                    case SDLK_LEFT: //GAUCHE
                        if(!mario->end)
                            mario->isWalkingLeft = true;
                        break;
                    
                    case SDLK_ESCAPE: //ECHAP
                        continuer = 0;
                        break;
                    
                    case SDLK_UP: //HAUT
                        if(mario->isJumping == false && !mario->end)
                            mario->isJumping = true;
                        break;
                    
                }
                break;
            
            case SDL_KEYUP: //RELACHER TOUCHE
                switch(event.key.keysym.sym)
                {
                    case SDLK_RIGHT: //DROITE
                        if(!mario->end)
                            mario->isWalkingRight = false;
                        break;
                    case SDLK_LEFT: //GAUCHE
                        if(!mario->end)
                            mario->isWalkingLeft = false;
                        break;                    
                }
                break;
        }

        //DEPLACEMENT MARIO DROITE

        if(mario->isWalkingRight == true && !mario->mort){ //Si flèche droite appuyée et mario pas mort alors :
            *mario->x += 10; //il bouge de 10 pixels

            mario->isTouchingBlock = false;
            for(int i = 0; i < nombreBlocs; i++){
                if(SDL_HasIntersection(&mario->rect, &(tableauBlocs[i]->fond))){ //Si interaction avec un quelconque bloc du jeu :
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
            if(!(*mario->x <= mario->block->x + 35 && *mario->x + 50 >= mario->block->x)){ //Et si mario est dehors de la colonne qui contient le bloc
                if(mario->isJumping == false){ //Et qu'il n'est pas en train de sauter
                    mario->isJumping = true; //alors il saute
                    mario->jumpCount = -2; //négativement <=> tomber
                }
                mario->block = &rectNull; //Et on rénitialise le bloc sur lequel était mario
            }
        }

        //GESTION MORT DANS LE VIDE ET COLLISIONS GOOMBA

        if(SDL_HasIntersection(&(mario->rect), &(vide->fond)) && !mario->mort && !mario->end) //Si mario entre en contact avec le fondBleu <=> vide et qu'il n'est pas déjà mort et qu'il n'a pas gagné le jeu
            mario->mort = true; //il devient mort

        if(!mario->mort && !mario->end){ //Si mario n'est pas mort et qu'il n'a pas fini le jeu
            for(int i = 0; i < nombreGoombas; i++){
                goombaCollision = tableauGoombas[i];
                if(SDL_HasIntersection(&(mario->rect), &(goombaCollision->fond)) && !(goombaCollision->mort)){ //et qu'il rentre en contact avec un goomba vivant
                    if(mario->isJumping && mario->jumpCount < 0){ //si mario saute négativement <=> tombe
                        mario->jumpCount = 5; //il fait un petit saut
                        goombaCollision->mort = true; //le goomba meurt
                        son = Mix_LoadWAV("src/kick.wav"); //et on joue un son
                        Mix_VolumeChunk(son, MIX_MAX_VOLUME);
                        Mix_PlayChannel(-1, son, 0);

                        SDL_DestroyTexture(goombaCollision->texture);
                        surfaceGoomba = SDL_LoadBMP("src/goomba_mort.bmp"); //on charge l'image du goomba écrasé
                        goombaCollision->texture = SDL_CreateTextureFromSurface(renderer, surfaceGoomba);
                        goombaCollision->fond.h = 20;
                        *goombaCollision->y += 20;
                        SDL_FreeSurface(surfaceGoomba);
                    }
                    else{ //sinon mario meurt
                        mario->isJumping = true;
                        mario->mort = true;
                    }

                }
            }
        }

        //GESTION COLLISION DRAPEAU

        collisionPoteau(mario, poteauFin, &musique, son, chateau->chateauGauche, &continuer, &endGame);

        //GESTION SAUT

        Jump(mario, &nombreBlocs, &tableauBlocs, &nombreBlocsBriques, renderer, &nombreItems, &tableauItems);
        
        //DEPLACEMENTS GOOMBA

        moveGoomba(&tableauGoombas, &tableauBlocs, &nombreGoombas, mario, poteauFin, renderer, fond, son, &nombreBlocsPierres, &nombreBlocsChances, &nombreBlocsBriques);

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
    SDL_DestroyTexture(background->texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    if(endGame)
        win();

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