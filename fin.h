#ifndef FIN_H
#define FIN_H
#include <stdbool.h>
#include <SDL2/SDL.h>


void fin()
{
    bool cont = true; //variable pour rentrer dans la boucle infinie
    SDL_Window *window_fin = NULL; //création un pointeur de type SDL_Window
    window_fin = SDL_CreateWindow("Super ZenMa : FIN", SDL_WINDOWPOS_CENTERED,
                                                                SDL_WINDOWPOS_CENTERED,
                                                                512,
                                                                301,
                                                                SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE); //création fenetre


    SDL_Renderer *rendererFin = SDL_CreateRenderer(window_fin, -1, SDL_RENDERER_ACCELERATED); //crée renderer
    SDL_Surface *surfaceFin = SDL_LoadBMP("src/game_over.bmp"); //crée surface
    SDL_Texture *textureFin = SDL_CreateTextureFromSurface(rendererFin, surfaceFin); //crée texture avec renderer et surface
    SDL_RenderCopy(rendererFin, textureFin, NULL, NULL);
    SDL_RenderPresent(rendererFin);
    SDL_RenderClear(rendererFin);
    SDL_Event event;

    while(cont)
    {
        SDL_WaitEvent(&event);

        switch(event.type)
        {
            case SDL_QUIT: //si on appuie sur la croix alors...
                cont = 0; //quitte la boucle while
                break;

            case SDL_KEYDOWN: //récupère appuie touche
                switch(event.key.keysym.sym)
                {
                    case SDLK_ESCAPE: //touche échap alors...
                        cont = 0; //quitte la boucle while
                        break;
                }
        }
    }
    SDL_DestroyTexture(textureFin); //libère la texture de la mémoire
    SDL_DestroyRenderer(rendererFin); //libère le renderer de la mémoire
    SDL_DestroyWindow(window_fin); //libère la window de la mémoire
}

#endif