#pragma once
#include "ALU.h"
#include "Registers.h"
#include "Memory.h"
#include "tools.h"
#include <iostream>
#include <string.h>


class controlUnit{
    public:
        controlUnit( Registers* _reg, ALU* _alu, Memory* _mem);
        void decode(uint8_t instruction);
        void execute(uint8_t instruction);
        void reset();
        void printDebug();
        void GetOperands(uint8_t instruction);
        uint8_t flags;
        void run();

    private:
        uint8_t* flagsptr;
        uint8_t opcode;
        Register* RD;
        Register* RS;

        Registers* _reg;
        ALU* _alu;
        Memory* _mem;

        uint8_t operands[2];

        uint32_t controlCodes = -1;
        char* instName;
};