#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <SDL2/SDL.h>
#include "personnage.h"

int main(int argc, char** argv)
{

    int neg = 1;
    int continuer = 1;

    Perso *mario;
    mario = malloc(sizeof(Perso));

    mario->orientation = 1;
    mario->stepCount = 0;
    mario->jumpCount = 9;
    mario->isJumping = false;
    mario->isWalkingLeft = false;
    mario->isWalkingRight = false;

    SDL_Window *window = NULL;

    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("Super Brechi", SDL_WINDOWPOS_UNDEFINED,
                                                                SDL_WINDOWPOS_UNDEFINED,
                                                                800,
                                                                300,
                                                                SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);


    if (window == NULL) // Si l'ouverture a échoué, on le note et on arrête
    {
        fprintf(stderr, "Impossible de charger le mode vidéo : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_Event event;

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Surface * surfaceFond = SDL_LoadBMP("src/fond.bmp");
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surfaceFond);
    SDL_free(surfaceFond);

    SDL_Rect fond;
    fond.x = 0;
    fond.y = 0;
    fond.h = 300;
    fond.w = 800;

    SDL_Rect fond2;
    fond2.x = 800;
    fond2.y = 0;
    fond2.h = 300;
    fond2.w = 800;

    SDL_Rect fond3;
    fond3.x = 1600;
    fond3.y = 0;
    fond3.h = 300;
    fond3.w = 800;

    SDL_Surface *surfaceMario = SDL_LoadBMP("src/mario_droite.bmp");
    SDL_Texture *textureMario = SDL_CreateTextureFromSurface(renderer, surfaceMario);
    SDL_Rect dest = {320 - surfaceMario->w/2, 240 - surfaceMario->h/2, 50, 50}; //un SDL_Rect qui sers de destination à l'image
    mario->rect = dest;
    mario->x = &mario->rect.x;
    mario->y = &mario->rect.y;

    SDL_Surface *surfaceGoomba = SDL_LoadBMP("src/goomba.bmp");
    SDL_Texture *textureGoomba = SDL_CreateTextureFromSurface(renderer, surfaceGoomba);
    SDL_Rect goomba = {320 - surfaceGoomba->w/2, 240 - surfaceGoomba->h/2, 50, 50};
    SDL_free(surfaceGoomba);

    goomba.x = 200;
    goomba.y = 225;

    SDL_free(surfaceMario);
 
    *mario->x = 10;
    *mario->y = 230;

    while (continuer)
    {
        SDL_RenderCopy(renderer, texture, NULL, &fond);
        SDL_RenderCopy(renderer, texture, NULL, &fond2);
        SDL_RenderCopy(renderer, texture, NULL, &fond3);
        SDL_RenderCopy(renderer, textureMario, NULL, &mario->rect);
        SDL_RenderCopy(renderer, textureGoomba, NULL, &goomba);
        SDL_RenderPresent(renderer);
        SDL_RenderClear(renderer);

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
            if(*mario->x == 500){
                fond.x -= 10;
                fond2.x -= 10;
                fond3.x -= 10;
                goomba.x -= 10;
            }
            else
                *mario->x += 10;

            mario->orientation = 1;

            if(mario->isJumping == false){
                if(mario->stepCount%2 == 0)
                {
                    surfaceMario = SDL_LoadBMP("src/mario_droite.bmp");
                    textureMario = SDL_CreateTextureFromSurface(renderer, surfaceMario);
                    SDL_free(surfaceMario);
                    mario->stepCount++;
                }
                else if (mario->stepCount%2 == 1)
                {
                    surfaceMario = SDL_LoadBMP("src/mario_droite1.bmp");
                    textureMario = SDL_CreateTextureFromSurface(renderer, surfaceMario);
                    SDL_free(surfaceMario);
                    mario->stepCount++;
                }
            }
        
        }

        if(mario->isWalkingLeft == true){
            if(*mario->x == 300){
                fond.x += 10;
                fond2.x += 10;
                fond3.x += 10;
                goomba.x += 10;
            }
            else
                *mario->x -= 10;
            
            mario->orientation = -1;

            if(mario->isJumping == false){
                if (mario->stepCount%2 == 0)
                {
                    surfaceMario = SDL_LoadBMP("src/mario_gauche.bmp");
                    textureMario = SDL_CreateTextureFromSurface(renderer, surfaceMario);
                    SDL_free(surfaceMario);
                    mario->stepCount++;
                }
                else if (mario->stepCount%2 == 1)
                {
                    surfaceMario = SDL_LoadBMP("src/mario_gauche1.bmp");
                    textureMario = SDL_CreateTextureFromSurface(renderer, surfaceMario);
                    SDL_free(surfaceMario);
                    mario->stepCount++;
                }
            }
        }

        if(mario->isJumping == true){
            if(mario->orientation == 1){
                surfaceMario = SDL_LoadBMP("src/mario_saut_droit.bmp");
                textureMario = SDL_CreateTextureFromSurface(renderer, surfaceMario);
                SDL_free(surfaceMario);
            }
            else if(mario->orientation == -1){
                surfaceMario = SDL_LoadBMP("src/mario_saut_gauche.bmp");
                textureMario = SDL_CreateTextureFromSurface(renderer, surfaceMario);
                SDL_free(surfaceMario);
            }

            neg = 1;
            if(mario->jumpCount >= -9){
                if(mario->jumpCount < 0)
                    neg = -1;
                int valeur = (pow(mario->jumpCount, 2))*0.5 * neg;
                *mario->y -= valeur;
                mario->jumpCount -= 1;
            }
            else{
                mario->isJumping = false;
                mario->jumpCount = 9;

                if(mario->orientation == 1){
                    surfaceMario = SDL_LoadBMP("src/mario_droite.bmp");
                    textureMario = SDL_CreateTextureFromSurface(renderer, surfaceMario);
                    SDL_free(surfaceMario);
                }
                else if(mario->orientation == -1){
                    surfaceMario = SDL_LoadBMP("src/mario_gauche.bmp");
                    textureMario = SDL_CreateTextureFromSurface(renderer, surfaceMario);
                    SDL_free(surfaceMario);
                }
            }
        }

        if(SDL_HasIntersection(&mario->rect, &goomba)){
            fprintf(stderr, "Personnage touché");
        }

        SDL_Delay(30); 
    }

    SDL_DestroyTexture(textureMario);
    free(mario);
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

//Sous macos, saut de 10, sous window saut de 5