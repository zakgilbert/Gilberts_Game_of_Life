#ifndef ATLAS_H
#define ATLAS_H

struct Alpha_Node
{
    SDL_Texture *texture;
    SDL_Rect rect;
    char *key;
};

typedef struct _Atlas
{
    void (*destroy)(struct _Atlas *this);
    void (*insert)(struct _Atlas *this, const char *key, SDL_Renderer *renderer, const char *path);
    struct Alpha_Node *(*search)(struct _Atlas *this, const char *key);
    void (*map)(struct _Atlas *this, SDL_Renderer *renderer);
    void (*render)(struct Alpha_Node *node, SDL_Renderer *renderer);
    struct Alpha_Node **letters;
    int size;
    int count;

} Atlas;
Atlas *CREATE_ATLAS();
#endif /* ATLAS_H */
