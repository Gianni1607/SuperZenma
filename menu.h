#ifndef MENU_H
#define MENU_H
#include <math.h> //include math pour utiliser "%"
#include <stdbool.h> //inclue bool pour pouvoir utiliser des boolean
#include <SDL2/SDL.h> //include la sdl pour afficher l'image


bool menu()
{
    int cont = 1; //init cont de type integer
    int choix = 0; //init choix de type integer
    SDL_Window *windowMenu = NULL; //init un poiteur qui pointe sur NULL de type SDL_Window
    windowMenu = SDL_CreateWindow("Super ZenMa : MENU", SDL_WINDOWPOS_CENTERED,
                                                                SDL_WINDOWPOS_CENTERED,
                                                                996,
                                                                372,
                                                                SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE); //création fenetre menu


    SDL_Renderer *rendererMenu = SDL_CreateRenderer(windowMenu, -1, SDL_RENDERER_ACCELERATED); //création renderer
    SDL_Surface *surfaceMenu = SDL_LoadBMP("src/menu.bmp"); //load l'image menu.bmp
    SDL_Texture *textureMenu = SDL_CreateTextureFromSurface(rendererMenu, surfaceMenu); //création d'une texture avec renderer et surface

    SDL_Rect fleche1; //rect contenant la fleche
    fleche1.x = 120; //position en x
    fleche1.y = 65; //position en y
    fleche1.h = 45; //longueur
    fleche1.w = 54; //largeur


    SDL_Surface *surfaceFleche = SDL_LoadBMP("src/fleche.bmp"); //load l'image fleche
    SDL_Texture *textureFleche = SDL_CreateTextureFromSurface(rendererMenu, surfaceFleche); //création d'une texture avec le renderer et l'image

    SDL_Event event;


    while(cont)
    {
        SDL_RenderCopy(rendererMenu, textureMenu, NULL, NULL);
        SDL_RenderCopy(rendererMenu, textureFleche, NULL, &fleche1);
        SDL_RenderPresent(rendererMenu);
        SDL_RenderClear(rendererMenu);
        SDL_PollEvent(&event);

        switch(event.type)
        {
        case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                case SDLK_UP : //si fleche du haut appuyé alors...
                    if(choix%2 == 0) //si choix %2 == 0 alors fleche descend de qq pixels en y et choix ++
                    {
                        fleche1.y = 110; //nouvelle position en y de la flèche
                        choix ++;//incrémentation choix
                    }
                    else if(choix%2 == 1) //si choix %2 == 0 alors fleche monte de qq pixels en y et choix ++
                    {
                        fleche1.y = 65;//nouvelle position en y de la flèche
                        choix ++;//incrémentation choix
                    }
                    break;
                case SDLK_DOWN : //si fleche du bas appuyé alors...
                    if(choix%2 == 0) //si choix %2 == 0 alors fleche descend de qq pixels en y et choix ++
                    {
                        fleche1.y = 110;//nouvelle position en y de la flèche
                        choix ++;//incrémentation choix
                    }
                    else if(choix%2 == 1) //si choix %2 == 0 alors fleche monte de qq pixels en y et choix ++
                    {
                        fleche1.y = 65;//nouvelle position en y de la flèche
                        choix ++;//incrémentation choix
                    }
                    break;
                case SDLK_RETURN : //si entrée appuyé alors ...
                    if(choix%2 == 0) //si choix%2 = 0 alors on alnce le jeu grace a return true
                    {
                        SDL_DestroyTexture(textureMenu); //libère la texture de la ram
                        SDL_DestroyRenderer(rendererMenu); //libère le renderer de la ram
                        SDL_DestroyWindow(windowMenu); //libère la window de la ram
                        cont = 0; //quitte la boucle while
                        return true; //on lance le jeu grace a continuer = true
                        break;
                    }
                    else if(choix%2 == 1) // on ferme la fenetre et on ne lance pas le jeu
                    {
                        SDL_DestroyTexture(textureMenu); //libère la texture de la ram
                        SDL_DestroyRenderer(rendererMenu); //libère le renderer de la ram
                        SDL_DestroyWindow(windowMenu); //libère la window de la ram
                        cont = 0; //quitte la boucle while
                        return false; //ne lance pas le jeu
                    }
                case SDLK_ESCAPE: //echape : on ferme le menu
                    SDL_DestroyTexture(textureMenu); //libère la texture de la ram
                    SDL_DestroyRenderer(rendererMenu); //libère le renderer de la ram
                    SDL_DestroyWindow(windowMenu); //libère la window de la ram
                    cont = 0; //quitte la boucle while
                    return false; //ne lance pas le jeu
                    break;
            }
        break;
        }
    }
    return false;
}
#endif
