#include <SDL3/SDL.h>
#include <stdbool.h>

int main(void)
{
    /* Logs détaillés (API SDL3) */
    SDL_SetLogPriorities(SDL_LOG_PRIORITY_VERBOSE);

    /* Forcer X11 */
    SDL_SetHint(SDL_HINT_VIDEO_DRIVER, "x11");

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("SDL_Init a échoué: %s", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow(
        "Fenêtre SDL3",
        800,
        600,
        SDL_WINDOW_RESIZABLE
    );

    if (!window) {
        SDL_Log("SDL_CreateWindow a échoué: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    bool running = true;
    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT)
                running = false;
        }
        SDL_Delay(16);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

