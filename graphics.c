#include "graphics.h"

window_t *new_window(int width, int height) {
    SDL_Window *window = SDL_CreateWindow("Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL);
    if(window == NULL) {
        return NULL;
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(renderer == NULL) {
        SDL_DestroyWindow(window);
        return NULL;
    }
    SDL_Surface *surface = SDL_GetWindowSurface(window);
    if(surface == NULL) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        return NULL;
    }

    window_t *win = malloc(sizeof(window_t));
    win->window = window;
    win->renderer = renderer;
    win->surface = surface;
    win->quit_flag = FALSE;

    return win;
}

void window_clear(window_t *win) {
    SDL_RenderClear(win->renderer);
    SDL_RenderPresent(win->renderer);
}

void draw_chip8(window_t *win, chip8_t *chip8) {
    SDL_Event event;
    SDL_PollEvent(&event);
    if(event.type == SDL_QUIT) {
        win->quit_flag = TRUE;
        return;
    }

    if(chip8->draw_flag) {
        SDL_LockSurface(win->surface);
        Uint32 *screen = (Uint32 *)win->surface->pixels;
        memset(screen, 0, (win->surface->w)*(win->surface->h)*sizeof(Uint32));
        for(int i=0; i < win->surface->h; i++) {
            for(int j=0; j < win->surface->w; j++) {
                if(chip8->gfx[(j/10) + (i/10)*64] > 0) {
                    screen[j+i*(win->surface->w)] = 0xFFFFFFFF;
                }
            }
        }
        SDL_UnlockSurface(win->surface);

        SDL_Texture *txt = SDL_CreateTextureFromSurface(win->renderer, win->surface);

        SDL_RenderClear(win->renderer);
        SDL_RenderCopy(win->renderer, txt, NULL, NULL);
        SDL_RenderPresent(win->renderer);

        SDL_DestroyTexture(txt);
        chip8->draw_flag = FALSE;
    }
}

void free_window(window_t *win) {
    SDL_DestroyRenderer(win->renderer);
    SDL_DestroyWindow(win->window);

    free(win);
}