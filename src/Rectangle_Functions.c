
/************************
	 *  Rectangle_Functions.c
	*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Rectangle_Functions.h"

int get_bottom(SDL_Rect rect)
{
    return (rect.y + rect.h);
}