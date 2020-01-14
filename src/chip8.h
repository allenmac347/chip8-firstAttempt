#ifndef CHIP8_H
#define CHIP8_H

#include <stdlib.h>
#include <iostream>

class Chip8{
    //For reference, unsigned char is 8 bits (1 byte), unsigned short is 16 bits (2 bytes)
    private:
        /*Current opcode*/
        unsigned short opcode; 

        /*Memory layout is as follows:
        Font set is at first 512 bytes (0x000-0x1ff)
        Programs start at memory location (0x200)
        */
        unsigned char memory[4096];
        
        /*Chip 8 has 16 data registers labeled as 0, 1, ..., F*/
        unsigned char V[16];
        
        /*Special addres register I, which is 16 bits wide*/ 
        unsigned short I; 
        
        /*Program counter*/
        unsigned short pc;

        /*Mapping of chip8 graphics screen*/
        unsigned char graphics[64*32]; 

        /*Timer for game events*/
        unsigned char delay_timer;

        /*Timer for sound events*/
        unsigned char sound_timer;
        
        /*Stack*/
        unsigned short stack[16]; 
        
        /*Stack pointer, can only have values from 0-15 since we have 16 available stack frames*/
        unsigned short sp;  

        /*HEX keypad map*/
        unsigned char key[16]; 

    public:
        Chip8(); 

        void nextCylce(); 

        void detectKeyPress();

        void clearDisp(); 

        void draw(unsigned char v_x, unsigned char v_y, unsigned char height); 

};

#endif