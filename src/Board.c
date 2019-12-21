
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

/**
 * Free allocated memory the board
 */
static void _destroy(Board *this)
{
    if (NULL != this)
    {
        for (size_t i = 0; i < this->num_x; i++)
        {
            free(this->rects[i]);
            free(this->aux[i]);
            free(this->past[i]);
        }
        free(this->aux);
        free(this->past);
        free(this->rects);
        free(this);
    }
}
/**
 * - Render the board
 * - This is a render function
 * prot: void(void*,SDL_Renderer*)
 */
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
                SDL_SetRenderDrawColor(renderer, gold.r, gold.g, gold.b, 0);
                SDL_RenderFillRect(renderer, &r);
            }

            else if (this->rects[i][k] == 2)
            {
                SDL_SetRenderDrawColor(renderer, red.r, red.g, red.b, 0);
                SDL_RenderFillRect(renderer, &r);
            }
            else if (this->rects[i][k] == 0)
            {
                SDL_SetRenderDrawColor(renderer, grey.r, grey.g, grey.b, 0);
                SDL_RenderDrawRect(renderer, &r);
            }
            r.x += r.w;
        }
        r.x = 0;
        r.y += r.h;
    }

    SDL_SetRenderDrawColor(renderer, space_blue.r, space_blue.g, space_blue.b, 0);
}

/**
 * clear it
 */
static int _clear(Board *this)
{
    for (size_t i = 0; i < this->num_x; i++)
    {
        for (size_t k = 0; k < this->num_x; k++)
        {
            this->rects[i][k] = 0;
            this->aux[i][k] = 0;
            this->past[i][k] = 0;
        }
    }
    return 0;
}

/**
 * The lions mane was dirty
 */
static int rand_range(int low, int up)
{
    return (rand() % (up - low + 1)) + low;
}

/**
 * Twas a female
 */
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
                this->past[i][k] = 1;
            }
            else
            {
                this->rects[i][k] = 0;
                this->past[i][k] = 0;
            }
            this->aux[i][k] = 0;
        }
    }
}
static void _add(Board *this)
{
    for (size_t i = 0; i < this->num_x; i++)
    {
        this->rects[i] = malloc(sizeof(int) * this->num_x);
        this->aux[i] = malloc(sizeof(int) * this->num_x);
        for (size_t k = 0; k < this->num_x; k++)
        {
            if (this->rects[i][k] == 0)
            {
                int num = rand_range(0, 40);
                if (num == 0)
                {
                    this->rects[i][k] = 1;
                }
            }
        }
    }
}

/**
 * Starting board 
 */
