#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <stdio.h>
#include <stdio.h>
#include <stdbool.h>

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

void    Size_Init(Size *this) {
    this->width = 0;
    this->height = 0;
}

void    Size_Destroy(Size *this) {
    this->width = 0;
    this->height = 0;
}

void    Game_Texture_Init(Game_Texture *this) {
    this->texture = NULL;
    Size_Init(&this->size);
} 

void    Game_Screen_Init(Game_Screen *this) {
    this->window = NULL;
    this->renderer = NULL;
}

void    Game_InitSDL(void) {
    if (SDL_Init(SDL_INIT_VIDEO) == false) {
        SDL_Log("SDL could not initialize! %s\n", SDL_GetError());
        exit(-1);
    }
}

void    Game_Init(Game *this) {
    Game_InitSDL();
    Game_Texture_Init(&this->texture);
    Game_Screen_Init(&this->screen);
}



void    Size_Set(size_t *destination, size_t source) {
    *destination = source;
}

void    Game_Texture_SetSize(Game_Texture *this, Size size) {
    Size_Set(&this->size.width, size.width);
    Size_Set(&this->size.height, size.height);
}

size_t  Game_Texture_GetWidth(Game_Texture *this) {
    return (this->size.width);
}

size_t  Game_Texture_GetHeight(Game_Texture *this) {
    return (this->size.height);
}

bool    Game_Texture_IsLoaded(Game_Texture *this) {
    return (this->texture != NULL);
}

void    Game_Texture_Destroy(Game_Texture *this) {
    SDL_DestroyTexture(this->texture);
    this->texture = 0;
    Size_Destroy(&this->size);
}

bool    Game_Texture_LoadFromFile(Game_Texture *this, Game_Screen *screen, const char *path) {
    SDL_Surface     *loaded_surface;

    Game_Texture_Destroy(this);
    loaded_surface = IMG_Load(path);
    if (!loaded_surface) {
            SDL_Log( "Unable to load image %s! SDL_image error: %s\n", path, SDL_GetError());
            return (false);
    }
    this->texture = SDL_CreateTextureFromSurface(screen->renderer, loaded_surface);
    if (!this->texture) {
        SDL_DestroySurface(loaded_surface);
        SDL_Log("Unable to create texture from loaded pixels! SDL error: %s\n", SDL_GetError());
        return (false);
    }
    Game_Texture_SetSize(this, (Size){.width = loaded_surface->w,
                                      .height = loaded_surface->h});
    SDL_DestroySurface(loaded_surface);
    return (true);
}

void    Game_Screen_Render(Game_Screen *this, Game_Texture *texture, Coordinates coordinates) {
    SDL_FRect   rect;

    rect = (SDL_FRect){ coordinates.x,
                        coordinates.y,
                        Game_Texture_GetWidth(texture),
                        Game_Texture_GetHeight(texture)};
    SDL_RenderTexture( this->renderer, texture->texture, NULL, &rect );
}

bool    Game_InitSDL(Game *this) {
    if (SDL_Init( SDL_INIT_VIDEO ) == false) {
        SDL_Log("SDL could not initialize due to this error %s\n", SDL_GetError());
        return (false);
    }
    if (SDL_CreateWindowAndRenderer("Example_3", SCREEN_WIDTH, SCREEN_HEIGHT, 0, &this->screen.window, &this->screen.renderer)) {
        SDL_Log("SDL could not initialize due to this error %s\n", SDL_GetError());
        return (false);
    }
    return (true);
}

bool    Game_Texture_LoadMedia(Game_Texture *this) {
    if (Game_Texture_LoadFromFile(this) == false) {
        SDL_Log("Unable to load png image!!!\n");
        return (false);
    }
    return (true);
}

void    Game_Screen_Destroy(Game_Screen *this) {
    SDL_DestroyRenderer(this->renderer);
    this->renderer = NULL;
    SDL_DestroyWindow(this->window);
    this->window = NULL;
}

void    Game_Quit(void) {
    SDL_Quit();
    exit(0);
}

void    Game_Destroy(Game *this) {
    Game_Texture_Destroy(this->texture);
    Game_Screen_Destroy(this->screen);
    Game_Quit();
}

void    SDL_Event_Init(SDL_Event *event) {
    SDL_zero(*event);
}

void    Game_Screen_Fill(Game_Screen *this) {
    SDL_SetRenderDrawColor(this->render, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(this->render);
}

void    Game_Render(Game_Screen *this) {
    Game_Screen_Render(this->renderer);
}

void    Game_Screen_Update(Game_Screen *this) {
    SDL_RenderPresent(this->renderer);
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

int     main(void)  {
    Game    game;

   Game_Init(&game);
   Game_Destroy(&game);
   Game_Quit();
}