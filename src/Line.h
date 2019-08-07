#ifndef LINE_H
#define LINE_H

typedef struct _Atlas Atlas;

struct Letter
{
    const char *letter;
    SDL_Rect rect;
    SDL_Texture *texture;
};
typedef struct _Line
{
    void (*destroy)(struct _Line *this);
    void (*set_letters)(struct _Line *this);

    struct SDL_Texture *(*get_texture)(struct _Line *this, int i);
    void (*render_letter)(SDL_Renderer *renderer, SDL_Texture *texture, SDL_Rect *rect);

    struct Letter **letters;
    Atlas *atlas;
    int num_let;
    const char *line;
    int *r_x, *r_y;
    int x, y;
    int inc;

} Line;

Line *CREATE_LINE(Atlas *atlas, const char *line, int x, int y, int inc);
struct Letter *NEW_LETTER(const char *letter, int x, int y);
void render_line0(void *obj, SDL_Renderer *renderer);
void destroy_line(void *obj);
void render_char(void *obj, SDL_Renderer *renderer);
#endif /* LINE_H */
