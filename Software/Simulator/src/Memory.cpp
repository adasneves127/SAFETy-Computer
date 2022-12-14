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

uint8_t Memory::read(uint16_t address){
    if(address == 0x7FF0){
    }
    //Read from the address
    if(address > 0x8000){
        //Read from ROM
        return ROM[address - 0x8000];
    } else {
        //Read from RAM
        return RAM[address];
    }
}

void Memory::put(uint16_t address, uint8_t data){
    if(address > 0x8000){
        //Read from ROM
        ROM[address - 0x8000] = data;
    } else {
        //Read from RAM
        RAM[address] = data;
    }
}


uint16_t Memory::getPC(){
    //Get the PC
    return this->PC;
}

void Memory::reset(){
    //Reset the PC
    PC = 0;
}

void Memory::push(uint8_t imm){
    //Save the value to the stack
    RAM[stackPage * 256 + stackPointer] = imm;
    stackPointer++;
}

void Memory::push(Register* RD){
    //Save the value to the stack
    RAM[stackPage * 256 + stackPointer] = RD->get();
    stackPointer++;
}

void Memory::pop(Register* RD){
    //Pop the value from the stack
    //Save it to the register
    stackPointer--;
    RD->set(RAM[stackPage * 256 + stackPointer]);
}

uint8_t Memory::pop(){
    //Pop the value from the stack
    //Return it
    stackPointer--;
    return RAM[stackPage * 256 + stackPointer];
}

void Memory::store(uint16_t address, Register* RS){
    //Store the value in the register to the address
    if(address > 0x8000){
        if(address == 0xF5ea){
            printf("%c", RS->get());
        }
        //This is a ROM Address.
        //Normally, ROM Writing is a big no-no...
        //However, with SAFETy, we do have some hardware registeres stored in ROM.
        //TODO: Implement Hardware Registers (IO, Video Out).
    } else {
        //Read from RAM
        RAM[address] = RS->get();
    }
}