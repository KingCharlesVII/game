#include "SDL_lib.h"
#include "libstd.h"
#include "game_error.h"

#define DEFAULT_FPS  60
#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

#define PATH_SPRITE_ROOM "../../sprites/room.bmp"
#define PATH_SPRITE_FLOOR "../../sprites/floor.bmp"
#define PATH_SPRITE_PLAYER "../../sprites/player.bmp"

#define TEXTURES_NUMBER 2

typedef enum  Sprite_Code {
    FLOOR,
    PLAYER,
    SPRITE_NUMBER,
}   Sprite_Code;

static const char  *texture_path[SPRITE_NUMBER] = {
    PATH_SPRITE_FLOOR,
    PATH_SPRITE_PLAYER
};

typedef struct Size {
    size_t  width;
    size_t  height;
}   Size;

typedef struct Coordinates {
    float   x;
    float   y;
}   Coordinates;

typedef struct Game_Timer {
    size_t  screen_fps;
    size_t  ns_per_frame;
    size_t  rendering_ns;
    size_t  start_ticks;
    size_t  paused_ticks;
    bool    is_paused;
    bool    is_started;
}   Game_Timer;

typedef struct Game_Surface {
    SDL_Surface *content;
    Game_Error  error;
}   Game_Surface;

typedef struct Game_Texture {
    SDL_Texture *content;
    SDL_FRect   rectangle;
    const char *path;
    Game_Error  error;
    Size    size;
}   Game_Texture;

typedef struct Game_Player {
    Game_Texture    texture;
    Coordinates        coordinates;
    float   speed;
}   Game_Player;

typedef struct Game_Floor {
    Game_Texture    texture;
    Coordinates coordinates;
}   Game_Floor;

typedef struct Game_Texture_Array {
    Game_Texture    content[TEXTURES_NUMBER];
    size_t  capacity;
}   Game_Texture_Array;

typedef struct Game_Window {
    bool    vsync_enabled;
    SDL_Window  *content;
    SDL_Renderer *renderer;
}   Game_Window;

typedef struct Game {
    Game_Window window;
    Game_Timer timer;
    Game_Texture_Array  textures;
    Game_Player player;
    Game_Floor  floor;
    Game_Error error;
}   Game;

void    Size_Set(Size *this, size_t width, size_t height) {
    this->width = width;
    this->height = height;
}

void    Size_Init(Size *this) {
    this->width = 0;
    this->height = 0;
}

size_t  Size_GetWidth(Size *this) {
    return (this->width);
}

size_t  Size_GetHeight(Size *this) {
    return (this->height);
}

void    Coordinates_Init(Coordinates *this) {
    this->x = 0;
    this->y = 0;
}

float    Coordinates_Getx(Coordinates *this) {
    return (this->x);
}

float   Coordinates_Gety(Coordinates *this) {
    return (this->y);
}

void   Coordinates_Setx(Coordinates *this, float x) {
    this->x = x;
}

void    Coordinates_Sety(Coordinates *this, float y) {
    this->y = y;
}

void    Game_Timer_Init(Game_Timer *this) {
    this->screen_fps = DEFAULT_FPS;
    this->ns_per_frame = 1000000000 / this->screen_fps;
    this->rendering_ns = 0;
    this->start_ticks = 0;
    this->paused_ticks = 0;
    this->is_paused = false;
    this->is_started = false;
}

void    Game_Texture_Init(Game_Texture *this) {
    Game_Error_Init(&this->error);
    this->rectangle = (SDL_FRect){0, 0, 0, 0};
    this->content = NULL;
    Size_Init(&this->size);
}

void    Game_Player_Init(Game_Player *this) {
    Coordinates_Init(&this->coordinates);
    Game_Texture_Init(&this->texture);
    this->texture.path = PATH_SPRITE_PLAYER;
    this->speed = 8.f;
}

void    Game_Floor_Init(Game_Floor  *this) {
    Coordinates_Init(&this->coordinates);
    Game_Texture_Init(&this->texture);
    this->texture.path = PATH_SPRITE_FLOOR;
}

void    Game_Surface_Init(Game_Surface  *this) {
    Game_Error_Init(&this->error);
    this->content = NULL;
}

void    Game_Window_Init(Game_Window *this) {
    if (SDL_CreateWindowAndRenderer("test", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_FULLSCREEN, &this->content, &this->renderer) == false)
        exit(-1);
    /*if (SDL_SetRenderVSync(this->renderer, 1) == false)
        exit(-1);*/
}

