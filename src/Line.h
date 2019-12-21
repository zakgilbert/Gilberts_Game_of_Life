#ifndef LINE_H
#define LINE_H

typedef struct _Atlas Atlas;

/**
 * Hastable node which holds values needed to render a letter.
 * The field "letter" is the key.
 */
struct Letter
{
    const char *letter;
    SDL_Rect rect;
    SDL_Texture *texture;
};

/**
 * Return a new struct Letter
 */
struct Letter *NEW_LETTER(const char *letter, int x, int y);

typedef struct _Line
{
    /* Free memory allocated for Line object */
    void (*destroy)(struct _Line *this);

    /* Render letter is called in render_line which is a function to be sent to the render_q*/
    void (*render_letter)(SDL_Renderer *renderer, SDL_Texture *texture, SDL_Rect *rect);

    /**
     * Sets the correct x and y coords to a letter.
     */
    struct SDL_Texture *(*get_texture)(struct _Line *this, int i);

    /**
     * Adds the letters to the line.
     */
    void (*set_letters)(struct _Line *this);

    struct Letter **letters;
    Atlas *atlas;
    int num_let;
    const char *line;
    int *r_x, *r_y;
    int x, y;
    int inc;

} Line;

Line *CREATE_LINE(Atlas *atlas, const char *line, int x, int y, int inc);
void render_line(void *obj, SDL_Renderer *renderer);
void destroy_line(void *obj);
void render_char(void *obj, SDL_Renderer *renderer);
#endif /* LINE_H */