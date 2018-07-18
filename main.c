
#define WIDTH 640
#define HEIGHT 320

#include <stdio.h>
#include <stdlib.h>

#include "all.h"

void print_flag() {
    for(int i=0; i < 80; i++) {
        putc('%', stdout);
    }
    putc('\n', stdout);
}

int main(int argc, char *argv[]) {
    window_t *window = new_window(WIDTH, HEIGHT);
    if(window == NULL) {
        return EXIT_FAILURE;
    }

    if(argc != 2) {
        return EXIT_FAILURE;
    }

    
    window_clear(window);
    
    chip8_t *chip8 = new_chip8();
    if(!load_data(chip8, argv[1])) {

        free_chip8(chip8);
        return EXIT_FAILURE;
    }
    for(;;) {
        if(window->quit_flag) {
            break;
        }
        if(!chip8_cycle(chip8)) {
            print_flag();
            printf("\nAn error occurred while processing input, is the data format correct\n");
            printf("\n\n");
            printf("The opcode read was not recognised to be part of the chip8 instruction set\n");
            printf("This could be a bug in this program's code as well\n\n");
            print_flag();

            free_chip8(chip8);
            return EXIT_FAILURE;
        }
        //draw_chip8(window, chip8);
    }

    free_chip8(chip8);
    free_window(window);
    return EXIT_SUCCESS;
}

