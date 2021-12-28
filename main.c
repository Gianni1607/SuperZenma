#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "musique.h"
#include "poteau.h"
#include "personnage.h"
#include "affichage.h"
#include "fond.h"
#include "vide.h"
#include "menu.h"
#include "fin.h"
#include "win.h"
#include "chateau.h"
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

    //Initialisation mario

    Perso *mario = initPerso(renderer, 10, 230, 50, 50, "src/mario_droite.bmp");

    //Initialisation du fond

    Fond *background = initFond(renderer, 0, 0, 1200, 300, "src/fond.bmp");

    //Création goombas

    for(int i = 0; i < nombreGoombas; i++){
        int orientation;

        if(i%2)
            orientation = 1;
        else
            orientation = -1;

        Goomba *goomba = initGoomba(renderer, positionsGoombas[i], 235, 40, 40, orientation, "src/goomba_droit.bmp");
        tableauGoombas[i] = goomba;
    }

    //Création blocs chances

    for(int i = 0; i < nombreBlocsChances; i++){
        Bloc *blocChance = initBloc(renderer, positionsBlocsChances[i], 150, 35, 35, 1, "src/bloc_chance.bmp");
        tableauBlocs[i] = blocChance;
    }

    //Création blocs briques

    for(int i = 0; i < nombreBlocsBriques; i++){
        Bloc *blocBrique = initBloc(renderer, positionsBlocsBriques[i], 150, 35, 35, 2, "src/bloc_brique.bmp");
        tableauBlocs[i + nombreBlocsChances] = blocBrique;
    }

    //Création blocs pierre

    for(int i = 0; i < nombreBlocsPierres; i++){
        Bloc *blocPierre = initBloc(renderer, positionsBlocsPierresX[i], positionsBlocsPierresY[i], 35, 35, 3, "src/bloc_pierre.bmp");
        tableauBlocs[i + nombreBlocsChances + nombreBlocsBriques] = blocPierre;
    }

    //Création fond bleu (vide)
    Vide *vide = initVide(renderer, 1740, 240, 220, 70, "src/fond_bleu.bmp");

    //Création poteau fin jeu
    Poteau *poteauFin = initPoteau(renderer, 3100, 35, 30, 240, "src/poteau.bmp", "src/drapeau.bmp");

    //Création château

    Chateau *chateau = initChateau(renderer, 3300, 107, 100, 170, "src/chateau_gauche.bmp", "src/chateau_droit.bmp");

    //BOUCLE PRINCIPALE
    while (continuer)
    {
        //AFFICHAGE ELEMENTS

        affichage(renderer, background, WINDOW_W, WINDOW_H, vide, poteauFin, chateau, mario, nombreGoombas, nombreBlocs, nombreItems, tableauGoombas, tableauBlocs, tableauItems);

        //GESTION EVENEMENTS

        SDL_PollEvent(&event);

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

        //GESTION MARIO :

            //DEPLACEMENT MARIO DROITE
            if(mario->isWalkingRight == true && !mario->mort) //Si flèche droite appuyée et mario pas mort
                moveRight(renderer, mario, background, vide, poteauFin, chateau, tableauBlocs, tableauGoombas, nombreBlocs, nombreGoombas);

            //DEPLACEMENT MARIO GAUCHE
            if(mario->isWalkingLeft == true && !mario->mort) //Si flèche gauche appuyée et mario pas mort
                moveLeft(renderer, mario, background, vide, poteauFin, chateau, tableauBlocs, tableauGoombas, nombreBlocs, nombreGoombas);

            //SAUT MARIO
            if(mario->isJumping) //Si mario peut sauter
                jump(mario, &nombreBlocs, &tableauBlocs, &nombreBlocsBriques, renderer, &nombreItems, &tableauItems);

            //VERIFICATION SI MARIO QUITTE LE BLOC SUR LEQUEL IL ETAIT
            isFallingOffTheBlock(mario);

            //VERIFICATION COLLISION DRAPEAU
            collisionPoteau(mario, poteauFin, &musique, son, chateau->chateauGauche, &continuer, &endGame);

            //VERIFICATION CHUTE DANS LE VIDE
            if(isMarioFallingInTheVoid(mario, vide)) //Si mario tombe dans le vide
                mario->mort = true; //Il a le statut de mort

            //GESTION COLLISIONS GOOMBA
            isMarioHittingAGoomba(renderer, mario, tableauGoombas, nombreGoombas, son);

            //GESTION MORT
            if(mario->mort) //Si mario a le statut de mort
                marioDies(mario, &musique, &continuer, &game_over); //Il meurt
        
        //DEPLACEMENTS GOOMBA
        moveGoomba(&tableauGoombas, &tableauBlocs, &nombreGoombas, mario, poteauFin, renderer, background, son, &nombreBlocsPierres, &nombreBlocsChances, &nombreBlocsBriques);


        SDL_Delay(30);
    }

    //LIBERATION DE LA MEMOIRE

    for(int i = 0; i < nombreBlocs; i++)
        free(tableauBlocs[i]);
    free(tableauBlocs);

    for(int i = 0; i < nombreGoombas; i++)
        free(tableauGoombas[i]);
    free(tableauGoombas);

    for(int i = 0; i < nombreItems; i++)
        free(tableauItems[i]);
    free(tableauItems);

    SDL_DestroyTexture(mario->texture);
    free(mario);
    SDL_DestroyTexture(background->texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    //ECRAN FIN JEU (SUCCES)
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