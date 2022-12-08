#include "Memory.h"
#include <iostream>
#include <fstream>

Memory::Memory(){

    for(int i = 0; i < 0x8000; i++){
        RAM[i] = 0;
        HEADS[i] = 0;

    }

    for(int i = 0; i < 0x8000; i++){
        ROM[i]= 0;
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

void Memory::changePage(){
    FILE* rom;
    rom = fopen(romFile, "rb");
    if(rom == NULL){
        std::cout << "\u001b[31mErr:\u001b[0m Could not open ROM file\n";
        return;
    }
    fseek(rom, ((memPageH * 256) + memPageL) * 0x8000, SEEK_SET);
    fread(ROM, 1, 0x8000, rom);
    fclose(rom);
}

void Memory::loadROM(char* fileName){
    this->romFile = fileName;
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
            fread(ROM, 1, 0x8000, rom);
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

uint8_t Memory::nextIns(){
    //Get the next instruction from the ROM
    //Increment the PC
    //Return the instruction
    return 0;
}

void Memory::jump(uint16_t addr){
    //Set the PC to the address
    PC = addr;
}