#ifndef FIN_H
#define FIN_H
#include <stdbool.h>
#include <SDL2/SDL.h>


void fin()
{
    bool cont = true;
    SDL_Window *window_fin = NULL;
    window_fin = SDL_CreateWindow("Super Brechi : FIN", SDL_WINDOWPOS_CENTERED,
                                                                SDL_WINDOWPOS_CENTERED,
                                                                512,
                                                                301,
                                                                SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);


    SDL_Renderer *rendererFin = SDL_CreateRenderer(window_fin, -1, SDL_RENDERER_ACCELERATED);
    SDL_Surface *surfaceFin = SDL_LoadBMP("src/game_over.bmp");
    SDL_Texture *textureFin = SDL_CreateTextureFromSurface(rendererFin, surfaceFin);
    SDL_RenderCopy(rendererFin, textureFin, NULL, NULL);
    SDL_RenderPresent(rendererFin);
    SDL_RenderClear(rendererFin);

    while(cont)
    {
        SDL_Event event;
        SDL_PollEvent(&event);

        switch(event.type)
        {
            case SDL_QUIT:
                cont = 0;
                break;
            
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        cont = 0;
                        break;
                }
        }
    }
    SDL_DestroyTexture(textureFin);
    SDL_DestroyRenderer(rendererFin);
    SDL_DestroyWindow(window_fin);

}

#endif