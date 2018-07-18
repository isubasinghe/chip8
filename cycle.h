#include <stdio.h>
#include <stdlib.h>

#ifndef BOOL_OPS
#define FALSE 0
#define TRUE 1
#define BOOL_OPS
#endif // BOOL_OPS

#include "chip8.h"

char chip8_cycle(chip8_t *);