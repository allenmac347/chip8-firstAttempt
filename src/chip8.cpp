#include "chip8.h"

Chip8::Chip8(){
    /*All programs will start at memory location 512 or 0x200*/
    pc = 0x200; 
    /*Dummy placeholder for opcode*/
    opcode = 0; 
    I = 0 ; 
    /*Using the first stack frame*/
    sp = 0; 

}

Chip8::nextCylce(){
    //fetch and combine opcodes
    opcode = memory[pc]; 
    opcode = opcode << 8; 
    opcode = opcode | memory[pc + 1]; 

    //Decode opcode
    switch((opcode & 0xf000) >> 12) {
        //execute opcode
        case 0:
            switch(opcode & 0x0fff){
                //TODO: Do we have to set display flag?
                case 0x00e0:
                    //Clear the display
                    clearDisp(); 
                    pc = pc + 2;
                    break;
                case 0x00ee:
                    //Return to address stored on current stack frame
                    pc = stack[sp];
                    //Move our stack pointer to the previous stack frame
                    sp--; 
                    if(sp < 0){
                        std::cout << "Error, deallocated our base stack frame, PC jumped to junk address"
                    }
                    break;
                default:
                    pc = opcode & 0x0fff; 
                    break; 
            }
            break;
        case 0x1:
            pc = opcode & 0x0fff; 
            break; 
        case 0x2:
            //Make a new stack frame
            sp++; 
            if(15 < sp){
                std::cout << "Error, unable to allocate space on the stack"
            }
            //Save the address of last executed instruction on previous stack frame onto our new stack frame
            stack[sp] = pc; 
            pc = opcode & 0x0fff; 
            break; 
        case 0x3:
            if(V[opcode & 0x0f00 >> 8] == (opcode & 0x00ff)){
                //Skip next instruction 
                pc = pc + 2; 
            }
            pc = pc + 2; 
            break; 
        case 0x4:
            if(V[opcode & 0x0f00 >> 8] != (opcode & 0x00ff)){
                //Skip next instruction
                pc = pc + 2; 
            }
            pc = pc + 2
            break;
        case 0x5:
            if(V[opcode & 0x0f00 >> 8] == V[opcode & 0x00f0 >> 4]){
                //Skip next instruction
                pc = pc + 2; 
            }
            pc = pc + 2; 
            break; 
        case 0x6:
            V[opcode & 0x0f00 >> 8] = opcode & 0x00ff; 
            pc = pc + 2; 
            break; 
        case 0x7:
            V[opcode & 0x0f00 >> 8] += opcode & 0x00ff; 
            pc = pc + 2; 
            break; 
        case 0x8:
            switch(opcode & 0xf){
                case 0x0:
                    V[opcode & 0x0f00 >> 8] = V[opcode & 0x00f0 >> 4]; 
                    pc = pc + 2; 
                    break;
                case 0x1:
                    V[opcode & 0x0f00 >> 8] = V[opcode & 0x0f00 >> 8] | V[opcode & 0x00f0 >> 4];
                    pc = pc + 2;
                    break;
                case 0x2:
                    V[opcode & 0x0f00 >> 8] = V[opcode & 0x0f00 >> 8] & V[opcode & 0x00f0 >> 4];
                    pc = pc + 2;
                    break;
                case 0x3:
                    V[opcode & 0x0f00 >> 8] = V[opcode & 0x0f00 >> 8] ^ V[opcode & 0x00f0 >> 4];
                    pc = pc + 2;
                    break;
                case 0x4:
                    if(V[opcode &  0x0f00 >> 8] + V[opcode & 0x00f0 >> 4] > 255){
                        V[0xf] = 1; 
                    }
                    else{
                        V[0xf] = 0; 
                    }
                    V[opcode & 0x0f00 >> 8] = (V[opcode &  0x0f00 >> 8] + V[opcode & 0x00f0 >> 4]) % 0xff; 
                    pc = pc + 2; 
                    break;
                case 0x5:
                    if(V[opcode &  0x0f00 >> 8] - V[opcode & 0x00f0 >> 4] < 0){
                        V[0xf] = 0;
                    }
                    else{
                        V[0xf] = 1; 
                    }
                    V[opcode & 0x0f00 >> 8] = (V[opcode &  0x0f00 >> 8] - V[opcode & 0x00f0 >> 4]) % 0xff; 
                    pc = pc + 2; 
                    break;
                case 0x6:
                    V[0xf] = V[opcode 0x0f00 >> 8] & 0b1; 
                    V[opcode 0x0f00 >> 8] = V[opcode 0x0f00 >> 8] >> 1
                    pc = pc + 2; 
                    break;
                case 0x7:
                    if(V[opcode &  0x0f00 >> 4] - V[opcode & 0x00f0 >> 8] < 0){
                        V[0xf] = 0;
                    }
                    else{
                        V[0xf] = 1; 
                    }
                    V[opcode & 0x0f00 >> 8] = (V[opcode &  0x0f00 >> 4] - V[opcode & 0x00f0 >> 8]) % 0xff; 
                    pc = pc + 2; 
                    break;
                case 0xe: 
                    V[0xf] = V[opcode & 0x0f00 >> 8] & 0x8; 
                    V[opcode & 0x0f00 >> 8] << 1; 
                    pc = pc + 2; 
                    break;
            }
            break;
        case 0x9:
            if(V[opcode &  0x0f00 >> 8] != V[opcode & 0x00f0 >> 4]){
                //Skip next instruction 
                pc = pc + 2; 
            }
            pc = pc + 2; 
            break; 
        case 0xa:
            I = opcode & 0x0fff;
            pc = pc + 2; 
            break; 
        case 0xb:
            pc = V[0] + (opcode & 0xfff); 
            break; 
        case 0xc:
            V[opcode &  0x0f00 >> 8] = (rand() % 255 + 1) & (opcode & 0xff); 
            pc = pc + 2; 
            break;
        case 0xd:
            draw(V[opcode & 0x0f00 >> 8], V[opcode & 0x00f0 >> 4], opcode & 0xf); 
            pc = pc + 2; 
            break; 
        case 0xe:
            switch(opcode & 0xf){
                case 0xe:
                    pc = pc + 2;
                    break; 
                case 0x1:
                    pc = pc + 2; 
                    break; 
            }
            break; 
        case 0xf:
            switch(opcode & 0xff){
                case 0x7:
                    break;
                case 0xa:
                    break;  
                case 0x15:
                    break; 
                case 0x18:
                    break; 
                case 0x1e:
                    break; 
                case 0x29:
                    break; 
                case 0x33:
                    unsigned char val = V[opcode & 0xf00 >> 8]; 
                    memory[I + 2] = val % 10; 
                    val = (val - (val % 10)) / 10; 
                    memory[I + 1] = (val % 10); 
                    val = (val - (val % 10)) / 10; 
                    memory[I] = val % 10; 
                    break;
                case 0x55:
                    break; 
                case 0x65:
                    break;  
                    
            }
            break;
    }
}