void    Game_Texture_Array_Init(Game_Texture_Array *this) {
    this->capacity = TEXTURES_NUMBER;
    for (size_t index = 0; index < this->capacity; index++)
        Game_Texture_Init(&this->content[index]);
}

void    Game_Init(Game *this) {
    if (SDL_Init(SDL_INIT_VIDEO) == false)
        exit(-1);
    Game_Error_Init(&this->error);
    Game_Window_Init(&this->window);
    Game_Texture_Array_Init(&this->textures);
    Game_Player_Init(&this->player);
    Game_Floor_Init(&this->floor);
    Game_Timer_Init(&this->timer);
}
 /*--------------------------------------------------*/

void    Game_Timer_Start(Game_Timer *this) {
    this->is_started = true;
    this->is_paused = false;
    this->start_ticks = SDL_GetTicksNS();
    this->paused_ticks = 0;
}

void    Game_Timer_Stop(Game_Timer *this) {
    this->is_started = false;
    this->is_paused = true;
    this->start_ticks = 0;
    this->paused_ticks = 0;
}

void    Game_Timer_Pause(Game_Timer *this) {
    if (this->is_started && !this->is_paused) {
        this->is_paused = true;
        this->paused_ticks = SDL_GetTicksNS() - this->start_ticks;
        this->start_ticks = 0;
    }
}

void    Game_Timer_Unpause(Game_Timer *this) {
    if (this->is_started && this->is_paused) {
        this->is_paused = false;
        this->start_ticks = SDL_GetTicksNS() - this->paused_ticks;
        this->paused_ticks = 0;
    }
}

size_t  Game_Timer_GetTicksNS(Game_Timer *this) {
    size_t  time;

    time = 0;
    if (this->is_started) {
        if (this->is_paused)
            time = this->paused_ticks;
        else
            time = SDL_GetTicksNS() - this->start_ticks;
    }
    return (time);
}

bool    Game_Timer_IsPaused(Game_Timer *this) {
    return (false);
}

bool    Game_Timer_IsStarted(Game_Timer *this) {
    return (false);
}

size_t  Game_Timer_Elapsed(Game_Timer *this) {
    return (SDL_GetTicksNS() - this->start_ticks);
}

void    Game_Timer_Sync(Game_Timer *this) {
    size_t  elapsed;

    elapsed = Game_Timer_Elapsed(this);
    if (elapsed < this->ns_per_frame)
        SDL_DelayNS(this->ns_per_frame - elapsed);
    this->start_ticks = SDL_GetTicksNS();
}

void    Game_Timer_Sleep(Game_Timer *this) {
    size_t  sleep_time;

    this->rendering_ns = Game_Timer_GetTicksNS(this);
    if (this->rendering_ns < this->ns_per_frame) {
        sleep_time = this->ns_per_frame - this->rendering_ns;
        SDL_DelayNS(this->ns_per_frame - this->rendering_ns);
        this->rendering_ns = Game_Timer_GetTicksNS(this);
    }
}

 /*---------------------------------------------------*/
bool    Game_Surface_LoadMedia(Game_Surface *this) {
    if ((this->content = SDL_LoadBMP("../../sprites/room.bmp")) == NULL)
        return (Game_Error_Failure(&this->error, GAME_SDL_ERROR));
    return (true);
}

void    Game_Surface_Destroy(Game_Surface *this) {
    SDL_DestroySurface(this->content);
    this->content = NULL;
}

void    Game_Window_Destroy(Game_Window *this) {
    SDL_DestroyWindow(this->content);
    SDL_DestroyRenderer(this->renderer);
    this->content = NULL;
    this->renderer = NULL;
}

void    Game_Texture_Destroy(Game_Texture *this) {
    SDL_DestroyTexture(this->content);
    this->content = NULL;
}

void    Game_Texture_Array_Destroy(Game_Texture_Array *this, size_t index) {
    for (size_t local_index = 0; local_index < index; local_index++)
        Game_Texture_Destroy(&this->content[local_index]);
}

void    Game_Quit(Game *this) {
    Game_Texture_Array_Destroy(&this->textures, this->textures.capacity);
    Game_Window_Destroy(&this->window);
    SDL_Quit();
}

