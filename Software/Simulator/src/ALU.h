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
};