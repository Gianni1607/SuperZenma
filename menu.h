#ifndef MENU_H
#define MENU_H
#include <math.h>
#include <stdbool.h>
#include <SDL2/SDL.h>


bool menu()
{
    int cont = 1;
    int choix = 0;
    SDL_Window *windowMenu = NULL;
    windowMenu = SDL_CreateWindow("Super Brechi : MENU", SDL_WINDOWPOS_CENTERED,
                                                                SDL_WINDOWPOS_CENTERED,
                                                                996,
                                                                372,
                                                                SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);


    SDL_Renderer *rendererMenu = SDL_CreateRenderer(windowMenu, -1, SDL_RENDERER_ACCELERATED);
    SDL_Surface *surfaceMenu = SDL_LoadBMP("src/menu.bmp");
    SDL_Texture *textureMenu = SDL_CreateTextureFromSurface(rendererMenu, surfaceMenu);

    SDL_Rect fleche1;
    fleche1.x = 120;
    fleche1.y = 65;
    fleche1.h = 45;
    fleche1.w = 54;


    SDL_Surface *surfaceFleche = SDL_LoadBMP("src/fleche.bmp");
    SDL_Texture *textureFleche = SDL_CreateTextureFromSurface(rendererMenu, surfaceFleche);



    SDL_Event event;


    while(cont)
    {
        SDL_RenderCopy(rendererMenu, textureMenu, NULL, NULL);
        SDL_RenderCopy(rendererMenu, textureFleche, NULL, &fleche1);
        SDL_RenderPresent(rendererMenu);
        SDL_RenderClear(rendererMenu);
        SDL_PollEvent(&event);
        SDL_Delay(50);

        switch(event.type)
        {
        case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                case SDLK_UP :
                    if(choix%2 == 0)
                    {
                        fleche1.y = 110;
                        choix ++;
                    }
                    else if(choix%2 == 1)
                    {
                        fleche1.y = 65;
                        choix ++;
                    }
                    break;
                case SDLK_DOWN :
                    if(choix%2 == 0)
                    {
                        fleche1.y = 110;
                        choix ++;
                    }
                    else if(choix%2 == 1)
                    {
                        fleche1.y = 65;
                        choix ++;
                    }
                    break;
                case SDLK_RETURN :
                    if(choix%2 == 0)
                    {
                        SDL_DestroyTexture(textureMenu);
                        SDL_DestroyRenderer(rendererMenu);
                        SDL_DestroyWindow(windowMenu);
                        cont = 0;
                        return true;
                        break;
                    }
                    else if(choix%2 == 1)
                    {
                        SDL_DestroyTexture(textureMenu);
                        SDL_DestroyRenderer(rendererMenu);
                        SDL_DestroyWindow(windowMenu);
                        cont = 0;
                        return false;
                    }
                case SDLK_ESCAPE:
                    SDL_DestroyTexture(textureMenu);
                    SDL_DestroyRenderer(rendererMenu);
                    SDL_DestroyWindow(windowMenu);
                    cont = 0;
                    return false;
                    break;
            }
        break;
        }
    }
    return false;
}
#endif