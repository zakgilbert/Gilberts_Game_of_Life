
/************************
	 *  Interface.c
	*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_thread.h>
#include "Header.h"
#include "Board.h"
#include "Interface.h"
#include "Mouse.h"
static void _destroy(Interface *this)
{
    if (NULL == this)
    {
        free(this);
    }
}
static void _render(void *obj, SDL_Renderer *renderer)
{
    Interface *this = (Interface *)obj;
    this->cursor(this, renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
}
static void _cursor(Interface *this, SDL_Renderer *renderer)
{
    SDL_Rect cursor = {this->color_x, this->color_y, this->color_w, this->color_h};
    SDL_Rect dial = {this->color_x, this->color_y, this->color_w / 10, this->color_h + 20};

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
    SDL_RenderFillRect(renderer, &cursor);
    SDL_SetRenderDrawColor(renderer, 255, 50, 134, 0);
    SDL_RenderFillRect(renderer, &dial);
}
static void _cursor_logic(Interface *this, Mouse *mouse)
{
    int i = 0;
    if (state == choose && SDL_BUTTON(SDL_BUTTON_RMASK))
    {
        i++;
    }
}
Interface *interface_create()
{
    Interface *this = malloc(sizeof(*this));
    this->cursor = _cursor;
    this->destroy = _destroy;
    this->render = _render;
    this->cursor_logic = _cursor_logic;
    this->color_x = 400;
    this->color_y = 800;
    this->color_w = 100;
    this->color_h = 50;

    return this;
}