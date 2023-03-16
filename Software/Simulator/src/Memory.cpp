#include "Memory.h"
#include <cstddef>

Memory::Memory(){
    this->_size = 0x8000;
    this->_RAM = new uint8_t[this->_size];
}

Memory::~Memory(){
    free(this->_RAM);
    free((void*)(this->_ROM));
}

void Memory::init(std::string rom, std::string ram){
    FILE* romFile = fopen(rom.c_str(), "rb");
    FILE* ramFile = fopen(ram.c_str(), "rb");
    uint8_t* interimROM = new uint8_t[this->_size];
    fread(interimROM, 1, this->_size, romFile);
    fread(this->_RAM, 1, this->_size, ramFile);
    this->_ROM = interimROM;
    fclose(romFile);
}