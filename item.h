#ifndef ITEM_H
#define ITEM_H
#include <SDL2/SDL.h>
#include <stdbool.h>

typedef struct Item{
    SDL_Rect fond;
    int *x;
    int *y;
    SDL_Texture *texture;
    bool isFalling;

} Item;

#endif