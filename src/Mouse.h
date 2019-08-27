#ifndef MOUSE_H
#define MOUSE_H

typedef struct SDL_Renderer SDL_Renderer;
typedef struct _Board Board;
typedef struct _Mouse
{
    /* Free memory allocated for Mouse object */
    void (*destroy)(struct _Mouse *this);

    /**
     * Get the mouse x cordinate after click such that the value returned is the x cordinate of the rectangle where the click occurred.
     */
    int (*get_x)(struct _Mouse *this);

    /**
     * Get the mouse y cordinate after click such that the value returned is the y cordinate of the rectangle where the click occurred.
     */
    int (*get_y)(struct _Mouse *this);

    /* If a valid click has occurred set the mouse coordinates and return true, else return false */
    int (*on_click)(struct _Mouse *this, int state, Board *board);

    /* Call the SDL_GetMouseState function */
    void (*get_state)(struct _Mouse *this);

    int x, y;
    int clk_x, clk_y;
    int lock;
    int bool;
    int rect_size;
} Mouse;
Mouse *mouse_create(int rect_size);
#endif /* MOUSE_H */
