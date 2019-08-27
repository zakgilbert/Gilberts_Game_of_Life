
/************************
	 *  Render_Q.c
	*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Render_Q.h"

/* Frees all nodes in queue as well as the queue itself */
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

/* Creates and returns a heap allocated Node pointer */
static struct Node *_create_node(void *obj, render_function target)
{
    struct Node *data;
    data = malloc(sizeof(struct Node));

    data->func = target;
    data->obj = obj;
    data->next = NULL;

    return data;
}

/* Adds a new node to queue */
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

/* Removes front node from queue */
static struct Node *_pop(Render_Q *this)
{
    struct Node *temp = this->front;
    this->front = this->front->next;
    return temp;
}

/* Pops nodes one by one as the function pointers that the node contains are called */
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

Render_Q *render_q_create()
{
    Render_Q *this = malloc(sizeof(*this));

    this->destroy = _destroy;
    this->create_node = _create_node;
    this->enqueue = _enqueue;
    this->pop = _pop;
    this->execute = _execute;

    this->front = NULL;
    this->tail = NULL;

    this->size = 0;
    return this;
}