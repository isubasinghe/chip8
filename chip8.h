#ifndef CHIP8_H
#define CHIP8_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifndef BOOL_OPS
#define FALSE 0
#define TRUE 1
#define BOOL_OPS
#endif // BOOL_OPS

#define MEM_SIZE 4096
#define NO_REGISTERS 16

#define GFX_SIZE 2048

#define STACK_SIZE 16

#define KEY_SIZE 16

#define PRGRMC_START 0x200

typedef struct {
    unsigned short opcode;
    unsigned char memory[MEM_SIZE];
    unsigned char V[NO_REGISTERS];

    unsigned short I;
    unsigned short  pc;

    unsigned char gfx[GFX_SIZE];

    unsigned char delay_timer;
    unsigned char sound_timer;

    unsigned short stack[STACK_SIZE];
    unsigned short sp;

    unsigned char key[KEY_SIZE];

    unsigned char draw_flag;
} chip8_t;


chip8_t *new_chip8();

void load_fontset(chip8_t *);

void free_chip8(chip8_t *);

#endif // CHIP8_H