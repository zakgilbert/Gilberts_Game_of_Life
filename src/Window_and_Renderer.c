#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdint.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include "Window_and_Renderer.h"
#include "Header.h"

SDL_Window *make_window(char *name)
{
    struct SDL_Window *window = NULL;
    window = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH,
                              WINDOW_HEIGHT, 0);
    if (!window)
    {
        printf("\nERROR: SDL window failed--\n%s\n", SDL_GetError());
    }
    return window;
}

SDL_Renderer *make_renderer(SDL_Window **window)
{
    Uint32 render_flags;
    SDL_Renderer *renderer = NULL;
    render_flags = SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE;

    renderer = SDL_CreateRenderer(*window, -1, render_flags);

    if (!renderer)
    {
        printf("error creating renderer: %s\n", SDL_GetError());
        SDL_Quit();
    }
    return renderer;
}

struct SDL_Texture *create_texture(struct SDL_Renderer *renderer, const char *path, struct SDL_Rect *rect)
{
    struct SDL_Texture *texture = NULL;
    struct SDL_Surface *surface = NULL;

    surface = IMG_Load(path);

    if (!surface)
    {
        printf("error creating surface: %s\n", SDL_GetError());
        SDL_Quit();
    }

    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!texture)
    {
        printf("error creating Texture: %s\n", SDL_GetError());
        SDL_Quit();
    }
    SDL_QueryTexture(texture, NULL, NULL, &rect->w, &rect->h);
    return texture;
}