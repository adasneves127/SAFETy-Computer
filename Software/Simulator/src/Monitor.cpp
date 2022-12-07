#include "Monitor.h"
#include "Memory.h"
#include "Registers.h"
#include "tools.h"

Monitor::Monitor(){

}

void Monitor::init(Memory* _mem, Registers* _reg, ALU* _alu, controlUnit* _control){
    this->_mem = _mem;
    this->_reg = _reg;
    this->_alu = _alu;
    this->_control = _control;
}

void Monitor::doInstruction(){
    unsigned char instruction = this->_mem->nextIns();
    this->_control->decode(instruction);
    this->_control->execute(instruction);
    this->_control->printDebug();
}

void getInsAddr(char* input, char* Inst, char* addr){
    bool isInst = true;
    for(char* chr = input; *chr != '\0'; chr++){
        if(*chr == ' '){
            isInst = false;
            *Inst = 0;
            continue;
        }
        if(isInst){
            *Inst = *chr;
            Inst++;
        } else {
            *addr = *chr;
            addr++;
        }
    }
    *addr = 0;
}

void Monitor::run(){

    std::cout << "SAFETy Monitor:";
    while(true){
        //Machine Code Monitor
        std::cout << "\n> ";
        char* command = new char[256];
        std::cin.getline(command, 256);
        char* Inst = new char[256];
        char* addr = new char[256];
        getInsAddr(command, Inst, addr);
        if(std::string(Inst) == "exit"){
            return;
        }
        if(std::string(Inst) == "x"){
            while(true){
                std::cout << addr << "\n";
                unsigned short addrJump = tools::HexStringToInt(std::string(addr));
                std::cout << (int)addrJump << "\n";
                this->_mem->jump(addrJump);
                this->doInstruction();
            }
        }

    }
}