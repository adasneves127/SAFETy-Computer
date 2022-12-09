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
    uint8_t instruction = this->_mem->read(this->_mem->getPC());
    this->_mem->nextIns();
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
        if(std::string(Inst) == "exit" || std::string(Inst) == "q"){
            return;
        }
        if(std::string(Inst) == "x"){
            //Execute
            //Convert address to hex
            uint16_t address = (uint16_t)std::stoi(std::string(addr), nullptr, 16);
            this->_mem->jump(address);
            this->doInstruction();
        }
        if(std::string(Inst) == "i"){
            //Dump
            printf("Dumping Memory:\n");
            printf("      00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F\n");
            //Command Format: i <start>:<end>
            
            std::string address = std::string(addr);

            uint16_t start;
            uint16_t end;

            if(address.find(":") == std::string::npos){
                start = (uint16_t)std::stoi(address, nullptr, 16);
                end = start + 255;
            } else{

                start = (uint16_t)std::stoi(address.substr(0, address.find(":")), nullptr, 16);
                end = (uint16_t)std::stoi(address.substr(address.find(":") + 1), nullptr, 16);
            }
            for(uint16_t i = 0; i <= end - start; i++){
                if(i % 16 == 0){
                    printf("%04x: ", i + start);
                }
                printf("%02x ", this->_mem->read(i + start));
                if(i % 16 == 15){
                    printf("\n");
                }
            }
        }
        if(std::string(Inst) == "p"){
            uint16_t address = (uint16_t)std::stoi(std::string(addr), nullptr, 16);

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
        if(std::string(Inst) == "h"){
            printf("\n");
            printf("Commands:\n");
            printf("\tx <address> - Execute instruction at address\n");
            printf("\ti <start>:<end> - Inspect memory from start to end\n");
            printf("\tp <address> - Put data at address\n");
            printf("\th - Help\n");
            printf("\texit - Exit\n");
        }

    }
}