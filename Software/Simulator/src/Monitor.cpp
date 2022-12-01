#include "Monitor.h"
#include "Memory.h"
#include "Registers.h"

Monitor::Monitor(){

}

void Monitor::init(Memory* _mem, Registers* _reg, ALU* _alu, controlUnit* _control){
    this->_mem = _mem;
    this->_reg = _reg;
    this->_alu = _alu;
    this->_control = _control;
}


void Monitor::run(){
    while(true){

    }
}