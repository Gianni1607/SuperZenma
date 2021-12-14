#ifndef MUSIQUE_H
#define MUSIQUE_H
#include <SDL2/SDL_mixer.h>
#include <stdbool.h>

void run_music(Mix_Music **musique, char music_path[20], bool pause){
    if(pause){
        Mix_FreeMusic(*musique);
        Mix_PauseMusic();
    }
    *musique = Mix_LoadMUS(music_path);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 2);
    Mix_PlayMusic(*musique, 1);
}


#endif