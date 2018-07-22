
#define WIDTH 640
#define HEIGHT 320


#define FPS 5
#define FPS_MICRO (FPS*1000/60)

#include <stdio.h>
#include <stdlib.h>

#ifdef WEB
#include <emscripten.h>
#endif // WEB

#include "all.h"

static unsigned char keymap[KEY_SIZE] = {
    SDLK_1,
    SDLK_2,
    SDLK_3,
    SDLK_w, // up
    SDLK_q,
    SDLK_4,
    SDLK_s, // down
    SDLK_a, // left
    SDLK_d, //right
    SDLK_v,
    SDLK_e,
    SDLK_f,
    SDLK_z,
    SDLK_x,
    SDLK_c,
    SDLK_r
};

void print_flag() {
    for(int i=0; i < 80; i++) {
        putc('%', stdout);
    }
    putc('\n', stdout);
}

typedef struct {
    window_t *win;
    chip8_t *chip8;
} context_t;

void loop_fn(void *arg) {
    context_t *ctx = (context_t *)arg;
    window_t *win = ctx->win;
    chip8_t *chip8 = ctx->chip8;

    if(!chip8_cycle(chip8)) {
        print_flag();
        printf("\nAn error occurred while processing input, is the data format correct\n");
        printf("\n\n");
        printf("The opcode read was not recognised to be part of the chip8 instruction set\n");
        printf("This could be a bug in this program's code as well\n\n");
        print_flag();
        win->quit_flag = TRUE;
        return;
    }

    printf("Called\n");

    draw_chip8(win, chip8);

}

int main(int argc, char *argv[]) {
    #ifdef WEB
    window_t *window = new_window(WIDTH, HEIGHT);
    if(window == NULL) {
        printf("Unable to create window\n");
        return EXIT_FAILURE;
    }
    
    
    /*
    if(argc != 2) {
        return EXIT_FAILURE;
    }
    */

    
    window_clear(window);
    
    chip8_t *chip8 = new_chip8();
    if(!load_data(chip8, "roms/blinky.c8")) {
        printf("Unable to load data\n");
        free_chip8(chip8);
        return EXIT_FAILURE;
    }

    context_t ctx;
    ctx.win = window;
    ctx.chip8 = chip8;
    emscripten_set_main_loop_arg(loop_fn, &ctx, -1, 1);

    free_chip8(chip8);
    free_window(window);

    return EXIT_SUCCESS;

    #else

    window_t *window = new_window(WIDTH, HEIGHT);
    if(window == NULL) {
        printf("Unable to create window\n");
        return EXIT_FAILURE;
    }
    
    if(argc != 2) {
        return EXIT_FAILURE;
    }
    
    window_clear(window);
    
    chip8_t *chip8 = new_chip8();
    if(!load_data(chip8, argv[1])) {
        printf("Unable to load data\n");
        free_chip8(chip8);
        return EXIT_FAILURE;
    }

    context_t ctx;
    ctx.win = window;
    ctx.chip8 = chip8;

    char pause = FALSE;

    for(;;) {

        SDL_Event event;
        memset(&event, 0, sizeof(event));
        SDL_PollEvent(&event);
        if(event.type == SDL_QUIT) {
            window->quit_flag = TRUE;
            break;
        }else if(event.type == SDL_KEYDOWN) {
            if(event.key.keysym.sym == SDLK_p) {
                pause = !pause;
            }else {
                for(short i=0; i < KEY_SIZE; i++) {
                    if(keymap[i] == event.key.keysym.sym) {
                        chip8->key[i] = TRUE;
                        break;
                    }
                }
            }
        }
        if(pause) {continue;}
        chip8_cycle(chip8);

        draw_chip8(window, chip8);
        
    }

    free_chip8(chip8);
    free_window(window);

    return EXIT_SUCCESS;

    #endif //WEB
}

