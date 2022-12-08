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
        unsigned short address = (unsigned short)std::stoi(std::string(addr), nullptr, 16);
        if(std::string(Inst) == "exit"){
            return;
        }
        if(std::string(Inst) == "x"){
            //Execute
        }
        if(std::string(Inst) == "i"){
            //Dump
            printf("Dumping Memory:\n");
            printf("      00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F\n");
            for(int i = 0; i < 16; i++){
                printf("%04x: ", i*16 + address);
                for(int j = 0; j < 16; j++){
                    printf("%02x ", this->_mem->read((i*16)+j + address) );
                }
                printf("\n");
            }
        }
        if(std::string(Inst) == "p"){
            while(true){
                printf("Data: ");
                char* data = new char[256];
                std::cin.getline(data, 256);
                if(std::string(data) == ""){
                    break;
                }
                this->_mem->put(address, (uint8_t)std::stoi(std::string(data), nullptr, 16));
                printf("Stored %02x at %04x\n", this->_mem->read(address), address);
                address++;
            }
        }

    }
}