#ifndef CODE_ERROR_H
#define CODE_ERROR_H

#include "libstd.h"
#include "SDL_lib.h"

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

void    Game_Error_Init(Game_Error *this);
void    Game_Error_Append(Game_Error *this, Game_Error_Code error_code);
void    Game_Error_Code_PrintMessage(Game_Error_Code this);
int    Game_Error_Log(Game_Error *this);
int     Game_Error_CleanUp(Game_Error *this, Game_Error_Context context);
void    Game_Error_Clear(Game_Error *this);
bool    Game_Error_Failure(Game_Error *this, Game_Error_Code error_code);

#endif