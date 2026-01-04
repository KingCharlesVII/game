#ifndef GAME_H
# define GAME_H

# include "libstd.h"
# include "SDL_lib.h"

# define SCREEN_WIDTH 640
# define SCREEN_HEIGHT 480

typedef struct Size {
    size_t  width;
    size_t  height;
}   Size;

typedef struct Coordinates {
    float  x;
    float  y;
}   Coordinates;

typedef struct Game_Texture {
    SDL_Texture     *texture;
    Size        size;
}   Game_Texture;

typedef struct Game_Screen {
    SDL_Window *window;
    SDL_Renderer    *renderer;
}   Game_Screen;

typedef struct Game {
    Game_Screen screen;
    Game_Texture    texture;   
}   Game;

#endif