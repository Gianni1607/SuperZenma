#ifndef WIN_H
#define WIN_H
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

void win()
{
    bool cont = true; //variable pour la boucle while dans la suite
    SDL_Window *window_fin = NULL; //crée un pointeur de type SDL_Window
    window_fin = SDL_CreateWindow("Super ZenMa : GG !", SDL_WINDOWPOS_CENTERED,
                                                                SDL_WINDOWPOS_CENTERED,
                                                                852,
                                                                480,
                                                                SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE); //création d'une fenetre vide


    SDL_Renderer *rendererFin = SDL_CreateRenderer(window_fin, -1, SDL_RENDERER_ACCELERATED); //création d'un renderer
    SDL_Surface *surfaceFin = SDL_LoadBMP("src/mario_win.bmp"); //charge l'image mario_win
    SDL_Texture *textureFin = SDL_CreateTextureFromSurface(rendererFin, surfaceFin); //charge la texture avec le renderer et l'image
    SDL_RenderCopy(rendererFin, textureFin, NULL, NULL); //ajouter un element au renderer
    SDL_RenderPresent(rendererFin); //actualise le renderer
    SDL_RenderClear(rendererFin); //efface tout le contenu du renderer
    SDL_Event event; //variable qui contient les evenements de la fenetre

    while(cont) //boucle infinie
    {
        SDL_WaitEvent(&event);

        switch(event.type) //récupère les events
        {
            case SDL_QUIT: //si on appuie sur la croix, ca quitte la fenetre
                cont = 0; //quitte la boucle while
                break;

            case SDL_KEYDOWN: //récupère les touches du clavier appuyé
                switch(event.key.keysym.sym)
                {
                    case SDLK_ESCAPE: //cas où la touche échape est appuyé
                        cont = 0; //quitte la boucle while
                        break;
                }
        }
    }


    SDL_DestroyTexture(textureFin); //retire la texture de la ram
    SDL_DestroyRenderer(rendererFin); //retire le renderer de la ram
    SDL_DestroyWindow(window_fin); //retire la window de la ram

}

#endif