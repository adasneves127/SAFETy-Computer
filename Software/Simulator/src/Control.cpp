#include "Control.h"


controlUnit::controlUnit(Registers* _reg, ALU* _alu, Memory* _mem){
    //Gives the control unit access to the registers, ALU, and memory.
    this->_reg = _reg;
    this->_alu = _alu;
    this->_mem = _mem;

    //Resets the control unit.
    this->reset();
}


void controlUnit::decode(uint8_t instruction){
    int rdNum = (instruction & 0b00001100) >> 2;
    int rsNum = (instruction & 0b00000011);
    this->RD = this->_reg->getRegister(rdNum);
    this->RS = this->_reg->getRegister(rsNum);
}
void controlUnit::GetOperands(uint8_t instruction){
    this->operands[0] = this->_mem->read(this->_mem->getPC() + 1);
    this->operands[1] = this->_mem->read(this->_mem->getPC() + 2);
}

void controlUnit::execute(uint8_t instruction){

}

void controlUnit::reset(){
    //Resets the control unit.
    this->flags = 0;
    this->opcode = 0;
    this->RD = nullptr;
    this->RS = nullptr;
    this->operands[0] = NULL;
    this->operands[1] = NULL;
}
void controlUnit::printDebug(){}

