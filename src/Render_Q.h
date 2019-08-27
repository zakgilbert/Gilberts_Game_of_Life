#ifndef RENDER_Q_H
#define RENDER_Q_H


/**
 * Template for the desired function pointer to be stored in queue
 */
typedef void render_function(void *obj, struct SDL_Renderer *renderer);

/**
 * Node type that stores the necessary elements to execute an added function from the queue
 */
struct Node
{
    render_function(*func);
    void *obj;
    struct Node *next;
};

typedef struct _Render_Q
{
    /* Frees all nodes in queue as well as the queue itself */
    void (*destroy)(struct _Render_Q *this);

    /* Creates and returns a heap allocated Node pointer */
    struct Node *(*create_node)(void *obj, render_function target);

    /* Adds a new node to queue */
    void (*enqueue)(struct _Render_Q *this, struct Node *node);

    /* Removes front node from queue */
    struct Node *(*pop)(struct _Render_Q *this);

    /* Pops nodes one by one as the function pointers that the node contains are called */
    struct _Render_Q *(*execute)(struct _Render_Q *this, struct SDL_Renderer *renderer);


    struct Node *front;
    struct Node *tail;

    int size;

} Render_Q;

Render_Q *render_q_create();

#endif /* RENDER_Q_H */
