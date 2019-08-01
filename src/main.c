
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

int main(int argc, char **argv)
{

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
    {
        printf("error creating renderer: %s\n", SDL_GetError());
        return 1;
    }
    RECT_SIZE = BLOCK;
    key_state = (Uint8 *)SDL_GetKeyboardState(NULL);
    srand(time(0));
    struct SDL_Window *window = NULL;
    struct SDL_Renderer *renderer = NULL;
    window = make_window("Window");
    renderer = make_renderer(&window);
    // SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "best");
    // SDL_RenderSetLogicalSize(renderer, WINDOW_WIDTH, WINDOW_HEIGHT);

    set_up_timer();
    quit = 0;
    grid.r = 255;
    grid.g = 255;
    grid.b = 255;
    box.r = 12;
    box.g = 69;
    box.b = 194;

    Render_Q *render_q = render_q_create();
    Atlas *atlas = CREATE_ATLAS();

    Board *board = board_create(RECT_SIZE, RECT_SIZE);
    Mouse *mouse = mouse_create(RECT_SIZE);
    Interface *ui = interface_create();

    atlas->map(atlas, renderer);
    state = choose;
    choose_state = squares;
    union SDL_Event ev;
    while (!quit)
    {
        start_timer();
        render_q->enqueue(render_q, render_q->create_node(board, board->render));
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
            else if (key_state[RIGHT] && RECT_SIZE < 31)
                choose_state = bigger;
            else if (key_state[R])
                choose_state = rando;
            else
                choose_state = squares;
            break;

        default:
            break;
        }

        delay();
        reset_timer();
        SDL_Delay(10);
    }
    atlas->destroy(atlas);
    board->destroy(board);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
