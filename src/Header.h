#ifndef HEADER_H
#define HEADER_H

#define WINDOW_WIDTH 1080
#define WINDOW_HEIGHT 600
#define BOARD_SIZE 600

#define FPS 60

#define PRIME_1 163
#define PRIME_2 151
#define BLOCK 9
extern int RECT_SIZE;
int RECT_SIZE;

extern int LAST_X;
int LAST_X;
extern int LAST_Y;
int LAST_Y;

extern int quit;
int quit;
extern Uint8 *key_state;
Uint8 *key_state;

extern int show_grid;
int show_grid;

extern int X;
extern int Y;
int X;
int Y;

extern SDL_Color grid;
extern SDL_Color box;
extern SDL_Color past;
SDL_Color grid;
SDL_Color box;
SDL_Color past;

enum KEYS
{
    S = SDL_SCANCODE_S,
    D = SDL_SCANCODE_D,
    C = SDL_SCANCODE_C,
    LEFT = SDL_SCANCODE_LEFT,
    RIGHT = SDL_SCANCODE_RIGHT,
    R = SDL_SCANCODE_R,
    Q = SDL_SCANCODE_Q,
    G = SDL_SCANCODE_G,
    V = SDL_SCANCODE_V,
    F = SDL_SCANCODE_F
} key;

enum game_state
{
    choose,
    gol
} state;
enum UI_states
{
    squares,
    smaller,
    bigger,
    start,
    clear,
    rando
} choose_state;

#endif