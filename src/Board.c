
/************************
	 *  board.c
	*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include "Header.h"
#include "Board.h"
#include "Mouse.h"

static void _destroy(Board *this)
{
    if (NULL != this)
    {
        for (size_t i = 0; i < this->num_x; i++)
        {
            free(this->rects[i]);
        }
        free(this);
    }
}
static void _render(void *obj, struct SDL_Renderer *renderer)
{
    Board *this = (Board *)obj;
    struct SDL_Rect r = {0, 0, RECT_SIZE, RECT_SIZE};

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);

    for (size_t i = 0; i < this->num_x; i++)
    {
        for (size_t k = 0; k < this->num_x; k++)
        {
            if (this->rects[i][k] == 1)
            {
                SDL_RenderFillRect(renderer, &r);
                this->aux[i][k] = 1;
            }
            else
            {
                SDL_RenderDrawRect(renderer, &r);
                this->aux[i][k] = 0;
            }
            r.x += r.w;
        }
        r.x = 0;
        r.y += r.h;
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
}
static void _set_rects(Board *this)
{
    this->rects = malloc(sizeof(int *) * this->num_x);
    this->aux = malloc(sizeof(int *) * this->num_x);
    for (size_t i = 0; i < this->num_x; i++)
    {
        this->rects[i] = malloc(sizeof(int) * this->num_x);
        this->aux[i] = malloc(sizeof(int) * this->num_x);
        for (size_t k = 0; k < this->num_x; k++)
        {
            this->rects[i][k] = 0;
            this->aux[i][k] = 0;
        }
    }
}
static int get_index(int r, int m)
{
    printf("m: %d\n", m);
    if (m == 0)
        return 0;
    float w = (float)WINDOW_WIDTH / (float)m;
    float ret = (float)r / w;
    printf("w: %f   ret: %f\n", w, ret);
    return (int)ret;
}
static void _select_filled(Board *this, Mouse *mouse)
{
    if (mouse->bool)
    {
        int x = get_index(this->num_x, mouse->clk_x);
        int y = get_index(this->num_x, mouse->clk_y);
        if (x >= this->num_x || y >= this->num_x)
            return;
        printf("x: %d   y: %d\n", x, y);
        this->rects[y][x] = 1;
    }
}
Board *board_create(int w, int h)
{
    Board *this = malloc(sizeof(*this));
    this->destroy = _destroy;
    this->render = _render;
    this->set_rects = _set_rects;
    this->select_filled = _select_filled;

    this->num_x = WINDOW_WIDTH / RECT_SIZE;
    this->num_y = WINDOW_HEIGHT / RECT_SIZE;
    this->num_rects = this->num_x * this->num_y;
    this->set_rects(this);
    return this;
}