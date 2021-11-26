#ifndef PERSONNAGE_H
#define PERSONNAGE_H
#include <SDL2/SDL.h>
#include <stdbool.h>

typedef struct personnage
{
    SDL_Rect rect;
    int *x;
    int *y;
    int orientation;
    int jumpCount;
    int stepCount;
    bool isJumping;
    bool isWalkingLeft;
    bool isWalkingRight;

} Perso;

int test(void);

#endif