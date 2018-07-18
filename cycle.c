#include "cycle.h"

char chip8_cycle(chip8_t *chip8) {
    chip8->opcode = chip8->memory[chip8->pc] << 8 | chip8->memory[chip8->pc+1];

    #ifdef DEBUG
    printf("Executing opcode=0x%04X pc=%d mem[pc]=%d mem[pc+1]=%d\n", chip8->opcode, chip8->pc, chip8->memory[chip8->pc], chip8->memory[chip8->pc+1]);
    #endif //

    //getc(stdin);

    switch(chip8->opcode & 0xF000) {
        case 0x0000:
            switch(chip8->opcode & 0x000F) {
                case 0x0000: // opcode is 0x00E0, clear the screen
                    for(unsigned short i=0; i < GFX_SIZE; i++) {
                        chip8->gfx[i] = 0;
                    }
                    chip8->draw_flag = TRUE;
                    chip8->pc += 2;
                break;

                case 0x000E: // opcode is 0x00EE, return from the subroutine
                    --chip8->sp;
                    chip8->pc = chip8->stack[chip8->sp];
                    chip8->pc += 2;
                break;

                default:
                    #ifdef DEBUG
                    printf("Unable to understand [1] opcode 0x%04X\n", chip8->opcode);
                    #endif // DEBUG
                    return FALSE;
            }
        break;
        
        case 0x1000: // opcode is 1NNN, Jump to address NNN
            chip8->pc = chip8->opcode & 0x0FFF;
        break;

        case 0x2000:
            chip8->stack[chip8->sp] = chip8->pc;
            ++(chip8->sp);
            chip8->pc = chip8->opcode & 0x0FFF;
        break;

        case 0x3000:
            if( chip8->V[(chip8->opcode & 0x0F00) >> 8] == (chip8->opcode & 0x00FF) ) {
                chip8->pc += 4;
            }else {
                chip8->pc += 2;
            }
        break;

        case 0x4000:
            if( chip8->V[(chip8->opcode & 0x0F00) >> 8] != (chip8->opcode & 0x00FF) ) {
                chip8->pc += 4;
            }else {
                chip8->pc += 2;
            }
        break;

        case 0x5000:
            if(chip8->V[ (chip8->opcode & 0x0F00) >> 8]  == chip8->V[(chip8->opcode & 0x00F0) >> 4]) {
                chip8->pc += 4;
            }else {
                chip8->pc += 2;
            }
        break;

        case 0x6000:
            chip8->V[(chip8->opcode & 0x0F00) >> 8] += (chip8->opcode&0x00FF);
            chip8->pc += 2;
        break;

        case 0x7000: // 0x7XNN: Adds NN to VX.
			chip8->V[(chip8->opcode & 0x0F00) >> 8] += chip8->opcode & 0x00FF;
			chip8->pc += 2;
		break;

        case 0x8000:
            switch(chip8->opcode & 0x000F) {

                case 0x0000:
                    chip8->V[(chip8->opcode & 0x0F00) >> 8] = chip8->V[(chip8->opcode & 0x00F0) >> 4];
                    chip8->pc += 2;
                break;

                case 0x0001:
                    chip8->V[(chip8->opcode & 0x0F00) >> 8] = chip8->V[(chip8->opcode & 0x0F00) >> 8] | chip8->V[(chip8->opcode & 0x00F0) >> 4];
                    chip8->pc += 2;
                break;

                case 0x0002:
                    chip8->V[(chip8->opcode & 0x0F00) >> 8] = chip8->V[(chip8->opcode & 0x0F00) >> 8] & chip8->V[(chip8->opcode & 0x00F0) >> 4];
                    chip8->pc += 2;
                break;

                case 0x0003:
                    chip8->V[(chip8->opcode & 0x0F00) >> 8] = chip8->V[(chip8->opcode & 0x0F00) >> 8] ^ chip8->V[(chip8->opcode & 0x00F0) >> 4];
                    chip8->pc += 2;
                break;

                case 0x0004:
                    if(chip8->V[(chip8->opcode & 0x00F0) >> 4] > (0xFF - chip8->V[(chip8->opcode & 0x0F00) >> 8])) {
                        chip8->V[0xF] = 1; //carry
                    } else {
                        chip8->V[0xF] = 0;	
                    }
		
					chip8->V[(chip8->opcode & 0x0F00) >> 8] += chip8->V[(chip8->opcode & 0x00F0) >> 4];
					chip8->pc += 2;					
                break;

                case 0x0005:
                    if(chip8->V[(chip8->opcode & 0x00F0) >> 4] > chip8->V[(chip8->opcode & 0x0F00) >> 8]) {
                        chip8->V[0xF] = 0; // there is a borrow
                    }
						
					else {
                        chip8->V[0xF] = 1;
                    }
										
					chip8->V[(chip8->opcode & 0x0F00) >> 8] -= chip8->V[(chip8->opcode & 0x00F0) >> 4];
					chip8->pc += 2;
                break;

                case 0x0006:
                    chip8->V[0xF] = chip8->V[(chip8->opcode & 0x0F00) >> 8] & 0x1;
					chip8->V[(chip8->opcode & 0x0F00) >> 8] >>= 1;
					chip8->pc += 2;
                break;

                case 0x0007:
                    if(chip8->V[(chip8->opcode & 0x0F00) >> 8] > chip8->V[(chip8->opcode & 0x00F0) >> 4]) {
                        chip8->V[0xF] = 0; // there is a borrow
                    }
						
					else {
                        chip8->V[0xF] = 1;
                    }
						
					chip8->V[(chip8->opcode & 0x0F00) >> 8] = chip8->V[(chip8->opcode & 0x00F0) >> 4] - chip8->V[(chip8->opcode & 0x0F00) >> 8];				
					chip8->pc += 2;
                break;

                case 0x000E: 
                    chip8->V[0XF] = chip8->V[(chip8->opcode & 0x0FF) >> 8] >> 7;
                    chip8->V[(chip8->opcode & 0x0F00) >> 8] <<=1;
                    chip8->pc += 2;
                break;

                default:
                    #ifdef DEBUG
                    printf("Unable to understand [2] opcode 0x%X\n", chip8->opcode);
                    #endif // DEBUG
                    return FALSE;
            }
        break;

        case 0x9000:
            if(chip8->V[(chip8->opcode & 0x0F00) >> 8] != chip8->V[(chip8->opcode & 0x00F0) >> 4]) {
                chip8->pc += 4;
            }else {
                chip8->pc += 2;
            }
        break;

        case 0xA000:
            chip8->I = chip8->opcode & 0x0FFF;
            chip8->pc += 2;
        break;

        case 0xB000:
            chip8->pc = chip8->opcode & 0x0FFF; + chip8->V[0];
        break;

        case 0xC000: // 0xCXNN, sets V[X] to rand() & NN
            chip8->V[chip8->opcode&0x0F00] = rand() & (chip8->opcode&0x00FF);
            chip8->pc += 2;
        break;

        case 0xD000: // DXYN, draws sprite at (V[X], V[Y]) that has width 8 and height N pixels
        {
            unsigned short x = chip8->V[(chip8->opcode & 0x0F00) >> 8];
            unsigned short y = chip8->V[(chip8->opcode & 0x00F0) >> 4];
            unsigned short height = chip8->opcode & 0x000F;
            unsigned short pixel;

            chip8->V[0xF] = 0;
            for(unsigned short yline=0; yline < height; yline++) {
                pixel = chip8->memory[chip8->I + yline];
                for(unsigned short xline=0; xline < 8; xline++) {
                    if((pixel & (0x80 >> xline) ) != 0) {
                        if(chip8->gfx[(x + xline + ((y + yline) * 64))] == 1) {
                            chip8->V[0xF] = 1;
                        }
                        chip8->gfx[x + xline + ((y + yline) * 64)] ^= 1;
                    }
                }
            }

            chip8->draw_flag = TRUE;
            chip8->pc += 2;
        }

        break;

        case 0xE000:
            switch(chip8->opcode & 0x00FF) {
                case 0x009E:
                    if(chip8->key[chip8->V[(chip8->opcode & 0x0F00) >> 8]] != 0) {
                        chip8->pc += 4;
                    }else {
                        chip8->pc += 2;
                    }
                break;

                case 0x00A1:
                    if(chip8->key[chip8->V[(chip8->opcode & 0x0F00) >> 8]] == 0) {
                        chip8->pc += 4;
                    }else {
                        chip8->pc += 2;
                    }
                break;


                default:
                    #ifdef DEBUG
                    printf("Unable to understand [3] opcode 0x%X\n", chip8->opcode);
                    #endif // DEBUG
                    return FALSE;
            }
        break;

        case 0xF000:
            switch(chip8->opcode & 0x00FF) {
                case 0x0007: // FX07: Sets VX to the value of the delay timer
					chip8->V[(chip8->opcode & 0x0F00) >> 8] = chip8->delay_timer;
					chip8->pc += 2;
				break;

                case 0x000A:
                    {
                        char key_pressed = FALSE;
                        for(unsigned char i=0; i < KEY_SIZE; i++) {
                            if(chip8->key[i] != 0) {
                                chip8->V[(chip8->opcode & 0x0F00) >> 8] = i;
                                key_pressed = TRUE;
                            }
                        }
                        if(!key_pressed) {
                            return TRUE;
                        }
                        chip8->pc += 2;
                    }

                break;

                case 0x0015:
                    chip8->delay_timer = chip8->V[(chip8->opcode & 0x0F00) >> 8];
					chip8->pc += 2;
                break;

                case 0x0018:
                    chip8->sound_timer = chip8->V[(chip8->opcode & 0x0F00) >> 8];
					chip8->pc += 2;
                break;

                case 0x001E:
                    if(chip8->I + chip8->V[(chip8->opcode & 0x0F00) >> 8] > 0xFFF) {
                        chip8->V[0xF] = 1;
                    }else {
                        chip8->V[0xF] = 0;
                    }
                    chip8->I += chip8->V[(chip8->opcode & 0x0F00) >> 8];
					chip8->pc += 2;
                break;

                case 0x0029: // FX29: Sets I to the location of the sprite for the character in VX. Characters 0-F (in hexadecimal) are represented by a 4x5 font
					chip8->I = chip8->V[(chip8->opcode & 0x0F00) >> 8] * 0x5;
					chip8->pc += 2;
				break;

                case 0x0033: // FX33: Stores the Binary-coded decimal representation of VX at the addresses I, I plus 1, and I plus 2
					chip8->memory[chip8->I]     = chip8->V[(chip8->opcode & 0x0F00) >> 8] / 100;
					chip8->memory[chip8->I + 1] = (chip8->V[(chip8->opcode & 0x0F00) >> 8] / 10) % 10;
					chip8->memory[chip8->I + 2] = (chip8->V[(chip8->opcode & 0x0F00) >> 8] % 100) % 10;					
					chip8->pc += 2;
				break;

                case 0x0055: // FX55: Stores V0 to VX in memory starting at address I					
					for (unsigned char i = 0; i <= ((chip8->opcode & 0x0F00) >> 8); ++i)
						chip8->memory[chip8->I + i] = chip8->V[i];	

					// On the original interpreter, when the operation is done, I = I + X + 1.
					chip8->I += ((chip8->opcode & 0x0F00) >> 8) + 1;
					chip8->pc += 2;
				break;

                case 0x0065: // FX65: Fills V0 to VX with values from memory starting at address I					
					for (unsigned char i = 0; i <= ((chip8->opcode & 0x0F00) >> 8); ++i)
						chip8->V[i] = chip8->memory[chip8->I + i];			

					// On the original interpreter, when the operation is done, I = I + X + 1.
					chip8->I += ((chip8->opcode & 0x0F00) >> 8) + 1;
					chip8->pc += 2;
				break;


                default:
                    #ifdef DEBUG
                    printf("Unable to understand [4] opcode 0x%X\n", chip8->opcode);
                    #endif // DEBUG
                    return FALSE;
            }
        break;

        default:
            #ifdef DEBUG
            printf("Unable to understand [5] opcode 0x%X\n", chip8->opcode);
            #endif // DEBUG
            return FALSE;
    }

    if(chip8->delay_timer > 0) {
        chip8->delay_timer--;
    }
    if(chip8->sound_timer > 0) {
        if(chip8->sound_timer == 1) {
            // Beep
        }
        chip8->sound_timer--;
    }
    return TRUE;
}