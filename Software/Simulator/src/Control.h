#pragma once
#include "ALU.h"
#include "Registers.h"
#include "Memory.h"
#include <iostream>

class controlUnit{
    public:
        controlUnit( Registers* _reg, ALU* _alu, Memory* _mem);
        void decode(unsigned char instruction);
        void execute(unsigned char instruction);
        void reset();
        void printDebug();

    private:
        unsigned char flags;
        unsigned char opcode;
        Register* RD;
        Register* RS;

        Registers* _reg;
        ALU* _alu;
        Memory* _mem;

        unsigned int controlCodes = -1;
};