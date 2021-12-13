#ifndef MENU_H
#define MENU_H
#include <stdbool.h>
#include <SDL2/SDL.h>


bool menu()
{
    int continuer = 0;
    int menu = 1;
    SDL_Window *windowMenu = NULL;
    windowMenu = SDL_CreateWindow("Super Brechi : MENU", SDL_WINDOWPOS_CENTERED,
                                                                SDL_WINDOWPOS_CENTERED,
                                                                996,
                                                                372,
                                                                SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);


    SDL_Renderer *rendererMenu = SDL_CreateRenderer(windowMenu, -1, SDL_RENDERER_ACCELERATED);
    SDL_Surface *surfaceMenu = SDL_LoadBMP("src/menu.bmp");
    SDL_Texture *textureMenu = SDL_CreateTextureFromSurface(rendererMenu, surfaceMenu);
    SDL_RenderCopy(rendererMenu, textureMenu, NULL, NULL);
    SDL_RenderPresent(rendererMenu);
    SDL_RenderClear(rendererMenu);

    SDL_Event event;


    while(menu)
    {
        SDL_PollEvent(&event);
        SDL_Delay(100);
        switch(event.type)
        {
        case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                    case SDLK_p:
                        SDL_DestroyTexture(textureMenu);
                        SDL_DestroyRenderer(rendererMenu);
                        SDL_DestroyWindow(windowMenu);
                        menu = 0;
                        return true;

                        break;
                    case SDLK_q:
                        SDL_DestroyTexture(textureMenu);
                        SDL_DestroyRenderer(rendererMenu);
                        SDL_DestroyWindow(windowMenu);
                        SDL_Quit();
                        menu = 0;
                        return false;

                        break;

                    case SDLK_ESCAPE:
                        SDL_DestroyTexture(textureMenu);
                        SDL_DestroyRenderer(rendererMenu);
                        SDL_DestroyWindow(windowMenu);
                        menu = 0;
                        return false;

                        break;

                }
        break;
        }
    }
    return false;
}

#endif