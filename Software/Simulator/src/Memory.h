#pragma once
#include "Registers.h"
#include <stdlib.h>
#include <string>
class Memory{
    public:
        Memory();
        ~Memory();
        void fill(uint8_t value);
        void init(std::string rom, std::string ram);
    private:
        uint8_t* _RAM;
        const uint8_t* _ROM;
        int _size;
};