#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "chip8.h"

#include <SDL2/SDL.h>

typedef struct {
    SDL_Surface *surface;
    SDL_Renderer *renderer;
    SDL_Window *window;
    char quit_flag;
    int width;
    int height;
} window_t;

window_t *new_window(int, int);

void window_clear(window_t *);

void draw_chip8(window_t *, chip8_t *);

void free_window(window_t *);

#endif // GRAPHICS_H