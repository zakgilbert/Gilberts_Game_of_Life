#ifndef BOARD_H
#define BOARD_H

typedef struct SDL_Rect SDL_Rect;
typedef struct SDL_Renderer SDL_Renderer;
typedef struct _Mouse Mouse;
typedef struct _Render_Q Render_Q;

typedef struct _Board
{
    void (*destroy)(struct _Board *this);
    void (*render)(void *obj, SDL_Renderer *renderer);

    void (*set_rects)(struct _Board *this);
    void (*select_filled)(struct _Board *this, Mouse *mouse);
    void (*gol)(struct _Board *this);
    int (*clear)(struct _Board *this);
    int (*get_index)(int r, int m);
    void (*rando)(struct _Board *this);
    void (*add)(struct _Board *this);

    int num_x, num_y, num_rects;

    int **rects;
    int **aux;
    int **past;
} Board;

Board *board_create(int w, int h);

#endif /* BOARD_H */
