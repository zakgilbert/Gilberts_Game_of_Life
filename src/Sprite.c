
/************************
	 *  Sprite.c
	*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Sprite.h"
#include "Window_and_Renderer.h"

static void _destroy(Sprite *this)
{
    if (NULL != this)
    {
        SDL_DestroyTexture(this->tex);
        free(this);
    }
}

static void _render(void *obj, SDL_Renderer *renderer)
{
    Sprite *this = (Sprite *)obj;
    SDL_RenderCopy(renderer, this->tex, &this->ref, &this->ori);
}
static void _animate(Sprite *this, int frames_renderered)
{
    if (frames_renderered % 8 == 0)
    {
        if (this->ref.x >= this->ref.w)
        {
            if (this->ref.y >= this->ref.h)
            {
                this->ref.y = 0;
            }
            else
            {
                this->ref.y += this->ref.h;
            }

            this->ref.x = 0;
        }
        else
        {
            this->ref.x += this->ref.w;
        }
    }
}

Sprite *sprite_create(int x, int y, int w, int h, SDL_Renderer *renderer, char *path)
{
    Sprite *this = malloc(sizeof(*this));
    this->destroy = _destroy;
    this->render = _render;
    this->animate = _animate;
    this->tex = create_texture(renderer, path, &this->ori);
    this->ori.x = x;
    this->ori.y = y;
    this->ref.w = w;
    this->ref.h = h;
    return this;
}
