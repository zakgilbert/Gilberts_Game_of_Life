#ifndef SPRITE_H
#define SPRITE_H
typedef struct SDL_Renderer SDL_Renderer;
typedef struct SDL_Rect SDL_Rect;
typedef struct SDL_Texture SDL_Texture;
typedef struct _Sprite
{
    void (*destroy)(struct _Sprite *this);
    void (*render)(void *obj, SDL_Renderer *renderer);
    void (*animate)(struct _Sprite *this, int frames_renderered);
    SDL_Rect ref, ori;
    SDL_Texture *tex;
} Sprite;
Sprite *sprite_create(int x, int y, int w, int h, SDL_Renderer *renderer, char *path);
#endif /* SPRITE_H */
