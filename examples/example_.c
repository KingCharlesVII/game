#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define MAX_ERROR_NUMBER 32

typedef enum Game_Error_Code {
    GAME_OK,
    GAME_ALLOCATION_ERROR,
    GAME_INT_RANGE_ERROR,
    GAME_SDL_ERROR,
}   Game_Error_Code;

typedef struct Game_Error {
    Game_Error_Code     code[MAX_ERROR_NUMBER];
    size_t  length;
}   Game_Error;

typedef struct Game_Error_Context {
    void    (*delete)(void *);
    void    *object;
    Game_Error_Code error_code;
}   Game_Error_Context;

#define GAME_ERROR_CONTEXT(delete, object, code) (Game_Error_Context){delete, object, code}

void    Game_Error_Init(Game_Error *this) {
    for(size_t index = 0; index < MAX_ERROR_NUMBER; index++)
        this->code[index] = GAME_OK;
    this->length = 0;
}

void    Game_Error_Append(Game_Error *this, Game_Error_Code error_code) {
    if (this->length < MAX_ERROR_NUMBER)
        this->code[this->length++] = error_code;
}

void    Game_Error_Code_PrintMessage(Game_Error_Code this) {
    if (this == GAME_ALLOCATION_ERROR) {
        printf("Error during allocation of memory\n");
        return ;
    }
    if (this == GAME_INT_RANGE_ERROR) {
        printf("Error in range of integers\n");
        return ;
    }
    if (this == GAME_SDL_ERROR) {
        printf("Error with execution of SLD\n");
        return ;
    }
}

void    Game_Error_Log(Game_Error *this) {
    for(size_t index = 0; index < this->length; index++)
        Game_Error_Code_PrintMessage(this->code[index]);
}

int    Game_Error_CleanUp(Game_Error *this, Game_Error_Context context) {
    assert(this);
    Game_Error_Append(this, context.error_code);
    if (context.delete)
        context.delete(context.object);
    return (context.error_code);
}

void    Game_Error_Clear(Game_Error *this) {
    assert(this);
    for(size_t index = 0; index < MAX_ERROR_NUMBER; index++)
        this->code[index] = GAME_OK;
    this->length = 0;
}

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>

#define NO_FLAG 0

typedef struct Size {
    size_t  width;
    size_t  height;
}   Size;

void    Size_SetWidth(Size *this, size_t width) {
    this->width = width;
}

void    Size_SetHeight(Size *this, size_t height) {
    this->height = height;
}

void    Size_Init(Size *this) {
    Size_SetWidth(this, 0);
    Size_SetWidth(this, 0);
}

typedef struct Game_Window {
    SDL_Window  *sdl_window;
    Size    size;
}   Game_Window;

void    Game_Window_Init(Game_Window *this) {
    this->sdl_window = NULL;
    Size_Init(&this->size);
}

int     Game_Window_Create(Game_Window *this) {
    if ((this->sdl_window = SDL_CreateWindow( "game",
                                         this->size.width,
                                         this->size.height,
                                         NO_FLAG) == NULL)
        return ()
    return (0);
}

void    Game_Window_Destroy(Game_Window *this) {
    SDL_DestroyWindow(this->sdl_window);
}

typedef struct Game {
    Game_Window     window;
    Game_Error      error;
}   Game;

void    Game_InitSLD(void) {
    SDL_Init(SDL_INIT_VIDEO);
}

void    Game_Init(Game *this) {
    Game_InitSDL();
    Game_Window_Init(&this->window);
    Game_Error_Init(&this->error);
}

void    Game_Quit(Game *this) {
    Game_Window_Destroy(&this->window);
    SDL_Quit();
}

int     main(void) {

}