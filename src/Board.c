
/************************
	 *  board.c
	*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "time.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_thread.h>
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
            free(this->aux[i]);
        }
        free(this);
    }
}
static void _render(void *obj, struct SDL_Renderer *renderer)
{
    Board *this = (Board *)obj;
    struct SDL_Rect r = {0, 0, RECT_SIZE, RECT_SIZE};

    for (int i = 0; i < this->num_x; i++)
    {
        for (int k = 0; k < this->num_x; k++)
        {
            if (this->rects[i][k] == 1)
            {
                SDL_SetRenderDrawColor(renderer, grid.r, grid.g, grid.b, 0);
                SDL_RenderFillRect(renderer, &r);
            }
            else
            {
                SDL_SetRenderDrawColor(renderer, box.r, box.g, box.b, 0);
                SDL_RenderDrawRect(renderer, &r);
            }
            r.x += r.w;
        }
        r.x = 0;
        r.y += r.h;
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
}
static int _clear(Board *this)
{
    for (size_t i = 0; i < this->num_x; i++)
    {
        for (size_t k = 0; k < this->num_x; k++)
        {
            this->rects[i][k] = 0;
            this->aux[i][k] = 0;
        }
    }
    return 0;
}
static int rand_range(int low, int up)
{
    return (rand() % (up - low + 1)) + low;
}
static void _rando(Board *this)
{
    for (size_t i = 0; i < this->num_x; i++)
    {
        this->rects[i] = malloc(sizeof(int) * this->num_x);
        this->aux[i] = malloc(sizeof(int) * this->num_x);
        for (size_t k = 0; k < this->num_x; k++)
        {
            int num = rand_range(0, 1);
            if (num == 0)
            {
                this->rects[i][k] = 1;
                this->aux[i][k] = 1;
            }
            else
            {
                this->rects[i][k] = 0;
                this->aux[i][k] = 0;
            }
        }
    }
}
static void _set_rects(Board *this)
{
    int even = 0;
    int pat = 2;
    this->rects = malloc(sizeof(int *) * this->num_x);
    this->aux = malloc(sizeof(int *) * this->num_x);
    for (int i = 0; i < this->num_x; i++)
    {
        this->rects[i] = malloc(sizeof(int) * this->num_x);
        this->aux[i] = malloc(sizeof(int) * this->num_x);
        for (int k = 0; k < this->num_x; k++)
        {
            /*
            if (even)
            {
                if (k % pat == 0)
                {

                    this->rects[i][k] = 1;
                    this->aux[i][k] = 1;
                }
                else
                {
                    this->rects[i][k] = 0;
                    this->aux[i][k] = 0;
                }
            }
            else if (!even)
            {
                if (k % pat == 0)
                {

                    this->rects[i][k] = 0;
                    this->aux[i][k] = 0;
                }
                else
                {
                    this->rects[i][k] = 1;
                    this->aux[i][k] = 1;
                }
            }
         */
            if (i == k || (i + k) == this->num_x - 1)
            {
                this->rects[i][k] = 1;
                this->aux[i][k] = 1;
            }
            else
            {
                this->rects[i][k] = 0;
                this->aux[i][k] = 0;
            }
        }
        even = !even;
    }
}
static int get_cord(int index, int num_sqrs)
{
    return index * RECT_SIZE;
}
static int _get_index(int r, int m)
{
    return m / RECT_SIZE;
}
static int get_index(int r, int m)
{
    return m / RECT_SIZE;
}
static int get_index_up(int num_squares, int y, int height)
{
    if ((y - height) < 0)
        return -1;
    int y_up = y - height;
    return get_index(num_squares, y_up);
}

static int get_index_down(int num_squares, int y, int height)
{
    if ((y + height) >= BOARD_SIZE)
        return -1;
    int y_down = y + height;
    return get_index(num_squares, y_down);
}

static int get_index_right(int num_squares, int x, int width)
{
    if ((x + width) >= BOARD_SIZE)
        return -1;
    int x_down = x + width;
    return get_index(num_squares, x_down);
}

