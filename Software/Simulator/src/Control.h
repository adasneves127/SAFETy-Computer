#pragma once
#include "ALU.h"
#include "Registers.h"
#include "Memory.h"
#include <iostream>

class controlUnit{
    public:
        controlUnit();
        void decode(unsigned char instruction);
        void execute(unsigned char instruction);
        void reset();
        void printDebug();
    private:
        unsigned char flags;
        unsigned char opcode;
        Register* RD;
        Register* RS;

        unsigned int controlCodes = -1;
};