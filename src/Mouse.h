#ifndef MOUSE_H
#define MOUSE_H

typedef struct SDL_Renderer SDL_Renderer;
typedef struct _Board Board;
typedef struct _Mouse
{
    void (*destroy)(struct _Mouse *this);
    void (*render)(void *obj, SDL_Renderer *renderer);
    int (*get_x)(struct _Mouse *this);
    int (*get_y)(struct _Mouse *this);
    int (*on_click)(struct _Mouse *this, int state, Board *board);
    void (*get_state)(struct _Mouse *this);

    int x, y;
    int clk_x, clk_y;
    int lock;
    int bool;
    int rect_size;
} Mouse;
Mouse *mouse_create(int rect_size);
#endif /* MOUSE_H */