static int get_index_left(int num_squares, int x, int width)
{
    if ((x - width) < 0)
        return -1;
    int x_down = x - width;
    return get_index(num_squares, x_down);
}
static int check_neighbor(int index_1, int **rects, int index_2, int num_squares)
{
    if (index_1 == -1 || index_2 == -1 || index_1 >= num_squares || index_2 >= num_squares)
        return 0;
    else if (rects[index_1][index_2] == 1)
        return 1;
    return 0;
}
static int life(int **rects, int x, int y, int num_squares)
{
    int neighbors;
    neighbors = 0;

    /* up */
    neighbors += check_neighbor(get_index_up(num_squares, y, RECT_SIZE), rects, get_index(num_squares, x), num_squares);

    /* down */
    neighbors += check_neighbor(get_index_down(num_squares, y, RECT_SIZE), rects, get_index(num_squares, x), num_squares);

    /* left */
    neighbors += check_neighbor(get_index(num_squares, y), rects, get_index_left(num_squares, x, RECT_SIZE), num_squares);

    /* right */
    neighbors += check_neighbor(get_index(num_squares, y), rects, get_index_right(num_squares, x, RECT_SIZE), num_squares);

    /* up right */
    neighbors += check_neighbor(get_index_up(num_squares, y, RECT_SIZE), rects, get_index_right(num_squares, x, RECT_SIZE), num_squares);

    /* down right */
    neighbors += check_neighbor(get_index_down(num_squares, y, RECT_SIZE), rects, get_index_right(num_squares, x, RECT_SIZE), num_squares);

    /* up left */
    neighbors += check_neighbor(get_index_up(num_squares, y, RECT_SIZE), rects, get_index_left(num_squares, x, RECT_SIZE), num_squares);

    /* down left */
    neighbors += check_neighbor(get_index_down(num_squares, y, RECT_SIZE), rects, get_index_left(num_squares, x, RECT_SIZE), num_squares);

    return neighbors;
}
static void _select_filled(Board *this, Mouse *mouse)
{
    int x = get_index(this->num_x, mouse->clk_x);
    int y = get_index(this->num_x, mouse->clk_y);
    if (mouse->clk_x < 0)
        return;
    if (mouse->bool)
    {
        if (x >= this->num_x || y >= this->num_x)
            return;
        this->rects[y][x] = 1;
        LAST_X = x;
        LAST_Y = y;
        mouse->clk_x = -1;
        mouse->clk_y = -1;
    }
}
static void _gol(Board *this)
{
    for (size_t i = 0; i < this->num_x; i++)
    {
        for (size_t k = 0; k < this->num_x; k++)
        {
            this->aux[i][k] = life(this->rects, get_cord(k, this->num_x), get_cord(i, this->num_x), this->num_x);
        }
    }
    for (size_t i = 0; i < this->num_x; i++)
    {
        for (size_t k = 0; k < this->num_x; k++)
        {
            if (this->rects[i][k] == 0 && this->aux[i][k] == 3)
                this->rects[i][k] = 1;
            else if (this->rects[i][k] == 1)
            {
                if (this->aux[i][k] == 3 || this->aux[i][k] == 2)
                    this->rects[i][k] = 1;
                else if (this->aux[i][k] > 3 || this->aux[i][k] < 2)
                    this->rects[i][k] = 0;
            }
        }
    }
}
Board *board_create(int w, int h)
{
    Board *this = malloc(sizeof(*this));
    this->destroy = _destroy;
    this->render = _render;
    this->set_rects = _set_rects;
    this->select_filled = _select_filled;
    this->gol = _gol;
    this->clear = _clear;
    this->get_index = _get_index;
    this->rando = _rando;

    this->num_x = BOARD_SIZE / RECT_SIZE;
    this->num_y = BOARD_SIZE / RECT_SIZE;
    this->set_rects(this);
    return this;
}