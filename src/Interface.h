#ifndef INTERFACE_H
#define INTERFACE_H
typedef struct SDL_Renderer SDL_Renderer;
typedef struct SDL_Rect SDL_Rect;
typedef struct _Mouse Mouse;
typedef struct _Board Board;
typedef struct _Interface
{
    void (*destroy)(struct _Interface *this);
    void (*render)(void *obj, SDL_Renderer *renderer);
    void (*cursor)(struct _Interface *this, SDL_Renderer *renderer);
    void (*cursor_logic)(struct _Interface *this, Mouse *mouse);

    int color_x, color_y, color_w, color_h;

} Interface;
Interface *interface_create();
#endif /* INTERFACE_H */
