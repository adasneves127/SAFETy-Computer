#include "Control.h"


controlUnit::controlUnit(Registers* _reg, ALU* _alu, Memory* _mem){
    //Gives the control unit access to the registers, ALU, and memory.
    this->_reg = _reg;
    this->_alu = _alu;
    this->_mem = _mem;

    //Resets the control unit.
    this->flags = 0;
    this->opcode = 0;
    this->RD = nullptr;
    this->RS = nullptr;
}


void controlUnit::decode(unsigned char instruction){
    int rdNum = (instruction & 0b00001100) >> 2;
    int rsNum = (instruction & 0b00000011);
    this->RD = this->_reg->getRegister(rdNum);
    this->RS = this->_reg->getRegister(rsNum);
}
void controlUnit::execute(unsigned char instruction){}
void controlUnit::reset(){}
void controlUnit::printDebug(){}