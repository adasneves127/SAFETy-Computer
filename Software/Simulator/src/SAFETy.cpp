#include "Registers.h"
#include "Memory.h"
#include <iostream>
#include <string>
#include "Monitor.h"
#include "ALU.h"
#include "Control.h"
#include <thread>

Registers* _reg;
Memory* _mem;
Monitor* _mon;
ALU* _alu;
controlUnit* _control;


uint8_t options = 0;
char* fileNames[3];

void loadRam(char* fileName);
void loadRom(char* fileName);
void loadHeaders(char* fileName);

int main(int argc, char** argv){

    for(int i = 1; i < argc; i++){
        if(std::string(argv[i]) == "-h"){
            std::cout << "SAFETy Simulator\n";
            std::cout << "Usage: SAFETy [[-e] | <ROM File> <RAM File> <Headers File>] [-d] [-m]\n";
            std::cout << "Options:\n";
            std::cout << "\t-e\t\tInitialize the Simulator with empty memory contents\n";
            std::cout << "\t-d\t\tEnable Debug Mode\n";
            std::cout << "\t-m\t\tEnable Monitor Mode\n";
            return 0;
        }
        if(std::string(argv[i]) == "-m"){
            options |= 0b00000001;
        }
        if(std::string(argv[i]) == "-d"){
            options |= 0b00000010;
        }
        if(std::string(argv[i]) == "-e"){
            options |= 0b00000100;
        }
    }
    if(argc < 4 && !(options & 0b00000100)){
        std::cout <<"\u001b[31mErr:\u001b[0m Invalid Arguments\n";
        return EXIT_FAILURE;
    }

    if((options & 0b00000100)){
        fileNames[0] = (char*)"/dev/zero";
        fileNames[1] = (char*)"/dev/zero";
        fileNames[2] = (char*)"/dev/zero";
    } else {
        fileNames[0] = argv[1];
        fileNames[1] = argv[2];
        fileNames[2] = argv[3];
    }

    _reg = new Registers();

    _reg->printDebug();

    _reg->set(0, 14);
    _reg->printDebug();

    _mem = new Memory();

    _alu = new ALU();

    _control = new controlUnit(_reg, _alu, _mem);

    std::thread RamThread(loadRam, fileNames[1]);
    std::thread RomThread(loadRom, fileNames[0]);
    std::thread HeadThread(loadHeaders, fileNames[2]);   

    if(options & 0b00000001){
        _mon = new Monitor();
        _mon->init(_mem, _reg, _alu, _control);
        _mon->run();
    }

    RamThread.join();
    RomThread.join();
    HeadThread.join();
    delete _reg;
    delete _mem;
}

void loadRam(char* fileName){
    _mem->loadRAM(fileName);
}
void loadRom(char* fileName){
    _mem->loadROM(fileName);
}
void loadHeaders(char* fileName){
    _mem->loadHeaders(fileName);
}