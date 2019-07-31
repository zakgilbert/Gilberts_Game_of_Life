
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_thread.h>
#include "Header.h"
#include "Window_and_Renderer.h"
#include "Delta.h"
#include "Render_Q.h"
#include "Atlas.h"
#include "Line.h"
#include "Board.h"
#include "Mouse.h"

int main(int argc, char **argv)
{

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
    {
        printf("error creating renderer: %s\n", SDL_GetError());
        return 1;
    }

    int quit;
    int begin_game;
    key_state = (Uint8 *)SDL_GetKeyboardState(NULL);

    struct SDL_Window *window = NULL;
    struct SDL_Renderer *renderer = NULL;
    window = make_window("Window");
    renderer = make_renderer(&window);
    // SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "best");
    // SDL_RenderSetLogicalSize(renderer, WINDOW_WIDTH, WINDOW_HEIGHT);

    set_up_timer();
    quit = 0;

    Render_Q *render_q = render_q_create();
    Atlas *atlas = CREATE_ATLAS();

    Board *board = board_create(RECT_SIZE, RECT_SIZE);
    Mouse *mouse = mouse_create(RECT_SIZE);

    atlas->map(atlas, renderer);
    begin_game = 0;

    union SDL_Event ev;
    while (!quit)
    {
        start_timer();
        render_q->enqueue(render_q, render_q->create_node(board, board->render));
        //render_q->enqueue(render_q, render_q->create_node(board, board->render_filled));

        // render_q->enqueue(render_q, render_q->create_node(ball, ball->render));

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
            }
        }
        mouse->on_click(mouse, begin_game);
        mouse->get_state(mouse);
        board->select_filled(board, mouse);

        delay();
        reset_timer();
    }
    atlas->destroy(atlas);
    board->destroy(board);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
