#include "cycle.h"

char chip8_cycle(chip8_t *chip8) {
    chip8->opcode = chip8->memory[chip8->pc] << 8 | chip8->memory[chip8->pc+1];

    #ifdef DEBUG
    printf("Executing opcode=0x%04X pc=%d mem[pc]=%d mem[pc+1]=%d\n", chip8->opcode, chip8->pc, chip8->memory[chip8->pc], chip8->memory[chip8->pc+1]);
    #endif //

    switch(chip8->opcode & 0xF000){    
        case 0x0000:
            switch(chip8->opcode & 0x000F){
                case 0x0000: // 00E0: Clears the screen  
                    memset(chip8->gfx, 0, sizeof(chip8->gfx));
                    chip8->pc += 2;
                    chip8->draw_flag = TRUE;
                break;
        
                case 0x000E: // 00EE: Returns from subroutine      
                    chip8->pc = chip8->stack[(--chip8->sp)&0xF] + 2;
                break;  
                default: printf("Wrong opcode: %X\n", chip8->opcode); getchar();
                    
            }
        break;
        
        case 0x1000: // 1NNN: Jumps to address NNN
            chip8->pc = chip8->opcode & 0x0FFF;
        break;  
            
        case 0x2000: // 2NNN: Calls subroutine at NNN
            chip8->stack[(chip8->sp++)&0xF] = chip8->pc;
            chip8->pc = chip8->opcode & 0x0FFF;
        break;
    
        case 0x3000: // 3XNN: Skips the next instruction if VX equals NN
            if(chip8->V[(chip8->opcode & 0x0F00) >> 8] == (chip8->opcode & 0x00FF))
                chip8->pc += 4;
            else
                chip8->pc += 2;
        break;
    
        case 0x4000: // 4XNN: Skips the next instruction if VX doesn't equal NN
            if(chip8->V[(chip8->opcode & 0x0F00) >> 8] != (chip8->opcode & 0x00FF))
                chip8->pc += 4;
            else
                chip8->pc += 2;
        break;
    
        case 0x5000: // 5XY0: Skips the next instruction if VX equals VY
            if(chip8->V[(chip8->opcode & 0x0F00) >> 8] == chip8->V[(chip8->opcode & 0x00F0) >> 4])
                chip8->pc += 4;
            else
                chip8->pc += 2;
        break;
    
        case 0x6000: // 6XNN: Sets VX to NN
            chip8->V[(chip8->opcode & 0x0F00) >> 8] = (chip8->opcode & 0x00FF);
            chip8->pc += 2;
        break;
    
        case 0x7000: // 7XNN: Adds NN to VX
            chip8->V[(chip8->opcode & 0x0F00) >> 8] += (chip8->opcode & 0x00FF);
            chip8->pc += 2;
        break;
            
        case 0x8000:
            switch(chip8->opcode & 0x000F){
                case 0x0000: // 8XY0: Sets VX to the value of VY
                    chip8->V[(chip8->opcode & 0x0F00) >> 8] = chip8->V[(chip8->opcode & 0x00F0) >> 4];
                    chip8->pc += 2;
                break;
    
                case 0x0001: // 8XY1: Sets VX to VX or VY
                    chip8->V[(chip8->opcode & 0x0F00) >> 8] = chip8->V[(chip8->opcode & 0x0F00) >> 8] | chip8->V[(chip8->opcode & 0x00F0) >> 4];
                    chip8->pc += 2;
                break;
    
                case 0x0002: // 8XY2: Sets VX to VX and VY
                    chip8->V[(chip8->opcode & 0x0F00) >> 8] = chip8->V[(chip8->opcode & 0x0F00) >> 8] & chip8->V[(chip8->opcode & 0x00F0) >> 4];
                    chip8->pc += 2;
                break;
    
                case 0x0003: // 8XY3: Sets VX to VX xor VY
                    chip8->V[(chip8->opcode & 0x0F00) >> 8] = chip8->V[(chip8->opcode & 0x0F00) >> 8] ^ chip8->V[(chip8->opcode & 0x00F0) >> 4];
                    chip8->pc += 2;
                break;
    
                case 0x0004: // 8XY4: Adds VY to VX. VF is set to 1 when there's a carry, and to 0 when there isn't
                    if(((int)chip8->V[(chip8->opcode & 0x0F00) >> 8 ] + (int)chip8->V[(chip8->opcode & 0x00F0) >> 4]) < 256)
                        chip8->V[0xF] &= 0;
                    else
                        chip8->V[0xF] = 1;

                    chip8->V[(chip8->opcode & 0x0F00) >> 8] += chip8->V[(chip8->opcode & 0x00F0) >> 4];
                    chip8->pc += 2;
                break;
    
                case 0x0005: // 8XY5: VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there isn't
                    if(((int)chip8->V[(chip8->opcode & 0x0F00) >> 8 ] - (int)chip8->V[(chip8->opcode & 0x00F0) >> 4]) >= 0)
                        chip8->V[0xF] = 1;
                    else
                        chip8->V[0xF] &= 0;

                    chip8->V[(chip8->opcode & 0x0F00) >> 8] -= chip8->V[(chip8->opcode & 0x00F0) >> 4];
                    chip8->pc += 2;
                break;
    
                case 0x0006: // 8XY6: Shifts VX right by one. VF is set to the value of the least significant bit of VX before the shift
                    chip8->V[0xF] = chip8->V[(chip8->opcode & 0x0F00) >> 8] & 7;
                    chip8->V[(chip8->opcode & 0x0F00) >> 8] = chip8->V[(chip8->opcode & 0x0F00) >> 8] >> 1;
                    chip8->pc += 2;
                break;
    
                case 0x0007: // 8XY7: Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there isn't
                    if(((int)chip8->V[(chip8->opcode & 0x0F00) >> 8] - (int)chip8->V[(chip8->opcode & 0x00F0) >> 4]) > 0)
                        chip8->V[0xF] = 1;
                    else
                        chip8->V[0xF] &= 0;

                    chip8->V[(chip8->opcode & 0x0F00) >> 8] = chip8->V[(chip8->opcode & 0x00F0) >> 4] - chip8->V[(chip8->opcode & 0x0F00) >> 8];
                    chip8->pc += 2;
                break;
    
                case 0x000E: // 8XYE: Shifts VX left by one. VF is set to the value of the most significant bit of VX before the shift
                    chip8->V[0xF] = chip8->V[(chip8->opcode & 0x0F00) >> 8] >> 7;
                    chip8->V[(chip8->opcode & 0x0F00) >> 8] = chip8->V[(chip8->opcode & 0x0F00) >> 8] << 1;
                    chip8->pc += 2;
                break;
                default: printf("Wrong opcode: %X\n", chip8->opcode); getchar();
                        
            }
        break;
    
        case 0x9000: // 9XY0: Skips the next instruction if VX doesn't equal VY
            if(chip8->V[(chip8->opcode & 0x0F00) >> 8] != chip8->V[(chip8->opcode & 0x00F0) >> 4])
                chip8->pc += 4;
            else
                chip8->pc += 2;
        break;
    
        case 0xA000: // ANNN: Sets I to the address NNN
            chip8->I = chip8->opcode & 0x0FFF;
            chip8->pc += 2;
        break;
    
        case 0xB000: // BNNN: Jumps to the address NNN plus V0
            chip8->pc = (chip8->opcode & 0x0FFF) + chip8->V[0];
        break;
    
        case 0xC000: // CXNN: Sets VX to a random number and NN
            chip8->V[(chip8->opcode & 0x0F00) >> 8] = rand() & (chip8->opcode & 0x00FF);
            chip8->pc += 2;
        break;
    
        case 0xD000: // DXYN: Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and a height of N pixels
            {
                short vx = chip8->V[(chip8->opcode & 0x0F00) >> 8];
                short vy = chip8->V[(chip8->opcode & 0x00F0) >> 4];
                short height = chip8->opcode & 0x000F;  
                chip8->V[0xF] = 0;
            
                for(short y = 0; y < height; y++){
                    short pixel = chip8->memory[chip8->I + y];
                    for(short x = 0; x < 8; x++){
                        if(pixel & (0x80 >> x)){
                            if(chip8->gfx[x+vx+(y+vy)*64])
                                chip8->V[0xF] = 1;
                            chip8->gfx[x+vx+(y+vy)*64] ^= 1;
                        }
                    }
                }
                chip8->pc += 2;
                chip8->draw_flag = TRUE;
            }
        break;
    
        case 0xE000:
            switch(chip8->opcode & 0x000F){
                case 0x000E: // EX9E: Skips the next instruction if the key stored in VX is pressed
                    if(chip8->key[chip8->V[(chip8->opcode & 0x0F00) >> 8]]) {
                        chip8->pc += 4;
                    }else {
                        chip8->pc += 2;
                    }
                break;
                            
                case 0x0001: // EXA1: Skips the next instruction if the key stored in VX isn't pressed
                    if(!chip8->key[chip8->V[(chip8->opcode & 0x0F00) >> 8]]) {
                        chip8->pc += 4;
                    }else {
                        chip8->pc += 2;
                    }
                break;

                default: printf("Wrong opcode: %X\n", chip8->opcode); getchar();
                        
            }
        break;
    
        case 0xF000:
            switch(chip8->opcode & 0x00FF){
                case 0x0007: // FX07: Sets VX to the value of the delay timer
                    chip8->V[(chip8->opcode & 0x0F00) >> 8] = chip8->delay_timer;
                    chip8->pc += 2;
                break;
    
                case 0x000A: // FX0A: A key press is awaited, and then stored in VX
                    {
                        for(short i = 0; i < KEY_SIZE; i++)
                        if(chip8->key[i]) {
                            chip8->V[(chip8->opcode & 0x0F00) >> 8] = i;
                            chip8->pc += 2;
                        }
                    }
                   
                        
                break;
    
                case 0x0015: // FX15: Sets the delay timer to VX
                    chip8->delay_timer = chip8->V[(chip8->opcode & 0x0F00) >> 8];
                    chip8->pc += 2;
                break;
    
                case 0x0018: // FX18: Sets the sound timer to VX
                    chip8->sound_timer = chip8->V[(chip8->opcode & 0x0F00) >> 8];
                    chip8->pc += 2;
                break;
    
                case 0x001E: // FX1E: Adds VX to I
                    chip8->I += chip8->V[(chip8->opcode & 0x0F00) >> 8];
                    chip8->pc += 2;
                break;

                case 0x0029: // FX29: Sets I to the location of the sprite for the character in VX. Characters 0-F (in hexadecimal) are represented by a 4x5 font
                    chip8->I = chip8->V[(chip8->opcode & 0x0F00) >> 8] * 5;
                    chip8->pc += 2;
                break;
    
                case 0x0033: // FX33: Stores the Binary-coded decimal representation of VX, with the most significant of three digits at the address in I, the middle digit at I plus 1, and the least significant digit at I plus 2
                    chip8->memory[chip8->I] = chip8->V[(chip8->opcode & 0x0F00) >> 8] / 100;
                    chip8->memory[chip8->I+1] = (chip8->V[(chip8->opcode & 0x0F00) >> 8] / 10) % 10;
                    chip8->memory[chip8->I+2] = chip8->V[(chip8->opcode & 0x0F00) >> 8] % 10;
                    chip8->pc += 2;
                break;
    
                case 0x0055: // FX55: Stores V0 to VX in memory starting at address I
                    for(short i = 0; i <= ((chip8->opcode & 0x0F00) >> 8); i++)
                        chip8->memory[chip8->I+i] = chip8->V[i];
                    chip8->pc += 2;
                break;
    
                case 0x0065: //FX65: Fills V0 to VX with values from memory starting at address I
                    for(short i = 0; i <= ((chip8->opcode & 0x0F00) >> 8); i++)
                        chip8->V[i] = chip8->memory[chip8->I + i];
                    chip8->pc += 2;
                break;
                        
                default: printf("Wrong opcode: %X\n", chip8->opcode); getchar();
            }
        break;
        default: printf("Wrong opcode: %X\n", chip8->opcode); getchar();
                 
    }

    

    if(chip8->delay_timer > 0) {
        chip8->delay_timer--;
    }
    if(chip8->sound_timer > 0) {
        if(chip8->sound_timer == 1) {
            printf("\a");
        }
        chip8->sound_timer--;
    }
    return TRUE;
}