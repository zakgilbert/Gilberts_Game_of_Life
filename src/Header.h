#ifndef HEADER_H
#define HEADER_H

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080
#define BOARD_SIZE 1000

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

extern int show_box;
int show_box;

extern int X;
extern int Y;
int X;
int Y;

extern SDL_Color blue;
extern SDL_Color white;
extern SDL_Color past;
extern SDL_Color red;
extern SDL_Color grey;
extern SDL_Color cyan;

SDL_Color blue;
SDL_Color white;
SDL_Color past;
SDL_Color red;
SDL_Color grey;
SDL_Color cyan;

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
    F = SDL_SCANCODE_F,
    B = SDL_SCANCODE_B
} key;

enum game_state
{
    choose,
    gol,
    brian
} state;

enum current_game
{
    con,
    bri
} CURRENT_GAME;
enum UI_states
{
    squares,
    smaller,
    bigger,
    start,
    clear,
    rando,
    change
} choose_state;

#endif