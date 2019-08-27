
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
#include "Render_Q.h"
#include "Atlas.h"
#include "Line.h"

#define controlls "Controlls, they will help you"
#define welcome "Welcome to Life"
#define running "Game is Running"
#define paused "Game is Paused"

const char *rules[13] = {
    "In conways universe survival",
    "is defined as follows.",
    "    1. A living cell with two",
    "       or three neighbors",
    "       continues its life.",
    "    2. A living cell with less",
    "       than two neighbors dies",
    "       of lonleness.",
    "    3. A living cell with more",
    "       than three neighbors",
    "       dies of overcrowdedness.",
    "    4. A dead cell with three",
    "       neighbors is given life."};
const char *
    con_trolls_letters[8] = {"R", "Q", "C", "S", "D", "F", "Left", "Right"};
const char *con_trolls_words[8] = {
    "Random",
    "Quit",
    "Clear",
    "Start",
    "Pause",
    "Full Screen",
    "Increase Size",
    "Decrease Size"};

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
static Render_Q *_words(Interface *this, Render_Q *q, Atlas *atlas)
{
    int x = BOARD_SIZE + 150;
    int y = 70;
    int x_let = x + 200;
    q->enqueue(q, q->create_node(CREATE_LINE(atlas, welcome, x + 60, 40, 2), render_line0));
    for (int i = 0; i < 8; i++)
    {
        q->enqueue(q, q->create_node(CREATE_LINE(atlas, con_trolls_words[i], x, y, 2), render_line0));
        q->enqueue(q, q->create_node(CREATE_LINE(atlas, con_trolls_letters[i], x_let, y, 2), render_line0));
        y += 20;
    }
    if (state == choose)
        q->enqueue(q, q->create_node(CREATE_LINE(atlas, paused, x + 60, y + 50, 2), render_line0));
    else
        q->enqueue(q, q->create_node(CREATE_LINE(atlas, running, x + 60, y + 50, 2), render_line0));

    y += 90;
    for (int k = 0; k < 13; k++)
    {
        q->enqueue(q, q->create_node(CREATE_LINE(atlas, rules[k], x + 10, y, 2), render_line0));
        y += 20;
    }
    return q;
}

Interface *interface_create()
{
    Interface *this = malloc(sizeof(*this));
    this->cursor = _cursor;
    this->destroy = _destroy;
    this->render = _render;
    this->words = _words;
    this->cursor_logic = _cursor_logic;
    this->color_x = 400;
    this->color_y = 800;
    this->color_w = 100;
    this->color_h = 50;

    return this;
}