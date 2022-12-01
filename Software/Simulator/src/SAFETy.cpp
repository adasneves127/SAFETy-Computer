#include "Registers.h"
#include "Memory.h"
#include <iostream>
#include <string>
#include "Monitor.h"
#include "ALU.h"
#include "Control.h"

Registers* _reg;
Memory* _mem;
Monitor* _mon;
ALU* _alu;
controlUnit* _control;


unsigned char options = 0;
char* fileNames[3];

int main(int argc, char** argv){

    for(int i = 1; i < argc; i++){
        if(std::string(argv[i]) == "-h"){
            std::cout << "SAFETy Simulator\n";
            std::cout << "Usage: SAFETy <ROM File> <RAM File> <Headers File> [-d] [-m]\n";
            return 0;
        }
        if(std::string(argv[i]) == "-m"){
            options |= 0b00000001;
        }
        if(std::string(argv[i]) == "-d"){
            options |= 0b00000010;
        }
    }
    if(argc < 4){
        std::cout <<"\u001b[31mErr:\u001b[0m Invalid Arguments\n";
        return EXIT_FAILURE;
    }

    fileNames[0] = argv[1];
    fileNames[1] = argv[2];
    fileNames[2] = argv[3];

    _reg = new Registers();

    _reg->printDebug();

    _reg->set(0, 14);
    _reg->printDebug();

    _mem = new Memory();

    _mem->loadRAM(fileNames[1]);
    _mem->loadROM(fileNames[0]);
    _mem->loadHeaders(fileNames[2]);

    if(options & 0b00000001){
        _mon = new Monitor();
        _mon->init(_mem, _reg, _alu, _control);
        _mon->run();
    }

}