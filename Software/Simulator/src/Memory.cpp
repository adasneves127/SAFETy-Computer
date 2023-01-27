#include "Memory.h"
#include <iostream>
#include <fstream>
#include "Stack.h"

Memory::Memory(){

    for(int i = 0; i < 0x8000; i++){
        RAM[i] = 0;
        HEADS[i] = 0;

    }

    for(int i = 0; i < 0x8000; i++){
        ROM[i]= 0;
    }
    std::cout << "Memory Initialized!\n";

    this->keyboardBuffer = new char[0x400];
    this->inputIdx = this->outputIdx = 0;
    this->LCD_screen = std::string("");
    //this->_sysStack = new Stack((uint16_t)0x8000);
    this->_sysStack = new Stack();
    this->raw = false;
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
    uint8_t ins = ROM[PC];
    PC++;
    return ins;
}

void Memory::enableRaw(){
    //Enable raw mode
    printf("Raw mode enabled\n");
    this->raw = true;
}


void Memory::jump(uint16_t addr){
    //Set the PC to the address
    PC = addr;
}

uint8_t Memory::read(uint16_t address){
    if(address == 0x7FF0){
        if(this->inputIdx == this->outputIdx){
            //Get more characters from the standard input
            this->inputIdx = this->outputIdx = 0;
            std::cin.getline(keyboardBuffer, 0x400);
            int i = 0;
            for(char* x = keyboardBuffer; *x != 0; x++){
                if(x == 0){
                    this->inputIdx = i;
                    break;
                } else{
                    i++;
                }
            }
        }
        return keyboardBuffer[outputIdx++];
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
void Memory::top(Register* RD){
    //Pop the value from the stack
    //Save it to the register
    RD->set(RAM[stackPage * 256 + stackPointer-1]);
}

uint8_t Memory::pop(){
    //Pop the value from the stack
    //Return it
    stackPointer--;
    return RAM[stackPage * 256 + stackPointer];
}

void Memory::store(uint16_t address, Register* RS){

    //Hardware Registers:
    if(address == 0xF5ea){
        if(!(this->raw)){
            printf("%c", RS->get());
        } else {
            printf("%02x ", RS->get() + 0x20);
        }
        return;
    }
    if(address == 0x7FF1){
        this->LCD_screen.push_back((char) RS->get());
        printf("+--------------------+\n");
        std::cout << this->LCD_screen << "\n";
        printf("+--------------------+\n");
        return;
    }

    //Store the value in the register to the address
    if(address > 0x8000){
        //This is a ROM Address.
        //Normally, ROM Writing is a big no-no...
        //However, with SAFETy, we do have some hardware registeres stored in ROM.
        //TODO: Implement Hardware Registers (IO, Video Out).
    } else {
        //Read from RAM
        RAM[address] = RS->get();
    }
}

void Memory::JSR(uint16_t addr){
    uint8_t lower8 = this->PC & 0xFF;
    uint8_t upper8 = (this->PC>> 8) & 0xFF;
    this->_sysStack->push(lower8);
    this->_sysStack->push(upper8);
    this->jump(addr);
}
void Memory::RET(){
    uint8_t upper8 = this->_sysStack->pop();
    uint8_t lower8 = this->_sysStack->pop();
    uint16_t addr = (upper8 << 8 ) | lower8;
    this->jump(addr + 2);
}

void Memory::save(char* ram, char* rom, char* heads){

    FILE *ramFile;
    ramFile = fopen(ram, "wb");
    if(ramFile == NULL){
        std::cout << "\u001b[31mErr:\u001b[0m Could not open RAM file\n";
        return;
    }
    fwrite(RAM, 1, 0x8000, ramFile);
    fclose(ramFile);

    uint8_t ROMFILE[(unsigned int)(INT32_MAX)+1];

    FILE *romFile;
    romFile = fopen(rom, "rb");
    if(romFile == NULL){
        std::cout << "\u001b[31mErr:\u001b[0m Could not open RAM file\n";
        return;
    }
    fread(ROMFILE, 1, sizeof(ROMFILE), romFile);
    fclose(romFile);
    romFile = fopen(ram, "wb");
    for(int i = 0; i < sizeof(ROMFILE); i+= 0x8000){
        if(i == ((memPageH * 256) + memPageL) * 0x8000){
            fwrite(RAM, 1, 0x8000, romFile);
        } else{
            fwrite(&ROMFILE[i], 1, 0x8000, romFile);
        }
    }
    fclose(romFile);

    FILE *headsFile;
    headsFile = fopen(heads, "wb");
    if(headsFile == NULL){
        std::cout << "\u001b[31mErr:\u001b[0m Could not open RAM file\n";
        return;
    }
    fwrite(&PC, 1, 2, headsFile);
    fwrite(&stackPointer, 1, 1, headsFile);
    fwrite(&stackPage, 1, 1, headsFile);
    fwrite(&memPageL, 1, 1, headsFile);
    fwrite(&memPageH, 1, 1, headsFile);


}

void Memory::call(Registers* _reg, uint16_t addr, controlUnit* _control){
    
}