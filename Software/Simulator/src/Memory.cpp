#include "Memory.h"
#include <iostream>
#include <fstream>

Memory::Memory(){

    for(int i = 0; i < 0x8000; i++){
        RAM[i] = 0;
        HEADS[i] = 0;

    }

    for(int i = 0; i < 0x8000; i++){
        for(int j = 0; j < 255; j++){
            for(int k = 0; k < 255; k++){
                ROM[k][j][i] = 0;
            }
        }

    }
    std::cout << "Memory Initialized!\n";
}

void Memory::loadRAM(char* fileName){
    //Read in a binary file
    //Load it into RAM
    FILE *ram;
    ram = fopen(fileName, "rb");
    if(ram == NULL){
        std::cout << "\u001b[31mErr:\u001b[0m Could not open RAM file\n";
        return;
    }

    fread(RAM, 1, 0x8000, ram);
    fclose(ram);
    
}

void Memory::loadROM(char* fileName){
    //Read in a binary file
    //Load it into ROM
    FILE *rom;
    rom = fopen(fileName, "rb");
    if(rom == NULL){
        std::cout << "\u001b[31mErr:\u001b[0m Could not open ROM file\n";
        return;
    }
    for(int i = 0; i < 0xFF; i++){
        for(int j = 0; j < 0xFF; j++){
            fread(ROM[i][j], 1, 0x8000, rom);
        }
    }
}

void Memory::loadHeaders(char* fileName){
    //Read in a binary file
    //Load it into HEADS
    FILE *heads;
    heads = fopen(fileName, "rb");
    if(heads == NULL){
        std::cout << "\u001b[31mErr:\u001b[0m Could not open Headers file\n";
        return;
    }
    fread(HEADS, 1, 0x8000, heads);
    fclose(heads);
}
