
#ifndef WINDOW_AND_RENDERER_H
#define WINDOW_AND_RENDERER_H

/* Create and return an SDL window object */
SDL_Window *make_window(char *);

/* Create and return and SDL renderer */
SDL_Renderer *make_renderer(struct SDL_Window **window);

/* Create a texture from an image file and a pointer to be referenced during rendering */
SDL_Texture *create_texture(struct SDL_Renderer *renderer, const char *path, struct SDL_Rect *rect);

#endif //WINDOW_AND_RENDERER_H