Chip8::detectKeyPress(){

}

Chip8::waitKeyPress(){

}

Chip8::clearDisp(){
    for(int i = 0; i < graphics.size(); i++){
        graphics[i] = 0; 
    }
}

Chip8::draw(unsigned char v_x, unsigned char v_y, unsigned char height){
    /*Pixels are usually displayed as follows
    0 1 2 3 4
    5 6 7 8 9*
    These pixels are then stored in a 1d array where the corresponding pixel map is
    0 1 2 3 4 5 6 7 8 9*/
    bool collision = false; 
    for(unsigned char h = 0; h < height; h++){
        //Sprite data for a row of 8 pixels
        unsigned char row = memory[I + h]; 
        unsigned char startPos = (v_y + h)*64 + v_x; 
        //All sprites are 8 pixels wide
        for(unsigned char offset = 0; offset < 8; offset++){
        //XOR logic. Detects collisions
            if(graphics[startPos + offset] == 1){
                if(row & (0x80 >> offset) > 0){
                    graphics[startPos + offset] = 0; 
                    collision = true; 
                }
            }
            else{
                if(row & (0x80 >> offset) > 0){
                    graphics[startPos + offset] = 1; 
                }
            }

        }
    }
    if(collision){
        V[0xf] = 1; 
    }
    else{
        V[0xf] = 0; 
    }
    //TODO: Send graphics map to SDL
}