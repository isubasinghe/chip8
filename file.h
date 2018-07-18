#include <stdio.h>
#include <stdlib.h>

#ifndef BOOL_OPS
#define FALSE 0
#define TRUE 1
#define BOOL_OPS
#endif // BOOL_OPS

#include "chip8.h"

char load_data(chip8_t *, char *);