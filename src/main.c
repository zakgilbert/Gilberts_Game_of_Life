
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include "Header.h"
#include "Window_and_Renderer.h"
#include "Delta.h"
#include "Render_Q.h"
#include "Atlas.h"
#include "Line.h"
#include "Board.h"
#include "Mouse.h"
#include "Interface.h"

static void set_globals()
{
    RECT_SIZE = BLOCK;
    key_state = (Uint8 *)SDL_GetKeyboardState(NULL);
    quit = 0;
    box.r = 255;
    box.g = 255;
    box.b = 255;
    grid.r = 12;
    grid.g = 69;
    grid.b = 194;
    past.r = 194;
    past.g = 69;
    past.b = 12;

    show_grid = 1;

    state = choose;
    choose_state = squares;
}
static void set_full_screen(int full_screen, SDL_Window *window)
{
    if (full_screen)
        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
    else
        SDL_SetWindowFullscreen(window, 0);
}

int main(int argc, char **argv)
{

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
    {
        printf("error creating renderer: %s\n", SDL_GetError());
        return 1;
    }
    srand(time(0));
    struct SDL_Window *window = NULL;
    struct SDL_Renderer *renderer = NULL;
    window = make_window("Window");
    renderer = make_renderer(&window);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "best");
    SDL_RenderSetLogicalSize(renderer, WINDOW_WIDTH, WINDOW_HEIGHT);

    set_up_timer();
    set_globals();
    Render_Q *render_q = render_q_create();
    Atlas *atlas = CREATE_ATLAS(150);

    Board *board = board_create(RECT_SIZE, RECT_SIZE);
    Mouse *mouse = mouse_create(RECT_SIZE);
    Interface *ui = interface_create();

    atlas->map(atlas, renderer);
    union SDL_Event ev;
    int frames_renderered = 0;
    int full_screen = 0;
    while (!quit)
    {
        start_timer();
        render_q->enqueue(render_q, render_q->create_node(board, board->render));
        render_q = ui->words(ui, render_q, atlas);
        //render_q->enqueue(render_q, render_q->create_node(ui, ui->render));

        // render_q->enqueue(render_q, render_q->create_node(
        //                                 CREATE_LINE(atlas,
        //                                             player_2->get_score(player_2), WINDOW_WIDTH - 275, 50),
        //                                 render_line0));

        SDL_RenderClear(renderer);
        render_q = render_q->execute(render_q, renderer);
        SDL_RenderPresent(renderer);

        while (SDL_PollEvent(&ev) != 0)
        {
            switch (ev.type)
            {
            case SDL_QUIT:
                quit = 1;
                break;
            default:
                break;
            }
        }
        switch (state)
        {

        case gol:
            board->gol(board);
            /*
             */
            if (key_state[D])
            {
                state = choose;
            }
            else if (key_state[V])
            {
                board->add(board);
            }
            break;
        case choose:
            switch (choose_state)
            {
            case squares:
                mouse->get_state(mouse);
                mouse->on_click(mouse, state, board);
                board->select_filled(board, mouse);
                break;
            case smaller:
                RECT_SIZE--;
                board->destroy(board);
                Board *temp_1 = board_create(RECT_SIZE, RECT_SIZE);
                board = temp_1;
                break;
            case bigger:
                RECT_SIZE++;
                board->destroy(board);
                Board *temp = board_create(RECT_SIZE, RECT_SIZE);
                board = temp;
                break;
            case start:
                state = gol;
                break;
            case clear:
                board->clear(board);
                break;
            case rando:
                board->clear(board);
                board->rando(board);
                break;
            default:
                break;
            }
            if (key_state[S])
                choose_state = start;
            else if (key_state[C])
                choose_state = clear;
            else if (key_state[LEFT] && RECT_SIZE > 1)
                choose_state = smaller;
            else if (key_state[RIGHT] && RECT_SIZE < 100)
                choose_state = bigger;
            else if (key_state[R])
                choose_state = rando;
            else if (key_state[Q])
                quit = 1;
            else if (key_state[F])
            {
                full_screen = !full_screen;
                key_state[F] = 0;
                set_full_screen(full_screen, window);
            }
            else if (key_state[G])
            {
                key_state[G] = 0;
                show_grid = !show_grid;
            }
            else
                choose_state = squares;
            break;

        default:
            break;
        }

        frames_renderered = delay();
        reset_timer();
        SDL_Delay(1);
    }
    atlas->destroy(atlas);
    board->destroy(board);
    mouse->destroy(mouse);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
