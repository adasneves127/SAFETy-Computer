#include "ALU.h"


ALU::ALU(){
    std::cout << "ALU Initialized\n";
}

void ALU::reset(){
}

void ALU::add(Register* RD, Register* RS){
    RD->set(RD->get() + RS->get());
    if(RD->get() == 0){
        *(this->flags) &= 0b11111101;
    } else {
        *(this->flags) |= ~(0b11111101);
    }
}

void ALU::sub(Register* RD, Register* RS){
    RD->set(RD->get() - RS->get());

    if(RD->get() == 0){
        *(this->flags) &= 0b11111101;
    } else {
        *(this->flags) |= ~(0b11111101);
    }
}

void ALU::nand(Register* RD, Register* RS){
    RD->set(~(RD->get() & RS->get()));

    if(RD->get() == 0){
        *(this->flags) &= 0b11111101;
    } else {
        *(this->flags) |= ~(0b11111101);
    }
}

void ALU::cmp(Register* RD, Register* RS){
    if(RD->get() == RS->get()){
        *(this->flags) &= 0b11111101;
    } else {
        *(this->flags) |= ~(0b11111101);
    }
}


void ALU::add(Register* RD, uint8_t imm){
    RD->set(RD->get() + imm);

    if(RD->get() == 0){
        *(this->flags) &= 0b11111101;
    } else {
        *(this->flags) |= ~(0b11111101);
    }
}

void ALU::sub(Register* RD, uint8_t imm){
    RD->set(RD->get() - imm);

    if(RD->get() == 0){
        *(this->flags) &= 0b11111101;
    } else {
        *(this->flags) |= ~(0b11111101);
    }
}

void ALU::cmp(Register* RD, uint8_t imm){
    if(RD->get() == imm){
        *(this->flags) &= 0b11111101;
    } else {
        *(this->flags) |= ~(0b11111101);
    }
}

void ALU::setFlags(uint8_t* flags){
    this->flags = flags;
}

void ALU::printDebug(){
    std::cout << "\tHB   VNC\n";
    std::cout << "\t" << std::bitset<8>(*(this->flags)) << "\n";
}
