
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_thread.h>
#include "Delta.h"
#include "Header.h"

static long double time_per_tick;
static long double time_delay_per_second;
static double delta;

int frames_rendered;

static uint64_t time_now;
static uint64_t time_last;
static uint64_t nano_timer;
static uint64_t ticks_per_second;

void set_up_timer()
{
    delta = 0;
    time_per_tick = 1000000000 / (FPS / 2);
    time_last = SDL_GetPerformanceCounter();
    nano_timer = 0;
    frames_rendered = 0;
    ticks_per_second = 0;
    time_delay_per_second = 0;
}

void start_timer()
{
    time_now = SDL_GetPerformanceCounter();
    delta += (double)((time_now - time_last) / time_per_tick);
    nano_timer = time_now - time_last;
    ticks_per_second += nano_timer;
    time_last = time_now;
}

int check_delta()
{
    if (delta >= 1)
    {
        return 1;
    }
    return 0;
}

void update_delta()
{
    delta--;
    frames_rendered++;
}

void reset_timer()
{

    if (ticks_per_second >= SDL_GetPerformanceFrequency())
    {
        // printf("\nFrames Rendered Per Second: %d", FRAMES_RENDERED);
        // printf("\nTicks Per Second: %ld", ticks_per_second);
        frames_rendered = 0;
        ticks_per_second = 0;
    }
}

int delay()
{
    frames_rendered++;
    if (nano_timer < time_per_tick)
    {
        time_delay_per_second = ((time_per_tick - nano_timer) / (SDL_GetPerformanceFrequency() * 0.001));
        SDL_Delay(time_delay_per_second);
        // printf("\nTimeDelayed is: %Lf", time_delay_per_second);
        //   printf("\nTimeDelayed int is: %Lf", time_delay_per_second);
    }
    return frames_rendered;
}