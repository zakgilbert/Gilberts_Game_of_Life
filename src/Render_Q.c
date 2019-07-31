
/************************
	 *  Render_Q.c
	*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Render_Q.h"

static void _destroy(Render_Q *this)
{
    struct Node *temp;

    if (NULL == this)
        return;
    while (NULL != this->front)
    {
        temp = this->front;
        this->front = this->front->next;
        free(temp);
    }
    free(this);
}
static void _print(Render_Q *this)
{
    struct Node *current;
    current = this->front;

    while (NULL != current)
    {
        current = current->next;
    }
    printf("\n");
}
static struct Node *_create_node(void *obj, render_function target)
{
    struct Node *data;
    data = malloc(sizeof(struct Node));

    data->func = target;
    data->obj = obj;
    data->next = NULL;

    return data;
}
static void _enqueue(Render_Q *this, struct Node *node)
{
    if (NULL == this->tail)
    {
        this->front = node;
        this->tail = node;
        return;
    }
    this->tail->next = node;
    this->tail = node;
}

static Render_Q *_execute(Render_Q *this, struct SDL_Renderer *renderer)
{
    struct Node *temp;
    temp = NULL;

    while (NULL != this->front)
    {
        temp = this->pop(this);
        (*temp->func)(temp->obj, renderer);
        free(temp);
    }
    this->destroy(this);
    return render_q_create();
}

static struct Node *_pop(Render_Q *this)
{
    struct Node *temp = this->front;
    this->front = this->front->next;
    return temp;
}
Render_Q *render_q_create()
{
    Render_Q *this = malloc(sizeof(*this));

    this->destroy = _destroy;
    this->create_node = _create_node;
    this->enqueue = _enqueue;
    this->print = _print;
    this->pop = _pop;
    this->execute = _execute;

    this->front = NULL;
    this->tail = NULL;

    this->size = 0;
    return this;
}