#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

# define SCREEN_WIDTH 640
# define SCREEN_HEIGHT 480

typedef struct game_window {
    SDL_Window *window;
    SDL_Surface *screen_surface;
    SDL_Surface *hello_world;
}   game_window;

void    game_window_init(game_window *window) {
    window->window = NULL;
    window->screen_surface = NULL;
    window->hello_world = NULL;
}

void    game_SDL_init(void) {
    if (SDL_Init(SDL_INIT_VIDEO) == false) {
        SDL_Log("SDL could not initialize! SDL error: %s\n", SDL_GetError());
        exit(-1);
    }
}

void    game_SDL_window_init(game_window *window) {
    window->window = SDL_CreateWindow("Example", SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (!window->window) {
        SDL_Log("Window could not be created: %s\n", SDL_GetError());
        exit(-1);
    }
    window->screen_surface = SDL_GetWindowSurface(window->window);
}

void  game_init(game_window *window) {
    game_SDL_init();
    game_window_init(window);
    game_SDL_window_init(window);
}

bool    game_load_media(game_window *window) {
    char    *path;

    path = "boo.bmp";
    window->hello_world = SDL_LoadBMP(path);
    if (!window->hello_world) {
        SDL_Log("Unable to load the image %s! SDL Error %s\n", path, SDL_GetError());
        return (false);
    }
    return (true);
}

void    game_destroy(game_window *window) {
    SDL_DestroySurface(window->hello_world);
    window->hello_world = NULL;
    SDL_DestroyWindow(window->window);
    window->window = NULL;
    window->screen_surface = NULL;
    SDL_Quit();
}

void    game_update(game_window *window) {
    SDL_FillSurfaceRect(window->screen_surface, NULL, SDL_MapSurfaceRGB(window->screen_surface, 0xFF, 0xFF, 0xFF));
    SDL_BlitSurface(window->hello_world, NULL, window->screen_surface, NULL);
    SDL_UpdateWindowSurface(window->window);
}

void    game_loop(game_window   *window) {
    bool    running;
    SDL_Event event;

    SDL_zero(event);
    running = true;
    while (running) {
        while (SDL_PollEvent(&event) == true) {
            if (event.type == SDL_EVENT_QUIT)
                running = false;
            game_update(window);
        }
    }
}

int main(void) {
    game_window window;

    game_init(&window);
    if (game_load_media(&window) == false) {
        game_destroy(&window);
        return (1);
    }
    game_loop(&window);
    game_destroy(&window);
}