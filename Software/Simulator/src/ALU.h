#pragma once
#include "Registers.h"
#include <iostream>
#include <bitset>

class ALU{

    public:
        ALU();
        void add(Register* RD, Register* RS);
        void sub(Register* RD, Register* RS);
        void nand(Register* RD, Register* RS);
        void cmp(Register* RD, Register* RS);
        void reset();


        void add(Register* RD, uint8_t imm);
        void sub(Register* RD, uint8_t imm);
        void cmp(Register* RD, uint8_t imm);
        void setFlags(uint8_t* flags);

        void printDebug();

    private:
        uint8_t* flags;
};