static void _set_rects(Board *this)
{
    int even = 0;
    this->rects = malloc(sizeof(int *) * this->num_x);
    this->aux = malloc(sizeof(int *) * this->num_x);
    this->past = malloc(sizeof(int *) * this->num_x);
    for (int i = 0; i < this->num_x; i++)
    {
        this->rects[i] = malloc(sizeof(int) * this->num_x);
        this->aux[i] = malloc(sizeof(int) * this->num_x);
        this->past[i] = malloc(sizeof(int) * this->num_x);
        for (int k = 0; k < this->num_x; k++)
        {
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

static int _get_index(int r, int m)
{
    return m / RECT_SIZE;
}
static int get_index(int r, int m)
{
    return m / RECT_SIZE;
}
static int out_of_bounds(int index, int num)
{
    if (index < 0 || index >= num)
        return 1;
    return 0;
}
static int check_neighbor_(int i, int k, int **rects, int num)
{
    if (out_of_bounds(i, num) || out_of_bounds(k, num))
        return 0;
    else if (rects[i][k] == 2)
        return 0;
    return (rects[i][k] == 1) ? 1 : 0;
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
static int moore(int **rects, int i, int k, int num)
{
    int neighbors;
    neighbors = 0;
    neighbors += check_neighbor_(i + 1, k, rects, num);
    neighbors += check_neighbor_(i, k + 1, rects, num);
    neighbors += check_neighbor_(i - 1, k, rects, num);
    neighbors += check_neighbor_(i, k - 1, rects, num);
    neighbors += check_neighbor_(i + 1, k + 1, rects, num);
    neighbors += check_neighbor_(i - 1, k + 1, rects, num);
    neighbors += check_neighbor_(i + 1, k - 1, rects, num);
    neighbors += check_neighbor_(i - 1, k - 1, rects, num);
    return neighbors;
}
static int extended_moore(int **rects, int i, int k, int num, int range)
{
    int neighbors = 0;
    for (int l = 1; l <= range; l++)
    {
        neighbors += check_neighbor_(i + l, k, rects, num);
        neighbors += check_neighbor_(i, k + l, rects, num);
        neighbors += check_neighbor_(i, k - l, rects, num);
        neighbors += check_neighbor_(i - l, k, rects, num);
        for (int j = 1; j <= range; j++)
        {
            neighbors += check_neighbor_(i + l, k + j, rects, num);
            neighbors += check_neighbor_(i - l, k + j, rects, num);
            neighbors += check_neighbor_(i + l, k - j, rects, num);
            neighbors += check_neighbor_(i - l, k - j, rects, num);
        }
    }
    return neighbors;
}
static void _bugs(Board *this, int rules[6])
{
    for (size_t i = 0; i < this->num_x; i++)
    {
        for (size_t k = 0; k < this->num_x; k++)
        {
            this->aux[i][k] = extended_moore(this->rects, i, k, this->num_x, rules[0]);
        }
    }
    for (size_t i = 0; i < this->num_x; i++)
    {
        for (size_t k = 0; k < this->num_x; k++)
        {
            if (this->rects[i][k] == 0) //born
            {
                if (this->aux[i][k] >= rules[2] && this->aux[i][k] <= rules[3]) //loved
                {
                    this->rects[i][k] = 1;
                }
            }
            else if (this->rects[i][k] == 1)
            {
                this->aux[i][k] += rules[1];
                if (this->aux[i][k] >= rules[4] && this->aux[i][k] <= rules[5]) //loved
                {
                    this->rects[i][k] = 1;
                }
                else if (this->aux[i][k] < rules[4] || this->aux[i][k] > rules[5]) //loved
                {
                    this->rects[i][k] = 0;
                }
            }
        }
    }
}

static void _brians(Board *this)
{
    for (size_t i = 0; i < this->num_x; i++)
    {
        for (size_t k = 0; k < this->num_x; k++)
        {
            this->aux[i][k] = moore(this->rects, i, k, this->num_x);
        }
    }
    for (size_t i = 0; i < this->num_x; i++)
    {
        for (size_t k = 0; k < this->num_x; k++)
        {
            if (this->rects[i][k] == 0 && (this->aux[i][k] == 2)) //born
            {
                this->rects[i][k] = 1;
                this->past[i][k]++;
            }
            else if (this->rects[i][k] == 1)
            {
                this->rects[i][k] = 2;
            }
            else if (this->rects[i][k] == 2)
            {
                this->rects[i][k] = 0;
            }
        }
    }
}
static void _gol(Board *this)
{
    int three = 3;
    int two = 2;
    for (size_t i = 0; i < this->num_x; i++)
    {
        for (size_t k = 0; k < this->num_x; k++)
        {
            this->aux[i][k] = moore(this->rects, i, k, this->num_x);
        }
    }
    for (size_t i = 0; i < this->num_x; i++)
    {
        for (size_t k = 0; k < this->num_x; k++)
        {
            if (this->rects[i][k] == 0 && (this->aux[i][k] == three)) //born
            {
                this->rects[i][k] = 1;
            }
            else if (this->rects[i][k] == 1)
            {
                if (this->aux[i][k] == three || this->aux[i][k] == two) //loved
                {
                    this->rects[i][k] = 1;
                }
                else if (this->aux[i][k] > three || this->aux[i][k] < two) //lonley/murdered
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
    this->brians = _brians;
    this->bugs = _bugs;
    this->clear = _clear;
    this->get_index = _get_index;
    this->rando = _rando;
    this->add = _add;

    this->num_x = BOARD_SIZE / RECT_SIZE;
    this->num_y = BOARD_SIZE / RECT_SIZE;
    this->set_rects(this);
    return this;
}