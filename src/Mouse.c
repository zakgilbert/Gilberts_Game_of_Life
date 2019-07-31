
/************************
	 *  Mouse.c
	*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_thread.h>
#include "Header.h"
#include "Mouse.h"
static void _destroy(Mouse *this)
{
}

static void _render(void *obj, SDL_Renderer *renderer)
{
}
static int _get_x(Mouse *this)
{
    int result;
    result = this->x % RECT_SIZE;
    result = this->x - result;
    return result;
}
static int _get_y(Mouse *this)
{
    int result;
    result = this->y % RECT_SIZE;
    result = this->y - result;
    return result;
}
static int _on_click(Mouse *this, int state)
{
    if (this->lock && SDL_BUTTON(SDL_BUTTON_LEFT) && !state)
    {
        this->clk_x = this->get_x(this);
        this->clk_y = this->get_y(this);
        this->bool = 1;
    }
}

static int _get_state(Mouse *this)
{
    this->lock = SDL_GetMouseState(&this->x, &this->y);
}

Mouse *mouse_create(int rect_size)
{
    Mouse *this = malloc(sizeof(*this));
    this->destroy = _destroy;
    this->render = _render;
    this->get_x = _get_x;
    this->get_y = _get_y;
    this->on_click = _on_click;
    this->get_state = _get_state;

    this->rect_size = rect_size;
    this->bool = 0;
    return this;
}
