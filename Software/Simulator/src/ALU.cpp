#include "ALU.h"


ALU::ALU(){
    
}

void ALU::reset(){
}

void ALU::add(Register* RD, Register* RS){
    RD->set(RD->get() + RS->get());
}

void ALU::sub(Register* RD, Register* RS){
    RD->set(RD->get() - RS->get());
}

void ALU::nand(Register* RD, Register* RS){
    RD->set(~(RD->get() & RS->get()));
}

void ALU::cmp(Register* RD, Register* RS){
    if(RD->get() == RS->get()){
        this->flags = 0;
    } else if(RD->get() > RS->get()){
        this->flags = 1;
    } else {
        this->flags = 2;
    }
}


void ALU::add(Register* RD, uint8_t imm){
    RD->set(RD->get() + imm);
}

void ALU::sub(Register* RD, uint8_t imm){
    RD->set(RD->get() - imm);
}