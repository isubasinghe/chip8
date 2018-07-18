#include "file.h"

char load_data(chip8_t *chip8, char *fname) {
    FILE *fp = fopen(fname, "rb");
    if(fp == NULL) {
        #ifdef DEBUG
        printf("Could not open file name %s\n", fname);
        #endif // DEBUG
        return FALSE;
    }

    unsigned char buffer[MEM_SIZE-PRGRMC_START];
    unsigned short written = 0;

    while(1) {
        short c = getc(fp);
        if(c == EOF) {
            break;
        }
        if(written < (MEM_SIZE - PRGRMC_START)) {
            buffer[written] = c;
            written++;
        }
    }

    fclose(fp);
    if(written < 1) {
        #ifdef DEBUG
        printf("Program is larger than 1\n");
        #endif // DEBUG
        return FALSE;
    }
    
    for(unsigned short i=0; i < written; i++) {
        chip8->memory[i+PRGRMC_START] = buffer[i];
        #ifdef DEBUG
        printf("0x%04X\n", buffer[i]);
        #endif // DEBUG
    }

    #ifdef DEBUG
    printf("Loaded file %s\n", fname);
    #endif // DEBUG
    return TRUE;
}