/*void    Game_Texture_UpdateRectangle(Game_Texture *this, float x, float y) {
    this->rectangle = (SDL_FRect){x, y, (float)this->size.width, (float)this->size.height};
}

void    Game_Render_Texture(Game_Texture *this, Game_Window *window, float x, float y) {
    Game_Texture_UpdateRectangle(this, x, y);
    SDL_RenderTexture(window->renderer, this->content, NULL, &this->rectangle);
}

void    Game_Texture_Array_Render(Game_Texture_Array *this, Game_Window *window) {
    for (size_t index = 0; index < this->capacity; index++)
        Game_Render_Texture(&this->content[index], window, 200.f, 200.f);
}
*/

void    Game_Texture_UpdateRectangle(Game_Texture *this, Coordinates coordinates) {
    this->rectangle = (SDL_FRect){coordinates.x, coordinates.y, (float)this->size.width, (float)this->size.height};
}

void    Game_Texture_Render(Game_Texture *this, Coordinates coordinates, Game_Window *window) {
    Game_Texture_UpdateRectangle(this, coordinates);
    SDL_RenderTexture(window->renderer, this->content, NULL, &this->rectangle);
}

void    Game_Update(Game *this) {
    SDL_SetRenderDrawColor(this->window.renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(this->window.renderer);
    Game_Texture_Render(&this->floor.texture, this->floor.coordinates, &this->window);
    Game_Texture_Render(&this->player.texture, this->player.coordinates, &this->window);
    SDL_RenderPresent(this->window.renderer);
}
/*-----------------------------------------------------------*/

void    Game_Player_MoveRight(Game_Player *this) {
    this->coordinates.x += this->speed;
}

void    Game_Player_MoveLeft(Game_Player *this) {
    this->coordinates.x -= this->speed;
}

void    Game_Player_MoveUp(Game_Player *this) {
    this->coordinates.y -= this->speed;
}

void    Game_Player_MoveDown(Game_Player *this) {
    this->coordinates.y += this->speed;
}

void    Game_Player_SlideRight(Game_Player *this) {
    this->coordinates.x += this->speed / 2;
}

void    Game_Player_SlideLeft(Game_Player *this) {
    this->coordinates.x -= this->speed / 2;
}

void    Game_Player_SlideUp(Game_Player *this) {
    this->coordinates.y -= this->speed / 2;
}

void    Game_Player_SlideDown(Game_Player *this) {
    this->coordinates.y += this->speed / 2;
}

 /*----------------------------------------------------------*/

typedef struct Game_Command {
    void    (*execute)(struct Game_Command *this);    
}   Game_Command;

typedef struct Game_Command_Move_Up {
    Game_Command    base;
    Game_Player     *player;
}   Game_Command_Move_Up;

typedef struct Game_Command_Move_Down {
    Game_Command    base;
    Game_Player     *player;
}   Game_Command_Move_Down;

typedef struct Game_Command_Move_Right {
    Game_Command    base;
    Game_Player     *player;
}   Game_Command_Move_Right;

typedef struct Game_Command_Move_Left {
    Game_Command    base;
    Game_Player     *player;
}   Game_Command_Move_Left;

void    Game_Command_ExecuteMoveUp(Game_Command *this) {
    Game_Command_Move_Up  *command;
    
    command = (Game_Command_Move_Up *)this;
    Game_Player_SlideUp(command->player);
    Game_Player_MoveLeft(command->player);
}

void    Game_Command_ExecuteMoveDown(Game_Command *this) {
    Game_Command_Move_Down  *command;
    
    command = (Game_Command_Move_Down *)this;
    Game_Player_SlideDown(command->player);
    Game_Player_MoveRight(command->player);
}

void    Game_Command_ExecuteMoveRight(Game_Command *this) {
    Game_Command_Move_Right  *command;
    
    command = (Game_Command_Move_Right *)this;
    Game_Player_MoveRight(command->player);
    Game_Player_SlideUp(command->player);
}

void    Game_Command_ExecuteMoveLeft(Game_Command *this) {
    Game_Command_Move_Left  *command;
    
    command = (Game_Command_Move_Left *)this;
    Game_Player_MoveLeft(command->player);
    Game_Player_SlideDown(command->player);
}

Game_Command_Move_Up    Game_Command_Move_Up_Create(Game_Player *player) {
    Game_Command_Move_Up    command;
    
    command.base.execute = Game_Command_ExecuteMoveUp;
    command.player = player;
    return (command);
}

Game_Command_Move_Down    Game_Command_Move_Down_Create(Game_Player *player) {
    Game_Command_Move_Down    command;
    
    command.base.execute = Game_Command_ExecuteMoveDown;
    command.player = player;
    return (command);
}

Game_Command_Move_Right    Game_Command_Move_Right_Create(Game_Player *player) {
    Game_Command_Move_Right    command;
    
    command.base.execute = Game_Command_ExecuteMoveRight;
    command.player = player;
    return (command);
}

Game_Command_Move_Left    Game_Command_Move_Left_Create(Game_Player *player) {
    Game_Command_Move_Left    command;
    
    command.base.execute = Game_Command_ExecuteMoveLeft;
    command.player = player;
    return (command);
}

typedef struct Game_Command_Handler {
    Game_Command_Move_Right    move_right;
    Game_Command_Move_Left      move_left;
    Game_Command_Move_Up        move_up;
    Game_Command_Move_Down      move_down;
}   Game_Command_Handler;

void    Game_Command_Handler_Init(Game_Command_Handler *this, Game_Player *target) {
    this->move_right = Game_Command_Move_Right_Create(target);
    this->move_left = Game_Command_Move_Left_Create(target);
    this->move_up = Game_Command_Move_Up_Create(target);
    this->move_down = Game_Command_Move_Down_Create(target);
}

void    Game_Command_Handler_HandleInput(Game_Command_Handler *this, SDL_Event event, bool *running) {
    const bool  *key_state;
    
    if (event.type == SDL_EVENT_QUIT) {
        *running = false;
        return ;
    }
    if (event.type == SDL_EVENT_KEY_DOWN) {
        if (event.key.key == SDLK_ESCAPE) {
            *running = false;
            return ;
        }
    }
    key_state = SDL_GetKeyboardState(NULL);
    if (key_state[SDL_SCANCODE_UP] == true)
        this->move_up.base.execute((Game_Command *)&this->move_up);
    if (key_state[SDL_SCANCODE_DOWN] == true)
        this->move_down.base.execute((Game_Command*)&this->move_down);
    if (key_state[SDL_SCANCODE_LEFT] == true)
        this->move_left.base.execute((Game_Command *)&this->move_left);
    if (key_state[SDL_SCANCODE_RIGHT] == true)
        this->move_right.base.execute((Game_Command *)&this->move_right);
}

void    Game_HandleEvents(Game_Command_Handler *handler, SDL_Event event, bool *running) {
    while (SDL_PollEvent(&event) == true)
            Game_Command_Handler_HandleInput(handler, event, running);
}

/*--------------------------------------------------------------------------*/

void    Game_Loop(Game  *this) {
    Game_Command_Handler    handler;
    SDL_Event   event;
    bool    running;

    SDL_zero(event);
    running = true;
    Game_Command_Handler_Init(&handler, &this->player);
    Game_Timer_Start(&this->timer);
    while (running) {
        Game_HandleEvents(&handler, event, &running);
        Game_Update(this);
        Game_Timer_Sync(&this->timer);
    }
}

bool    Game_Texture_LoadFromFile(Game_Texture *this, Game_Window *window, const char *path) {
    SDL_Surface *loaded_surface;

    if ((loaded_surface = IMG_Load(path)) == NULL)
        return (Game_Error_Failure(&this->error, GAME_SDL_ERROR));
    if ((this->content = SDL_CreateTextureFromSurface(window->renderer, loaded_surface)) == NULL)
        return (Game_Error_CleanUp(&this->error, GAME_ERROR_CONTEXT(NULL, NULL, GAME_SDL_ERROR)));
    Size_Set(&this->size, loaded_surface->w, loaded_surface->h);
    SDL_DestroySurface(loaded_surface);
    return (true);
}

bool    Game_Texure_LoadFromFile(Game *this) {
   if (Game_Texture_LoadFromFile(&this->player.texture, &this->window, this->player.texture.path) == false)
        return (Game_Error_Failure(&this->error, GAME_SDL_ERROR));
    if (Game_Texture_LoadFromFile(&this->floor.texture, &this->window, this->floor.texture.path) == false) {
        Game_Texture_Destroy(&this->player.texture);
        return (Game_Error_Failure(&this->error, GAME_SDL_ERROR));
    }
    return (true);
}

bool    Game_Texture_IsLoaded(Game_Texture *this) {
    return (this->content != NULL);
}
int     main(void) {
    Game    game;

    Game_Init(&game);
    if (Game_Texure_LoadFromFile(&game) == false)
        return (Game_Error_Log(&game.error));
    Game_Loop(&game);
    Game_Quit(&game);
    return (0);
}