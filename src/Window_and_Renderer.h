
#ifndef WINDOW_AND_RENDERER_H
#define WINDOW_AND_RENDERER_H


SDL_Window *make_window(char *);

SDL_Renderer *make_renderer(struct SDL_Window **window);
SDL_Texture *create_texture(struct SDL_Renderer *renderer, const char *path, struct SDL_Rect *rect);

#endif //WINDOW_AND_RENDERER_H
