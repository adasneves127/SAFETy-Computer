#pragma once
#include "Registers.h"

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

    private:
        uint8_t flags;
};