#include "game.h"

void    Game_Init(Game *this) {
    Game_InitSDL();
    Game_Texture_Init(&this->texture);
    Game_Screen_Init(&this->screen);
}

void    Game_Loop(Game *game) {
    bool    running;
    SDL_Event event;

    running = false;
    SDL_Event_Init(&event);
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) 
                running = false;            
        }
    }

}

void    Game_Quit(void) {
    SDL_Quit();
    exit(0);
}