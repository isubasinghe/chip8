#include "graphics.h"

window_t *new_window(int width, int height) {
    SDL_Window *window = SDL_CreateWindow("Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL);
    if(window == NULL) {
        printf("Unable to initialise window\n");
        return NULL;
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(renderer == NULL) {
        printf("Unable to initialise renderer\n");
        SDL_DestroyWindow(window);
        return NULL;
    }

    window_t *win = malloc(sizeof(window_t));
    win->window = window;
    win->renderer = renderer;
    win->quit_flag = FALSE;
    win->width = width;
    win->height = height;

    return win;
}

void window_clear(window_t *win) {
    SDL_RenderClear(win->renderer);
    SDL_RenderPresent(win->renderer);
}

void draw_chip8(window_t *win, chip8_t *chip8) {
    

    

    if(chip8->draw_flag) {
        SDL_SetRenderDrawColor(win->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(win->renderer);

        SDL_SetRenderDrawColor(win->renderer, 255, 0, 255, SDL_ALPHA_OPAQUE);
        for(int i=0; i < win->height; i++) {
            for(int j=0; j < win->width; j++) {
                if(chip8->gfx[(j/10) + (i/10)*64] > 0) {
                    SDL_RenderDrawPoint(win->renderer, j, i);
                }
            }
        }

        SDL_RenderPresent(win->renderer);
        
        chip8->draw_flag = FALSE;
    }
}

void free_window(window_t *win) {
    SDL_DestroyRenderer(win->renderer);
    SDL_DestroyWindow(win->window);

    free(win);
}