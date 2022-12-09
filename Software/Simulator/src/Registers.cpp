#include "Registers.h"
#include <iostream>


#pragma region Register
Register::Register(){
    regVal = 0;
}
void Register::initialize(char name){
    regChr = name;
}
void Register::set(uint8_t val){
    regVal = val;
}
uint8_t Register::get(){
    return regVal;
}
void Register::printDebug(){
    std::cout << regChr << ": " << (int)regVal;
}
#pragma endregion

Registers::Registers(){
    char names[4] = {'A', 'B', 'X', 'Y'};
    for(int i = 0; i < 4; i++){
        regs[i] = new Register();
        regs[i]->initialize(names[i]);
    }
    std::cout << "Registers Created!\n";
}

void Registers::set(int reg, uint8_t value){
    regs[reg]->set(value);
}

uint8_t Registers::get(int reg){
    return regs[reg]->get();
}

void Registers::printDebug(){
    for(int i = 0; i < 4; i++){
        regs[i]->printDebug();
        if(i != 3){
            std::cout << ", ";
        }
    }
    std::cout << "\n";
}

Register* Registers::getRegister(int reg){
    return regs[reg];
}