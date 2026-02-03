/******************************************************************************

                            Online C Compiler.
                Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include "game_error.h"

void    Game_Error_Init(Game_Error *this) {
    for(size_t index = 0; index < MAX_ERROR_NUMBER; index++)
        this->code[index] = GAME_OK;
    this->length = 0;
}

void    Game_Error_Append(Game_Error *this, Game_Error_Code error_code) {
    if (this->length < MAX_ERROR_NUMBER)
        this->code[this->length++] = error_code;
}

bool    Game_Error_Failure(Game_Error *this, Game_Error_Code error_code) {
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
        SDL_Log("%s\n", SDL_GetError());
        printf("Error with execution of SLD\n");
        return ;
    }
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

int   Game_Error_Log(Game_Error *this) {
    for(size_t index = 0; this->code[index] != GAME_OK; index++)
        Game_Error_Code_PrintMessage(this->code[index]);
    Game_Error_Clear(this);
    return (-1);
}

#ifdef ERROR

int main(void)
{
    Game_Error error;
    
    Game_Error_Init(&error);
    Game_Error_Append(&error, GAME_SDL_ERROR);
    Game_Error_Append(&error, GAME_SDL_ERROR);
    Game_Error_Append(&error, GAME_SDL_ERROR);
    Game_Error_CleanUp(&error, GAME_ERROR_CONTEXT(NULL, NULL, GAME_SDL_ERROR));
    Game_Error_Log(&error);
    return 0;
}

#endif